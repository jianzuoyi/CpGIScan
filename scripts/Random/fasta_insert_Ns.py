import sys

if len(sys.argv) < 2:
    print('Usage: python %s <fasta>'%sys.argv[0])
    sys.exit()
    
fin_fasta = sys.argv[1]

for line in open(fin_fasta):
    print(line)
    if line[0] == '>':
        print('N'*1000)

    