import sys

if len(sys.argv) < 3:
	print("Usage: python %s file_cpgis file_combined"%(sys.argv[0]))
	sys.exit()

file_cpgis = sys.argv[1]
file_combined = sys.argv[2]

# get combined sequence
combined_seq = ''
for line in open(file_combined):
	if line[0] != '>':
		combined_seq += line.strip()
#print("combined_sequence_length\t%s"%len(combined_seq))
#sys.exit()
# get each CpG island's sequence and get their positions
start = 0
end = 0
last_end = 0
cgi_chrom = ''
cgi_seq = ''
for line in open(file_cpgis):
	if line[0] == '>':
		if len(cgi_seq) > 0:
			start = combined_seq.find(cgi_seq, last_end) + 1
			end = start + (len(cgi_seq) - 1)
			last_end = end
			print('%s\t%s\t%s'%(cgi_chrom, start, end))
			'''
			# check 
			print('cgi_seq')
			print(cgi_seq)
			print('combined_seq')
			print(combined_seq[start-1:end])
			'''
		cgi_chrom = line.strip()
		cgi_seq = ''
	else:
		cgi_seq += line.strip()
if len(cgi_seq) > 0:
	start = combined_seq.find(cgi_seq) + 1
	end = start + (len(cgi_seq) - 1)
	print('%s\t%s\t%s'%(cgi_chrom, start, end))
	'''
	# check 
	print('cgi_seq')
	print(cgi_seq)
	print('combined_seq')
	print(combined_seq[start-1:end])
	'''
