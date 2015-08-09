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

#include <iostream>
#include <getopt.h>
#include "cpgi.h"
#include "bounded_buffer.h"

using namespace std;

// parameters
static int      length;
static int      gcc;
static float    oe;
static int      gap;
static string   sequencefile;
static string   output_file;
static int      output_format;
static const char *short_options = "l:c:e:g:G:F:h";

static struct option long_options[] = {
    {"length",      required_argument,      0,      'l'},
    {"gcc",         required_argument,      0,      'c'},
    {"oe",          required_argument,      0,      'e'},
    {"gap",         required_argument,      0,      'g'},
    {"help",        no_argument,            0,      'h'},
    {"GFF",         required_argument,      0,      'G'},
    {"field",       required_argument,      0,      'F'},
    {0, 0, 0, 0}    
};

static void InitOptions()
{
    length  = 500;          // length of CpG island
    gcc     = 55;           // content of G and C
    oe      = 0.65;         // ObsCpG/ExpCpG
    gap     = 100;          // gap for merge two neighboring CpG island
    sequencefile.clear();   // file name of input sequence
    output_file.clear();     // file for GFF output (default: stdout)
    output_format=OUTPUTFILE_FORMAT_DEFAULT;
}

/*
 * Print a detailed message to the specified output stream
 */
void PrintUsage(ostream& out)
{
    std::string tool_name = "cpgiscan";
    
    out << tool_name << " version " << CpGIScan_VERSION << " by Zuoyi Jian (jzuoyi@126.com)" << endl
        << "Usage: "
        << tool_name << " [options] <sequencefile>" << endl
        //<< "example:" <<" cpgiscan --length 500 --gcc 55 --oe 65 -G chr21.gff chr21.fa" << endl << endl
        << "  <sequencefile>  Input file to scan CpG islands." << endl
        << "                  Could be FASTA or multi-FASTA format." << endl
        << "Options:" << endl
        << endl << " Criteria:" << endl
        << "  --length <int> length of lower limit (200-1500,default value:500bp)" << endl
        << "  --gcc <int>    %GC of lower limit (50-70,default value:55%)" << endl
        << "  --oe <float>   ObsCpG/ExpCpG of lower limit (0.60-1.00,default value:0.65)" << endl
        << endl << " Merge:" << endl
        << "  --gap <int>    merge two neighboring CpG islands when their distance is" << endl
        << "                 lower than the gap(e.g. < 100nt)" << endl
        << endl << " Output:" << endl
        << "  -G <path>      file for GFF output (default: stdout)" << endl
        << "  -F <path>      file for fields output" << endl
        << endl << " Other:" << endl
        << "  -h, --help     print this usage message"
        << "" << endl;
}

/*
 * Parse an int out of optarg and enforce that it be at least 'lower';
 * if it is less than 'lower', then output the given error message and
 * exit with an error and a usage message.
 */
template<typename T>
static T ParseNumber(T lower, const char* errmsg)
{
    string fs = optarg;
    T number = (T)std::stof(fs);
    if (number < lower)
    {
        cerr << errmsg << endl;
        cerr << std::string("Invalid arguments: ") + optarg << endl;
        PrintUsage(cerr);
        throw 1;
    }
    return number;
}

/*
 * Parse command-line arguments
 */
static bool ParseOptions(int argc, const char* argv[])
{
    int option;
    bool abort = false;

    while ((option = getopt_long(argc, const_cast<char**>(argv), short_options, long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'l':
                length = ParseNumber<int>(200, "--length argument must be at least 200");
                break;
            case 'c':
                gcc    = ParseNumber<int>(50, "--gcc argument must be at least 55");
                break;
            case 'e':
                oe  = ParseNumber<float>(0.60, "--oe argument must be at least 0.65");
                break;
            case 'g':
                gap = ParseNumber<int>(100, "--gap argument must be at leat 100");
                break;
            case 'G':
                output_format = OUTPUTFILE_FORMAT_GFF;
                output_file = optarg;
                break;
            case 'F':
                output_format = OUTPUTFILE_FORMAT_FIELD;
                output_file = optarg;
                break;
            case 'h':
                PrintUsage(cout);
                abort = true;
                break;
            case '?':
                PrintUsage(cerr);
                throw 1;
                break;
            default:
                PrintUsage(cerr);
                throw 1;
                break;
        }
    }
    return abort;
}

/*
 * main function.
 */
int main(int argc, const char *argv[])
{
    string  infile;
    
    try
    {
        InitOptions();
        
        if (ParseOptions(argc, argv))
        {
            return 0;
        }
        
        // Get input filename
        if (optind >= argc)
        {
            cerr << "No sequence file specified!" << endl;
            PrintUsage(cerr);
            throw 1;
        }
        sequencefile = argv[optind++];
        
        
        CPGIsland cpgi;
        
        cpgi.Scan(length, gcc, oe, gap, sequencefile.c_str(), output_file.c_str(), output_format);
        
        return 0;
    } catch (const std::exception& e)
    {
        cerr << "Exception: " << e.what() << "'" << endl;
        cerr << "Command: ";
        for (int i = 0; i < argc; i++) cerr << argv[i] << " ";
        cerr << endl;
        return 1;
    }catch (int e)
    {
        if (e != 0)
        {
            //cerr << "Exception: parameter is not valid, -h/--help for help (#" << e << ")" << endl;
            cerr << "Command: ";
            for (int i = 0; i < argc; i++) cerr << argv[i] << " ";
            cerr << endl;
        }
        return e;
    }
}

