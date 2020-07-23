# usage:
# dump the data from gdb with
# > dump value dump.dat data
#
# then run this script to plot it
import matplotlib.pyplot as plt
from array import array
import os

f = open('dump.dat', 'rb')
d = array('h')
size = os.path.getsize('dump.dat')
d.fromfile(f, size//2)
total = d[0::2]
induc = d[1::2]
plt.plot(total)
plt.plot(induc)
plt.show()
