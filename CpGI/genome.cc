/*
 * This file is part of CpGIScan
 *
 * Copyright (C) 2014, Zuoyi Jian <jzuoyi@126.com>
 *
 * CpGIScan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CpGIScan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CpGIScan.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "genome.h"

/*
 * Thread: join
 */
void Genome::Join()
{
    thread_.join();
}

/*
 * Open fasta file and start threads
 */
bool Genome::PrepareSequence(const char *fasta_file)
{
    if (!fasta_.Open(fasta_file))
        return false;
    
    thread_ = std::thread(std::bind(&Genome::Prepare, this));
    
    return true;
}


/*
 * Read and push all sequences into a share buffer
 */
void Genome::Prepare()
{
    bool next;
    ChromoSome* chromo_some;
    int chrom_ID = 0;
    unsigned int concurrency = std::thread::hardware_concurrency();
    concurrency = concurrency > 1 ? concurrency : 1;
    int chrom_num = 0;
    int nullptr_num = 0;
    
    do
    {
        chromo_some = new ChromoSome();
        next = fasta_.NextSequence(chromo_some->fasta);
        // push sequence into share buffer
        if (next)
        {
            chrom_num++;
            chromo_some->ID = ++chrom_ID;
            shared_buffer_.push(chromo_some);
        }
        else
        {
            nullptr_num = chrom_num < static_cast<int>(concurrency) ? chrom_num+1 : static_cast<int>(concurrency);
            //for (int i = 0; i < static_cast<int>(concurrency); i++)   // notify all scanning threads exit
            for (int i = 0; i < nullptr_num; i++)   // notify all scanning threads exit
            {
                shared_buffer_.push(nullptr);
            }
        }
    }while (next);
}

