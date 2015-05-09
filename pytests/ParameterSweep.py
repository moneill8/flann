import h5py
import numpy as np
import subprocess
import sys
import os

if len(sys.argv) != 7:
	print 'Usage: ParameterSweep.py knn input-file output-file linear-output algorithm tmpfile\n'
	sys.exit()

knn = int(sys.argv[1])

fin = sys.argv[2]

output = open(sys.argv[3], 'w')

algnum = int(sys.argv[5])

tmpfile = sys.argv[6]

executable = ''

if(algnum == 0):
	executable = './graph_test' 
elif(algnum == 1):
	executable = './kdtree_test'
elif(algnum == 2):
	executable = './kmeans_test'
elif(algnum == 3):
	executable = './approx_graph_test'
else:
	print 'Not a known algorithm\n'
	sys.exit()

checks = []

#Num trees for KD, gnn for graph
construction = []
trees = []

if(algnum == 0):
	construction = [20, 25, 30, 35, 40, 45]
	#construction = [50,75,100,300,500,750,1000]
elif(algnum == 1):
	construction = [3,4,5,6,7,8,9,10,12,15,17,20,23,25]
elif(algnum == 2):
	construction = [4, 6, 8, 10,14,18,20,25,30]
	trees = [2, 4, 6, 8]
elif(algnum ==3):
	construction = [10,12,14,16,18,20]
process = subprocess.Popen(['./linear_test', str(knn), fin, sys.argv[4]], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE) 

process.wait();

lin = h5py.File(sys.argv[4], "r")

ldset = lin['result']

if(algnum < 2):
		for const in construction:
			try:
				os.remove(tmpfile)
			except:
				pass
			process = subprocess.Popen([executable, str(knn), str(const), fin,tmpfile], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
			process.wait();
			poutput = process.stdout.readline()
			first = True
			checks = 0
			tooutput = []
			while poutput != '':
				s = poutput.strip()
				if first:
					testtime = s
					first = False
				else:
					s = s.split(',')
					if len(s) == 2:
						tooutput.append([const, s[0], s[1]])	
						#print toutput[len(tooutput)-1]
						checks = checks + 1
				poutput = process.stdout.readline()
			f = h5py.File(tmpfile, "r")
			for k in range(checks):
				testdset = f[tmpfile + str(k)]
				count = 0;
				for index in range(len(ldset)):
					for i in range(len(ldset[index])):
						for j in range(len(testdset[index])):
							if(ldset[index][i] == testdset[index][j]):
								count += 1
				accuracy = (float(count)/(float(len(ldset)*len(ldset[0]))))
				strout = str(testtime) + "," + str(tooutput[k][0]) + "," + str(tooutput[k][1]) + "," + str(tooutput[k][2]) + "," + str(accuracy) + "\n"
				print strout
				output.write(strout)
elif(algnum == 2):
	for const in construction:
		for tree in trees:
			try:
				os.remove(tmpfile)
			except:
				pass
			process = subprocess.Popen([executable, str(knn), str(const), str(tree), fin, tmpfile], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
			process.wait();
			poutput = process.stdout.readline()
			first = True
			checks = 0
			tooutput = []
			while poutput != '':
				s = poutput.strip()
				if first:
					testtime = s
					first = False
				else:
					s = s.split(',')
					if len(s) == 2:
						tooutput.append([const, tree, s[0], s[1]])
						#print toutput[len(tooutput)-1]
						checks = checks + 1
				poutput = process.stdout.readline()
			f = h5py.File(tmpfile, "r")
			for k in range(checks):
				testdset = f[tmpfile + str(k)]
				count = 0;
				for index in range(len(ldset)):
					for i in range(len(ldset[index])):
						for j in range(len(testdset[index])):
							if(ldset[index][i] == testdset[index][j]):
								count += 1
				accuracy = (float(count)/(float(len(ldset)*len(ldset[0]))))
				strout = str(testtime) + "," + str(tooutput[k][0]) + "," + str(tooutput[k][1]) + "," + str(tooutput[k][2]) +"," + str(tooutput[k][3]) + "," + str(accuracy) + "\n"
				print strout
				output.write(strout)
elif(algnum == 3):
	for alpha1 in construction:
		try:
			os.remove(tmpfile)
		except:
			pass
		gnn = int(alpha1*alpha1*alpha1/2)
		print "alpha1 = " + str(alpha1) + " gnn = " + str(gnn)
		process = subprocess.Popen([executable, str(knn), str(gnn), str(alpha1), str(alpha1), str(alpha1), fin, tmpfile], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
		process.wait();
		poutput = process.stdout.readline()
		first = True
		checks = 0
		tooutput = []
		while poutput != '':
			s = poutput.strip()
			if first:
				testtime = s
				first = False
			else:
				s = s.split(',')
				if len(s) == 2:
					tooutput.append([gnn, alpha1, s[0], s[1]])
					print toutput[len(tooutput)-1]
					checks = checks + 1
			poutput = process.stdout.readline()
		f = h5py.File(tmpfile, "r")
		for k in range(checks):
			testdset = f[tmpfile + str(k)]
			count = 0;
			for index in range(len(ldset)):
				for i in range(len(ldset[index])):
					for j in range(len(testdset[index])):
						if(ldset[index][i] == testdset[index][j]):
							count += 1
			accuracy = (float(count)/(float(len(ldset)*len(ldset[0]))))
			strout = str(testtime) + "," + str(tooutput[k][0]) + "," + str(tooutput[k][1]) + "," + str(tooutput[k][2]) +"," + str(tooutput[k][3]) + "," + str(accuracy) + "\n"
			print strout
		output.write(strout)
