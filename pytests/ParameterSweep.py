import h5py
import numpy as np
import subprocess
import sys
import os

if len(sys.argv) != 5:
	print 'Usage: ParameterSweep.py knn input-file output-file algorithm\n'
	sys.exit()

knn = int(sys.argv[1])

fin = sys.argv[2]

output = open(sys.argv[3], 'w')

algnum = int(sys.argv[4])

executable = ''

if(algnum == 0):
	executable = './graph_test' 
elif(algnum == 1):
	executable = './kdtree_test'
else:
	print 'Not a known algorithm\n'
	sys.exit()

checks = []

if(algnum == 0):
	checks = [1,2,3,4,5,6,7,8,9,10]
elif(algnum == 1):
	checks = [10,20,40,60,80,100,120,140,160,180,200,220]

#Num trees for KD, gnn for graph
construction = []

if(algnum == 0):
	construction = [10,50,100,200,300,400,500,1000]
elif(algnum == 1):
	construction = [3,4,5,6,7,8,9,10,20]

process = subprocess.Popen(['./linear_test', str(knn), fin, 'linear_result.hdf5'], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE) 

process.wait();

lin = h5py.File("linear_result.hdf5", "r")

ldset = lin['result']


for check in checks:
	for const in construction:
		try:
			os.remove('result.hdf5')
		except:
			pass
		process = subprocess.Popen([executable, str(knn), str(const), str(check), fin, 'result.hdf5'], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr 		= subprocess.PIPE)
		process.wait();
		poutput = process.stdout.readline()
		testtime = -1
		traintime = -1
		while poutput != '':
			s = poutput.strip()
			s = s.split(',')
			if len(s) == 2:
				testtime = s[0]
				traintime = s[1]
			poutput = process.stdout.readline()
		f = h5py.File("result.hdf5", "r")
		testdset = f['result']
		count = 0;
		for index in range(len(ldset)):
			for i in range(len(ldset[index])):
				for j in range(len(testdset[index])):
					if(ldset[index][i] == testdset[index][j]):
						count += 1
		accuracy = (float(count)/(float(len(ldset)*len(ldset[0]))))
		output.write(str(testtime) + "," + str(traintime) + "," + str(accuracy) + "\n")
	print "Completed with " + str(check) + " checks"
