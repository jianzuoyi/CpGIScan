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

#ifndef FASTA_H_
#define FASTA_H_

#include <iostream>
#include <fstream>
#include <string>

struct FastaRecord
{
    std::string chrom;
    std::string sequence;
};

/*
 * Read FASTA file
 */
class Fasta
{
public:
    Fasta();
    Fasta(const char* filepath);
    ~Fasta();
    
    bool Open(const char* filePath);
    bool NextSequence(FastaRecord& fasta_record);
private:
    std::ifstream    fin_;
    std::string      chrom_;
};

#endif // FASTA_H_
