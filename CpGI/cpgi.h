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

#ifndef CPGISLAND_H_
#define CPGISLAND_H_

#include "bounded_buffer.h"
#include "define.h"
#include "genome.h"
#include "scanner.h"
#include <string>
#include <thread>

// class CpG island
class CPGIsland
{
public:
    typedef BoundedBuffer<ChromoSome*> container_type;
    
    explicit CPGIsland():genome_(shared_buffer_),
		scanner_(shared_buffer_),shared_buffer_(kBufferLength){};
    ~CPGIsland(){};
    
    bool Scan(int LENGTH, float GCC, float OE, int gap,
              const char* sequence_file, const char* output_file, const int output_format);
    
public:
    void Print(const char* result_file, const int output_format);
    void Sort(std::vector<CPGI>& result_cpgis);
    
    Genome          genome_;
    Scanner         scanner_;
    container_type  shared_buffer_;
};

#endif // CPGISLAND_H_
