import os 
import sys
import re
from matplotlib import pyplot
from pylab import genfromtxt

if (len(sys.argv) < 2 ):
    print("Usage:> python " + sys.argv[0] + " <parallel_data_file>")
else:
    filename = sys.argv[1]
    mat = genfromtxt(filename)

    pyplot.grid()
    pyplot.title("OpenMP Basic")
    pyplot.xlabel("Number of particles")
    pyplot.ylabel("Time elapsed (s)")
    pyplot.plot(mat[:,0], mat[:,1], label="Thread 1",marker='o')
    pyplot.plot(mat[:,0], mat[:,2], label="Thread 2",marker='o')
    pyplot.plot(mat[:,0], mat[:,3], label="Thread 4",marker='o')
    pyplot.plot(mat[:,0], mat[:,4], label="Thread 8",marker='o')
    pyplot.legend(loc=0)
    pyplot.savefig("result.pdf")
