import sys

if len(sys.argv) < 2:
	print("Usage: python %s <cpgis_fasta>"%sys.argv[0])
	sys.exit()

fin_cpgis = sys.argv[1]
cpgi_number = 0
def count_cpgi_parameters(chrom, seq):
	global cpgi_number
	cpgi_number += 1
	C = 0
	G = 0
	CpG = 0
	GCC = 0.0
	OE = 0.0
	CpG_density = 0.0
	seq_len = len(seq);
	for i in xrange(0, seq_len):
		# count C G
		if seq[i] == 'C' or seq[i] == 'c':
			C += 1
		elif seq[i] == 'G' or seq[i] == 'g':
			G += 1
		# cout CpG
		if i < seq_len-1:
			if ((seq[i] == 'C' or seq[i] == 'c') and (seq[i+1] == 'G' or seq[i+1] == 'g')):
				CpG += 1
	#print(C, G, CpG)
	# calculate GCC OE
	if C > 0 and G > 0:
		GCC = ((G + C) * 1000.0 / seq_len) / 10
		OE = (CpG * seq_len) * 1000.0 / (C * G) / 1000.0;
		#print(GCC, OE)
	# calculate CpG density
	CpG_density = (CpG * 1.0 / seq_len) * 1000.0 / 1000.0;


	# print results
	print("%s\t%s\t%s\t%.5s\t%.5s\t%.5s"%(chrom, cpgi_number, seq_len, GCC, OE, CpG_density))
def main():
	# read all sequence
	chrom = ''
	seq = ''
	for line in open(fin_cpgis):
	    if line[0] == '>':
	        if seq != '':
	            count_cpgi_parameters(chrom, seq)
	            
	        chrom = line.strip()
	        seq = ''
	    else:
	        seq += line.strip()
	if seq != '':
	    count_cpgi_parameters(chrom, seq)
if __name__ == '__main__':
	main()

