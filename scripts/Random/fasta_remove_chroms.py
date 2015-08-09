from __future__ import print_function
import sys

if len(sys.argv) < 3:
    print("Usage: python %s <chroms> <fasta>"%sys.argv[0])
    sys.exit()

fin_chroms = sys.argv[1]
fin_fasta = sys.argv[2]

chroms = []
for line in open(fin_chroms):
    chroms.append(line.strip())

delete = False
for line in open(fin_fasta):
    if line[0] == '>':
        #print(line[1:].strip())
        if line[1:].strip() in chroms:
            delete = True
        else:
            delete = False
    if not delete:
        print(line,end='')
