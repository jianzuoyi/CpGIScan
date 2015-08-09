# position format: chrom-start-end eg. 1-100-200
# genome file: multi-fasta 
from __future__ import print_function
import sys

if len(sys.argv) < 3:
    print("Usage: python %s <position> <genome>"%sys.argv[0])
    sys.exit()
    
fin_position = sys.argv[1]
fin_genome = sys.argv[2]

positions = []
for line in open(fin_position):
    positions.append(line)
    
def checkout_cgi(chrom, seq):
    for pos in positions:
        cols = pos.strip().split()
        if not cols: continue
        name  = cols[0]
        start = int(cols[1])
        end = int(cols[2])
        #print(name, start, end)
        if name  == chrom:
            print(chrom)
            print(seq[start-1:end])

chrom = ''
seq = ''
for line in open(fin_genome):
    if line[0] == '>':
        if seq != '':
            checkout_cgi(chrom, seq)
            
        chrom = line.strip()
        seq = ''
    else:
        seq += line.strip()
if seq != '':
    checkout_cgi(chrom, seq)
    

