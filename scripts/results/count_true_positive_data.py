import sys

if len(sys.argv) < 3:
	print("Usage: python %s experiment_position prediction_position"%sys.argv[0])
	sys.exit()

file_experiment_position = sys.argv[1]
file_predition_position = sys.argv[2]

# CpG islands' position identified by experiment
cgis_experiment = []
for line in open(file_experiment_position):
	cols = line.strip().split()
	if not cols: continue
	start = int(cols[1])
	end = int(cols[2])
	cgis_experiment.append((start, end))
#print(cgis_experiment)

# CpG islands' position identified by cpgiscan 
true_positive_position = 0
for line in open(file_predition_position): 
	cols = line.strip().split()
	if not cols: continue
	start2 = int(cols[1])
	end2 = int(cols[2])
	#print(start2, end2)
	# count true positive position
	# first exclude no overlap : end2 < start or start2 > end -- > exlucding
	# second overlaped : end2 >= start or start2 <= end
	for start, end in cgis_experiment:
		#print(start, end)
		'''
				|----------|	P
				|----------|	P'

				|----------|	p'
			|--------------------|	P

			|--------------------|	P
		|-----------|	P'
		'''
		if end2 >= start and end2 <= end:
			if start2 >= start:	
				true_positive_position += (end2 - start2 + 1)	
				#print('1', end2 - start2 + 1)
				#print(start, end)
				#print(start2, end2)		
			else:
				true_positive_position += (end2 - start + 1)
				#print('2', end2 - start + 1)
			'''
				|----------|	p
			|--------------------|	P'

					|--------------------|	P'
				|-----------|	P
			'''
		elif end >= start2 and end <= end2:
			if start >= start2:
				true_positive_position += (end - start + 1)
				#print('3', end - start + 1)
			else:
				true_positive_position += (end - start2 + 1)
				#print('4', end - start2 + 1)
print('true_positive_position\t%s'%true_positive_position)


