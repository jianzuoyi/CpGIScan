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

#ifndef SCANNER_H_
#define SCANNER_H_

#include "bounded_buffer.h"
#include "define.h"
#include <thread>
#include <future>
#include <mutex>
#include <vector>

/*
 * Scan CpG islands
 */
class Scanner
{
public:
    typedef BoundedBuffer<ChromoSome*> container_type;

    Scanner(container_type& share_buffer):shared_buffer_(share_buffer) {cpgis_.clear();threads_.clear();};
    ~Scanner(){ };
    
    void Scan(int LENGTH, float GCC, float OE, int gap);
    void Join();
    std::vector<CPGI> GetCpGIs(){return cpgis_;};
private:
    void StartScan();
    void DoScan();
    void ScanCpGI(ChromoSome& chrom);
    bool IsCpGI(SlidingWindow& move_window, CPGI& cpgi);
    void DenovoCountWindowParams(const char* chrom, const char* begin, int length, SlidingWindow& move_window);
    void ReCountWindowParams(const char* chrom, const char* begin, int length, SlidingWindow& move_window);
    void PushCpGI(std::vector<CPGI>& cpgis);
    
    container_type&             shared_buffer_;
    std::vector<CPGI>           cpgis_;
    std::vector<std::thread>    threads_;
    static int                  threads_num_;
    std::mutex                  mutex_;
    Criteria                    criteria_;
};

#endif // SCANNER_H_
