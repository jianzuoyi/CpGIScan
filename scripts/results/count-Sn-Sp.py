import sys
import math

if len(sys.argv) < 5:
	print("Usage: python %s <total_NTs> <P> <P_prediction> <TP>"%sys.argv[0])
	sys.exit()

fin_total_NTs = sys.argv[1]
fin_P = sys.argv[2]
fin_P_prediction = sys.argv[3]
fin_TP = sys.argv[4]
total_NTs = []
P = []
P_prediction = []
TP = []

for line in open(fin_total_NTs):
	cols = line.strip().split()
	if not cols:
		print("error:split column")
	total_NTs.append(int(cols[3]))
#print(total_NTs)
for line in open(fin_P):
	cols = line.strip().split()
	if not cols:
		print("error:split column")
	P.append(int(cols[2]))
#print(P)
for line in open(fin_P_prediction):
	cols = line.strip().split()
	if not cols:
		print("error:split column")
	P_prediction.append(int(cols[2]))
#print(P_prediction)
for line in open(fin_TP):
	cols = line.strip().split()
	if not cols:
		print("error:split column")
	TP.append(int(cols[2]))
#print(TP)

def count(total_NTs, P, P_prediction, TP):
	FP = P_prediction - TP
	FN = P - TP
	TN = total_NTs - TP - FP - FN

	Sn = TP*1.0 / (TP+FN)
	Sp = TN*1.0 / (TN+FP)
	PPV= TP*1.0 / (TP+FP)
	PC = TP*1.0 / (TP+FN+FP)
	CC = (TP * TN - FN * FP)*1.0 / math.sqrt((TP+FN)*(TN+FP)*(TP+FP)*(TN+FN))
	'''
	print('%s\tP=%s\tN=%s'%(total_NTs, P, total_NTs-P))
	print("P'=%s\tTP=%s\tFP=%s"%(P_prediction,TP, FP))
	print("N'=%s\tFN=%s\tTN=%s"%(total_NTs-P_prediction,FN, TN))
	'''
	#print('Sn %s\tSp %s\tPPV %s\tPC %s\tCC %s'%(Sn, Sp, PPV, PC, CC))
	print('%s\t%s\t%s\t%s\t%s'%(Sn, Sp, PPV, PC, CC))
def main():
	print('Sn\tSp\tPPV\tPC\tCC')
	for i in xrange(0, len(total_NTs)):
		count(total_NTs[i], P[i], P_prediction[i], TP[i])

if __name__ == '__main__':
	main()