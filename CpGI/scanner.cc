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

#include "scanner.h"

int Scanner::threads_num_ = 0;
/*
 * Scan cpg islands
 */
void Scanner::Scan(int CGI_LENGTH, float CGI_GCC, float CGI_OE, int gap)
{
    // Get parameters
    criteria_.length = CGI_LENGTH;
    criteria_.gcc    = CGI_GCC;
    criteria_.oe     = CGI_OE;
    criteria_.cg     = (criteria_.oe * criteria_.length) / 16;
    criteria_.gap    = gap;
    
    DoScan();
}

void Scanner::DoScan()
{    
    //RunTime rt("Scanner::FindAllCPGI");
    
    ChromoSome* chrom = nullptr;
    unsigned int concurrency = std::thread::hardware_concurrency();
    concurrency = concurrency > 1 ? concurrency : 1;
    
    // Scan CpG islands until pickup a nullptr from the shared_buffer
    do
    {
        shared_buffer_.pop(&chrom);
        
        if (chrom != nullptr)
        {
            StartScan();
            ScanCpGI(*chrom);
        }
    } while (chrom != nullptr);
}

void Scanner::StartScan()
{
    std::unique_lock<std::mutex> ul(mutex_);
    
    unsigned int concurrency = std::thread::hardware_concurrency();
    concurrency = concurrency > 1 ? concurrency : 1;
    
    if (threads_num_ < static_cast<int>(concurrency)-1)
    {
        threads_.push_back(std::thread(std::bind(&Scanner::DoScan, this)));
        ++threads_num_;
    }
}

/*
 * Thread: join
 */
void Scanner::Join()
{
    for (auto& thread : threads_)
    {
        thread.join();
    }
}

/*
 * Scan CpG islands of a chromosome
 */
void Scanner::ScanCpGI(ChromoSome& chrom)
{
    if (static_cast<int>(chrom.fasta.sequence.length()) < criteria_.length)
        return;
    
    // when exit this scope, free the chrom
    std::unique_ptr<ChromoSome> up_chrom;
    up_chrom.reset(&chrom);
    
    std::vector<CPGI>   cpgis_chrom;
    CPGI                cpgi, cpgi_first, cpgi_final;
    SlidingWindow       sliding_window;
    
    cpgi.chrom = up_chrom->fasta.chrom;
    cpgi.chrom_ID = up_chrom->ID;
    
    const char* const sequence = up_chrom->fasta.sequence.c_str();
    const char* begin = sequence;
    const char* first = nullptr;
    const char* last  = nullptr;
    const char* begin_last_window = begin + (up_chrom->fasta.sequence.length() - criteria_.length + 1);
    
    DenovoCountWindowParams(sequence, begin, criteria_.length, sliding_window);
    while (begin < begin_last_window)
    {
        // A. find the first window which meet the criteria
        // 5' --------------------------------------------------> 3'
        if (!IsCpGI(sliding_window, cpgi))
        {
            ++begin;
            if (begin == begin_last_window)
                break;
            ReCountWindowParams(sequence, begin, criteria_.length, sliding_window);
        }
        else
        {
            // the first window meet the criteria
            first      = begin;
            cpgi_first = cpgi;
            
            // B. shfit the window 1 bp until it do not meet the criteria
            // 5' --------------------------------------------------> 3'
            do
            {
                ++begin;
                if (begin == begin_last_window)
                {
                    begin = begin_last_window - 1;
                    break;
                }
                ReCountWindowParams(sequence, begin, criteria_.length, sliding_window);
            } while (IsCpGI(sliding_window, cpgi));
            last = begin + criteria_.length;
            
            // C. Trim the large window until it meet the criteria
            // 5' >--------------------------------------------------< 3'
            bool flag = true;

            DenovoCountWindowParams(sequence, first, int(last - first), sliding_window);
            while (!IsCpGI(sliding_window, cpgi))
            {
                if (flag)
                {
                    flag = false;
                    --last;
                }
                else
                {
                    flag = true;
                    ++first;
                }
                if (int(last - first) + 1 < criteria_.length)
                {
                    cpgi = cpgi_first;
                    break;
                }
                DenovoCountWindowParams(sequence, first, int(last - first), sliding_window);
            }
            // the final CpG island : cpgi
            cpgi_final = cpgi;
            
            // D. merge the last two CpG islands
            if (cpgis_chrom.size() > 0)
            {
                if (cpgi_final.start - cpgis_chrom.back().end < criteria_.gap)
                {
                    first = sequence + cpgis_chrom.back().start-1;
                    last = sequence + cpgi_final.end-1;
                    DenovoCountWindowParams(sequence, begin, static_cast<int>(last - first) + 1, sliding_window);
                    if (IsCpGI(sliding_window, cpgi))
                    {
                        cpgis_chrom.pop_back();
                        cpgi_final = cpgi;
                        //std::cout << "merged" << std::endl;
                    }
                }
            }
            cpgis_chrom.push_back(cpgi_final);
            
            // next CpG island
            begin = sequence + cpgis_chrom.back().end;
            DenovoCountWindowParams(sequence, begin, criteria_.length, sliding_window);
            
        } // end if
    } // end while
    
    // found all CpG islands of the chromosome
    PushCpGI(cpgis_chrom);
}

