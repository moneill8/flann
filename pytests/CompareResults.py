import h5py
import numpy as np

f = h5py.File("../build/bin/kdtreeresult.hdf5", "r")
f2 = h5py.File("../build/bin/linearresult.hdf5", "r")

dset = f['result']
dset2 = f2['result']

count = 0;

for index in range(len(dset)):
	for i in range(len(dset[index])):
		for j in range(len(dset2[index])):
			if(dset[index][i] == dset2[index][j]):
				count += 1

size = len(dset)*len(dset[0])

print str(size) + "\n"

print str(count) + "\n"

accuracy = (float(count)/(float(len(dset)*len(dset[0]))))
print "Accuracy = " + str(accuracy)
