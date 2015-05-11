import subprocess
import sys
import os

alphas = [6,8]
gnn = [100,200,250,350,400]

knn = int(sys.argv[1])

output = open(sys.argv[2], 'w')

for alpha in alphas:
	for g in gnn:
		process = subprocess.Popen(['../build/bin/test_code', str(knn), str(g), str(alpha)], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
		process.wait();
		poutput = process.stdout.readline()
		while poutput != '':
			print poutput
			output.write(poutput + "\n")
