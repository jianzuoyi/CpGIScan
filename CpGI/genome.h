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

#ifndef GENOME_H_
#define GENOME_H_

#include "bounded_buffer.h"
#include "define.h"
#include "fasta.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <thread>
#include <future>

// open fasta file, get sequence and put them into shared buffer;
class Genome
{
public:
    typedef BoundedBuffer<ChromoSome*> container_type;
    
    Genome(container_type& share_buffer):shared_buffer_(share_buffer){};
    ~Genome(){};
    
    bool PrepareSequence(const char* fasta_file);
    void Join();
    //std::vector<std::string> get_chrom_names(){return chrom_names_;};
private:
    void Prepare();
    
    container_type&     shared_buffer_;
    Fasta               fasta_;
    std::thread         thread_;
    //std::future<bool>   future_done_;
    //std::vector<std::string> chrom_names_;
};


#endif // GENOME_H_