/*
 * denovo count window's parameters
 */
void Scanner::DenovoCountWindowParams(const char* chrom, const char* begin, int length,SlidingWindow& sliding_window)
{
    // initialize sliding_window
    sliding_window.start = int(begin - chrom) + 1;
    sliding_window.end   = sliding_window.start + (length - 1);
    sliding_window.length= length;
    sliding_window.gcc   = 0.0;
    sliding_window.oe    = 0.0;
    sliding_window.c     = 0;
    sliding_window.g     = 0;
    sliding_window.cg    = 0;
    // count C G CpG
    for (int i = 0; i < length; i++)
    {
        if (begin[i] == kDNA_Alphabet_Uper_C || begin[i] == kDNA_Alphabet_Lower_C)
        {
            ++sliding_window.c;
        }
        else if (begin[i] == kDNA_Alphabet_Uper_G || begin[i] == kDNA_Alphabet_Lower_G)
        {
            ++sliding_window.g;
        }
        
        if (i < length - 1)
        {
            if ((begin[i]==kDNA_Alphabet_Uper_C || begin[i]==kDNA_Alphabet_Lower_C)
                && (begin[i+1]==kDNA_Alphabet_Uper_G || begin[i+1]==kDNA_Alphabet_Lower_G))
            {
                ++sliding_window.cg;
            }
        }
    }
    if (sliding_window.c != 0 && sliding_window.g != 0)
    {
        sliding_window.gcc    = int(((sliding_window.g*1.0 + sliding_window.c) / sliding_window.length) * 1000) / 10.0;
        sliding_window.oe     = int(((sliding_window.cg*1.0*sliding_window.length) / sliding_window.c / sliding_window.g) * 1000) / 1000.0;
    }
}

/*
 * re count window's parameters
 */
void Scanner::ReCountWindowParams(const char* chrom, const char* begin, int length, SlidingWindow& sliding_window)
{
    // if c, g, cg content dont change, the gcc, oe will not change too
    sliding_window.start = int(begin - chrom) + 1;
    sliding_window.end   = sliding_window.start + length - 1;
    sliding_window.length= length;

    int prior_c = sliding_window.c;
    int prior_g = sliding_window.g;
    int prior_cg= sliding_window.cg;
    
    // shift out one char
    if (*(begin-1) == kDNA_Alphabet_Uper_C || *(begin-1) == kDNA_Alphabet_Lower_C)
    {
        --sliding_window.c;
    }
    else if (*(begin-1) == kDNA_Alphabet_Uper_G || *(begin-1) == kDNA_Alphabet_Lower_G)
    {
        --sliding_window.g;
    }
    
    if ((*(begin-1) == kDNA_Alphabet_Uper_C || *(begin-1) == kDNA_Alphabet_Lower_C)
        && (*begin == kDNA_Alphabet_Uper_G || *begin == kDNA_Alphabet_Lower_G))
    {
        --sliding_window.cg;
    }
    // shift in one char
    if (begin[length-1] == kDNA_Alphabet_Uper_C || begin[length-1] == kDNA_Alphabet_Lower_C)
    {
        ++sliding_window.c;
    }
    else if (begin[length-1] == kDNA_Alphabet_Uper_G || begin[length-1] == kDNA_Alphabet_Lower_G)
    {
        ++sliding_window.g;
    }
    
    if ((begin[length-2] == kDNA_Alphabet_Uper_C || begin[length-2] == kDNA_Alphabet_Lower_C)
        && (begin[length-1] == kDNA_Alphabet_Uper_G || begin[length-1] == kDNA_Alphabet_Lower_G))
    {
        ++sliding_window.cg;
    }
    
    // recalculate the GCC, OE
    if (sliding_window.c != prior_c
        || sliding_window.g != prior_g
        || sliding_window.cg != prior_cg)
    {
        if (sliding_window.c != 0 && sliding_window.g != 0)
        {
            sliding_window.gcc    = int(((sliding_window.g*1.0 + sliding_window.c) / sliding_window.length) * 1000) / 10.0;
            sliding_window.oe     = int(((sliding_window.cg*1.0*sliding_window.length) / sliding_window.c / sliding_window.g) * 1000) / 1000.0;
        }
    }
}

/*
 * Whether a window meet the criteria of CpG island
 */
bool Scanner::IsCpGI(SlidingWindow &sliding_window, CPGI& cpgi)
{
    if (sliding_window.cg     >= criteria_.cg &&
        sliding_window.gcc    >= criteria_.gcc &&
        sliding_window.oe     >= criteria_.oe &&
        sliding_window.length >= criteria_.length
        )
    {
        cpgi.start = sliding_window.start;
        cpgi.end   = sliding_window.end;
        cpgi.length= sliding_window.length;
        cpgi.gcc   = sliding_window.gcc;
        cpgi.oe    = sliding_window.oe;
        cpgi.cg    = sliding_window.cg;
        
        return true;
    }
    return false;
}

/*
 * Push CpG islands into a container
 */
void Scanner::PushCpGI(std::vector<CPGI>& cpgis)
{
    std::unique_lock<std::mutex> ul(mutex_);
    
    for (auto cpgi : cpgis)
    {
        cpgis_.push_back(cpgi);
    }
}

