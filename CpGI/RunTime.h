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

#ifndef RUNTIME_H_
#define RUNTIME_H_
#include <string>
#include <iostream>
#include <time.h>

class RunTime
{
public:
    RunTime(std::string func_name):func_name_(func_name){time_ = time(NULL);};
    ~RunTime(){std::cout << func_name_ << " run time: " << time(NULL)-time_ << std::endl;};
    
private:
    std::string func_name_;
    time_t time_;
};

#endif // RUNTIME_H_
