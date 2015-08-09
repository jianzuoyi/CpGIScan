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

#include "fasta.h"

Fasta::Fasta()
{
    chrom_   = "";
}

Fasta::Fasta(const char* filePath)
{
    fin_.open(filePath);
    if (!fin_.is_open())
    {
        std::cout << "Can't open the file: " << filePath << std::endl;
    }
    chrom_   = "";
}

Fasta::~Fasta()
{
    
}

/*
 * Open fasta file
 */
bool Fasta::Open(const char* filePath)
{
    if (!fin_.is_open())
    {
        fin_.open(filePath);
        if (!fin_.is_open())
        {
            std::cout << "Can't open the file: " << filePath << std::endl;
        }
    }
    return fin_.is_open();
}

/*
 * Read next sequence
 */
bool Fasta::NextSequence(FastaRecord& fasta_record)
{
    try
    {
        if (!fin_.is_open())
        {
            std::cout << "file not opened" << std::endl;
            return false;
        }
        //cout << "nextSequence()..." << endl;
        std::string line;
        
        fasta_record.chrom      = "";
        fasta_record.sequence   = "";
        
        if (chrom_ != "")
        {
            fasta_record.chrom = chrom_.substr(1);
        }
        //
        while (getline(fin_, line))
        {
            if (line[0] == '>')            // must begin with '>'
            {
                
                if (chrom_ != "") // not the first line
                {
                    chrom_ = line;
                    return true;
                }
                else
                {
                    chrom_ = line;         // the first chrom
                    fasta_record.chrom = chrom_.substr(1);
                }
                
            }
            else
            {
                if (fasta_record.chrom != "")
                    fasta_record.sequence += line;            // sequence
            }
        } // end while
        
        return (fasta_record.chrom != "" && fasta_record.sequence.length() > 0);
    } catch (...)
    {
        
    }
    return false;
}