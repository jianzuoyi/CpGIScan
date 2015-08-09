from __future__ import print_function
import sys
import random

if len(sys.argv) < 3:
    print("Usage: %s <cpgis> <segments>"%sys.argv[0])
    sys.exit()
    
fin_fasta = sys.argv[1]
fin_non_cpgi_segments = sys.argv[2]

segments = []
seq = ''
for line in open(fin_non_cpgi_segments):
    if line[0] == '>':
        if len(seq) > 0:
            segments.append(seq)
        seq = ''
    else:
        seq += line.strip()
if len(seq) != 0:
    segments.append(seq)
            
print('>shuffule_combined')
#print(segments[random.randint(0, len(segments)-1)].strip())

for line in open(fin_fasta):
    if line[0] == '>':
        print(segments[random.randint(0, len(segments)-1)].strip(), end='')
    else:
        print(line.strip(), end='')
print(segments[random.randint(0, len(segments)-1)].strip())