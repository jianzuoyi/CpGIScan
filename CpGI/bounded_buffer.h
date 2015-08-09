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

#ifndef BOUNDED_BUFFER_H_
#define BOUNDED_BUFFER_H_

#include <mutex>
#include <condition_variable>

template<typename T>
class BoundedBuffer
{
public:
    typedef T* pointer;
    
    BoundedBuffer(int capacity):size_(0)
    {
        buff_ = new T[capacity];
        end_  = buff_ + capacity;
        first_ = last_ = buff_;
    }
    ~BoundedBuffer()
    {
        if (buff_ != nullptr)
        {
            delete []buff_;
            buff_ = nullptr;
        }
    }
    
    void push(const T& item)
    {
        std::unique_lock<std::mutex> lck(mutex_);
        while (full())
            not_full_.wait(lck);
        
        if (full())
        {
            if (empty()) return;
            *last_ = item;
            increment(last_);
            first_ = last_;
        }
        else
        {
            *last_ = item;
            increment(last_);
            ++size_;
        }
        lck.unlock();
        not_empty_.notify_one();
    }
    void pop(T* pItem)
    {
        std::unique_lock<std::mutex> lck(mutex_);
        while (empty())
            not_empty_.wait(lck);
        
        if (empty()) return;
        *pItem = *first_;
        increment(first_);
        --size_;
        
        lck.unlock();
        not_full_.notify_one();
    }
    
    bool empty() const{return size() == 0;}
    bool full() const{return capacity() == size();}
    long  capacity() const{return end_ - buff_;}
    long  size() const{return size_;}
private:
    //! Increment the pointer.
    //template <typename Pointer>
    void increment(pointer& p) const
    {
        if (++p == end_)
            p = buff_;
    }
    
    pointer  buff_;
    pointer  end_;
    pointer  first_;
    pointer  last_;
    int      size_;
    
    // async
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    
};

#endif // BOUNDED_BUFFER_H_
