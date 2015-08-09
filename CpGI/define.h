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

#ifndef DEFINE_H_
#define DEFINE_H_

#include "RunTime.h"
#include <string>
#include "fasta.h"

// VERSION
const std::string CpGIScan_VERSION= "1.0";
#define CPGISCAN_NAME   "cpgiscan"
// shared buffer's length
const int   kBufferLength         = 10;

// DNA_Alphabet
const char kDNA_Alphabet_Uper_A  = 'A';
const char kDNA_Alphabet_Lower_A = 'a';
const char kDNA_Alphabet_Uper_T  = 'T';
const char kDNA_Alphabet_Lower_T = 't';
const char kDNA_Alphabet_Uper_G  = 'G';
const char kDNA_Alphabet_Lower_G = 'g';
const char kDNA_Alphabet_Uper_C  = 'C';
const char kDNA_Alphabet_Lower_C = 'c';

// result file header
const std::string kResultFileHeaderGFF = "#seqname\tsource\tfeature\tstart\tend\tscore\tstrand\tframe\tattribute";
const std::string kResultFileHeaderField = "#Seqname\tStart\tEnd\tLength\tGCContent\tObsCpG/ExpCpG\tCpGNum";

enum OutputFileFormat
{
    OUTPUTFILE_FORMAT_DEFAULT=255,
    OUTPUTFILE_FORMAT_GFF,
    OUTPUTFILE_FORMAT_FIELD,
};

struct Criteria
{
    int     length;
    float   gcc;
    float   oe;
    int     cg;             // cg = (oe * length) / 16
    int     gap;
};

struct ChromoSome
{
    int         ID;
    FastaRecord fasta;
    ChromoSome()
    {
        ID = 0;
    }
};

struct CPGI
{
    std::string chrom;
    int         chrom_ID;
    int         start;
    int         end;
    int         length;
    float       gcc;
    float       oe;
    int         cg;
    CPGI()
    {
        chrom = "";
        chrom_ID= 0;
        start = 0;
        end   = 0;
        length= 0;
        gcc   = 0;
        oe    = 0;
        cg    = 0;
    }
};

struct SlidingWindow
{
    int     start;
    int     end;
    int     length;
    float     gcc;
    float     oe;
    int     c;
    int     g;
    int     cg;
    SlidingWindow()
    {
        length= 0;
        c     = 0;
        g     = 0;
        cg    = 0;
    }
};

#endif // DEFINE_H_
