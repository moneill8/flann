import h5py
import numpy as np

f = h5py.File("tmp2", "r")
f2 = h5py.File("linear_result.hdf5", "r")

dset = f2['result']
for name in f:
	dset2 = f[name]

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
