import sys
import os
from operator import itemgetter

if(len(sys.argv) != 4):
	print "need input and output file and sort index"

f = open(sys.argv[1], "r")
f2 = open(sys.argv[2], "w")

index = int(sys.argv[3])

a = []

for line in f:
	line = line.strip()
	line = line.split(",")
	l = []
	for i in range(len(line) - 1):
		l.append(int(line[i]))
	l.append(line[len(line)-1])
	a.append(l)

a = sorted(a, key=itemgetter(index))

for line in a:
	st = str(line[0])
	for i in range(len(line)):
		if i==0:
			continue
		st = st + "," + str(line[i])
	f2.write(st + "\n")
	
