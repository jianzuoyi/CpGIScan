What is CpGIScan?
-----------------
CpGIScan, an ultrafast tool for CpG islands identification from genome sequences. 
It uses C++, and works on Linux, OS X and Windows. 


Download CpGIScan
------------------
Binaries and sources can be downloaded from [github site](https://github.com/jzuoyi/cpgiscan "github"). Binaries are available for the Intel x86_64 architecture running Linux, OS X and Windows. You also are allowed to compile CpGIScan yourself.

Build from source
--------------------
First, download the sources from this repo, and change to the code directory
   
         $ cd cpgiscan

Then, run make with no arguments

         $ make
      
CpGIScan employs the multithreading software model to reduce the running time for CpG islands identification. It requires the pthread library on POSIX platforms (like Linux, OS X, etc) and non-POSIX platform like Windows. 

Build requirements
------------------
  * CentOS 6.3, Make 3.81+
  * or Mac OS X 10.7+, Xcode 4.6+
  * or Windows 7+, MinGW-64 4.9+

Use of CpGIScan
---------------
##### Usage: #####

    $ cpgiscan [options] <sequencefile>
    
      <sequencefile>  Input file to scan CpG islands (FASTA or multi-FASTA format).

##### Options: #####

    Criteria:
     --length <int> length of lower limit (200-1500,default value:500bp)
     --gcc <int>    %GC of lower limit (50-70,default value:55%)
     --oe <float>   ObsCpG/ExpCpG of lower limit (0.60-1.00,default value:0.65)
    Merge:
     --gap <int>    merge two neighboring CpG islands when their distance is
                    lower than the gap(e.g. < 100nt)
    Output:
     -G <path>      file for GFF output (default: stdout)
     -F <path>      file for fields output
    Other:
     -h, --help     print this usage message
     
##### Examples: #####

   * Run CpGIScan with default parameter values (length ≥ 500, G + C ≥ 55%, CpG o/e ratio ≥ 0.65)
   
         $ ./cpgiscan chr21.fa
    
   * You can define the minimum values of parameters of CpG islands
   
         $ ./cpgiscan --length 500 --gcc 55 --oe 0.65 chr21.fa
    
   * By default, CpGIScan send the results to the stdout; If you want to save the results to a disk file, you can run
   
         $ ./cpgiscan chr21.fa > chr21.CpG

Contributing to the project
---------------------------
Did you find a bug? Do you have feature request? Do you want to merge a feature? 
Please do not hesitate to contact us. 

Contact us
-----------
   * Email: zxfan068@gmail.com (Zhenxin Fan)   jzuoyi@126.com (Zuoyi Jian)

