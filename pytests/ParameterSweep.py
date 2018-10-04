import h5py
import numpy as np
import subprocess
import sys
import os
from sets import Set

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
	construction = [25,35,50]
	#construction = [100,200]
elif(algnum == 1):
	construction = [1,2,3,4,5,6,7,10,15,20]
elif(algnum == 2):
	construction = [4, 6, 8, 10,14,18,20,25,30]
	trees = [2, 4, 6, 8]
elif(algnum ==3):
	construction = [6,8]
	trees = [10,15]

#try:
#	os.remove(sys.argv[4])
#except:
#	pass

tree = [50,100,200,500,750]
construction = [7,9]
process = subprocess.Popen(['./linear_test', str(knn), fin, sys.argv[4]], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE) 

process.wait();

lin = h5py.File(sys.argv[4], "r")

ldset = lin['result']

if(algnum == 0):
	#exact graph
	for const in construction:
		try:
			os.remove(tmpfile)
		except:
			pass
		process = subprocess.Popen([executable, str(knn), str(const), fin,tmpfile], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
		process.wait();
		poutput = process.stdout.readline()
		first = True
		tooutput = []
		ts = []
		es = []
		while poutput != '':
			s = poutput.strip()
			if first:
				testtime = s
				first = False
			else:
				s = s.split(',')
				if len(s) == 3:
					tooutput.append([const, s[2]])
					ts.append(s[0])
					es.append(s[1])	
					#print toutput[len(tooutput)-1]
			poutput = process.stdout.readline()
		f = h5py.File(tmpfile, "r")
		for l in range(len(es)):
			testdset = f[tmpfile + str(ts[l]) + "_" + str(es[l])]
			count = 0;
			size = 0
			for index in range(len(ldset)):
				myset = Set()
				for i in range(knn):
					myset.add(ldset[index][i])
				size = size + knn
				for i in range(knn):
					if(testdset[index][i] in myset):
						count += 1
			accuracy = (float(count)/(float(size)))
			strout = str(testtime)  + "," + str(tooutput[l][0]) + "," + str(tooutput[l][1]) + "," + str(ts[l]) + "," + str(es[l])+ "," + str(accuracy) + "\n"
			print strout
			output.write(strout)
elif(algnum == 1):
	#rand kd trees
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
			size = 0
			for index in range(len(ldset)):
				myset = Set()
				for i in range(len(ldset[index])):
					myset.add(ldset[index][i])
				size = size + len(ldset[index])
				for i in range(len(ldset[index])):
					if(testdset[index][i] in myset):
						count += 1
			accuracy = (float(count)/(float(size)))
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
				count = 0
				size = 0
				for index in range(len(ldset)):
					myset = Set()
					for i in range(len(ldset[index])):
						myset.add(ldset[index][i])
					size = size + len(myset)
					for i in range(len(testdset[index])):
						if(testdset[index][i] in myset):
							count += 1
				accuracy = (float(count)/(float(size)))
				strout = str(testtime) + "," + str(tooutput[k][0]) + "," + str(tooutput[k][1]) + "," + str(tooutput[k][2]) +"," + str(tooutput[k][3]) + "," + str(accuracy) + "\n"
				print strout
				output.write(strout)
elif(algnum == 3):
	#exact graph
	for const in construction:
		for gnn in trees:
			try:
				os.remove(tmpfile)
			except:
				pass
			print str(executable) + " " + str(knn) + " " + str(gnn) + " " + str(const) + " " + str(const) + " "+ str(const) + " " + str(fin) + " " + str(tmpfile)
			process = subprocess.Popen([executable, str(knn), str(gnn), str(const), str(const), str(const), fin,tmpfile], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
			process.wait();
			poutput = process.stdout.readline()
			first = True
			tooutput = []
			ts = []
			es = []
			while poutput != '':
				s = poutput.strip()
				print s
				if first:
					testtime = s
					first = False
				else:
					s = s.split(',')
					if len(s) == 3:
						tooutput.append([const, s[2]])
						ts.append(s[0])
						es.append(s[1])	
						#print toutput[len(tooutput)-1]
				poutput = process.stdout.readline()
			f = h5py.File(tmpfile, "r")
			l = 0
			for name in f:
				testdset = f[name]
				count = 0
				for index in range(len(ldset)):
					for i in range(knn):
						for j in range(knn):
							if(testdset[index][j] == ldset[index][i]):
								count += 1
				accuracy = (float(count)/(float(len(ldset)*knn)))
				print accuracy
				strout = str(testtime)  + "," + str(tooutput[l][0]) + "," + str(tooutput[l][1]) + "," + str(ts[l]) + "," + str(es[l])+ "," + str(accuracy) + "\n"
				print strout
				output.write(strout)
				l = l + 1


try:
	os.remove(tmpfile)
except:
	pass

try:
	os.remove(sys.argv[4])
except:
	pass
