import os 
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt

mat0 = genfromtxt("resultsBasic.dat")
mat1 = genfromtxt("resultsReduce.dat")

pyplot.grid()
pyplot.title("Basic x Reduce")
pyplot.xlabel("Number of particles")
pyplot.ylabel("Time elapsed (s)")
pyplot.plot(mat0[:,0], mat0[:,1], label="Basic",marker='o')
pyplot.plot(mat1[:,0], mat1[:,1], label="Reduce",marker='o')
pyplot.legend(loc=0)
pyplot.savefig("result.pdf")
