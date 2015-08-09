import sys
import shuffle_sequence as shuffle 

if len(sys.argv) < 3:
    print('Usage:python %s file_cpgis file_fasta'%(sys.argv[0]))
    sys.exit()
    
fin_cpgis = sys.argv[1]
fin_fasta = sys.argv[2]    

seq = ''
for line in open(fin_fasta):
    if line[0] != '>':
        seq += line.strip()
#print(len(seq))

start = 1
end = 1
non_cpgi_segment = ''
non_cpgi_segment_shuffled = ''
non_cpgi_segment_number = 1
for line in open(fin_cpgis):
    cols = line.strip().split()
    if not cols: continue
    #print(cols[3], cols[4])
    start = int(cols[3])
    non_cpgi_segment = seq[end-1:start-1]
    
    #print('>%s len: %s'%(non_cpgi_segment_number, len(non_cpgi_segment)))
    
    #if len(non_cpgi_segment) > 1000 and len(non_cpgi_segment) < 100000: 
        #print(non_cpgi_segment)
        #non_cpgi_segment = shuffle.ShufflePreserveSinglet(non_cpgi_segment)
    print('>non_cpgi_seg%s\n%s\n'%(non_cpgi_segment_number,non_cpgi_segment))
    
    end = int(cols[4])
    non_cpgi_segment_number += 1

non_cpgi_segment = seq[end:]
#print('>%s len: %s'%(non_cpgi_segment_number, len(non_cpgi_segment)))
#if len(non_cpgi_segment) > 1000 and len(non_cpgi_segment) < 100000:
    #non_cpgi_segment = shuffle.ShufflePreserveSinglet(non_cpgi_segment)
print('>non_cpgi_seg%s\n%s\n'%(non_cpgi_segment_number,non_cpgi_segment))