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

#include "cpgi.h"
#include <sstream>
#include <algorithm>

/*
 * scan cpg islands
 */
bool CPGIsland::Scan(int LENGTH, float GCC, float OE, int gap,
                     const char* sequence_file, const char* output_file, const int output_format)
{
    // preparing sequence
    if (!genome_.PrepareSequence(sequence_file))
        return false;
    
    // scanning cpg islands
    scanner_.Scan(LENGTH, GCC, OE, gap);
    
    // wait scanning finished
    genome_.Join();
    
    scanner_.Join();
    
    // save results
    Print(output_file, output_format);
    
    return true;
}

/*
 * save the
 */
void CPGIsland::Print(const char* output_file, const int output_format)
{
    std::vector<CPGI> cpgis;
    std::ofstream fout;
    std::ostringstream line;

    //
    // sort firstly
    Sort(cpgis);
    
    if (output_format == OUTPUTFILE_FORMAT_GFF || output_format == OUTPUTFILE_FORMAT_DEFAULT)
    {
        //line << kResultFileHeaderGFF << std::endl;
        for (auto cpgi : cpgis)
        {
            line << cpgi.chrom << '\t' << CPGISCAN_NAME <<'\t' << "CpGIsland" << '\t'
            << cpgi.start << '\t' << cpgi.end
            << '\t'<< "."          << '\t' << "." << '\t' << "." << '\t'
            << "LENGTH=" << cpgi.length <<"; " <<"GCC=" << cpgi.gcc << "; " << "ObsCpG/ExpCpG=" << cpgi.oe << "; "
            << "CpGNum=" << cpgi.cg << "; " << std::endl;
        }
    }else if (output_format == OUTPUTFILE_FORMAT_FIELD)
    {
        //line << kResultFileHeaderField << std::endl;
        for (auto cpgi : cpgis)
        {
            line << cpgi.chrom << '\t' << cpgi.start << '\t' << cpgi.end << '\t'
            << cpgi.length << '\t' << cpgi.gcc << '\t' << cpgi.oe << '\t'
            << cpgi.cg << '\t' << std::endl;
        }
    }
    
    // write to file or stdout
    fout.open(output_file);
    if (fout.is_open())
    {
        fout << line.str();
    }else
    {
        std::cout << line.str();
    }
}

/*
 * Sort CpG islands
 */
void CPGIsland::Sort(std::vector<CPGI>& cpgis)
{
    std::vector<int> chrom_IDs;
    std::vector<CPGI> cpgi_sorted;
    cpgis = scanner_.GetCpGIs();
    
    for (auto& cpgi : cpgis)
        chrom_IDs.push_back(cpgi.chrom_ID);
    
    // unique
    auto iter = std::unique(chrom_IDs.begin(), chrom_IDs.end());
    if (iter != chrom_IDs.end())
        chrom_IDs.erase(iter, chrom_IDs.end());
    
    // sort chrom IDs
    std::sort(chrom_IDs.begin(), chrom_IDs.end());

    // sort CpG islands
    for (auto& chrom_ID : chrom_IDs)
    {
        for (auto& cpgi : cpgis)
        {
            if (cpgi.chrom_ID == chrom_ID)
                cpgi_sorted.push_back(cpgi);
        }
    }
    cpgis = cpgi_sorted;
    
    //std::sort(std::begin(cpgis), std::end(cpgis), [](const CPGI& cgi_a, const CPGI& cgi_b){return cgi_a.no < cgi_b.no;});   // some system don't support this style
}
