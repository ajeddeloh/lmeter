from math import pi, sin

amp = 1024
offset = 2048
n_samples = 512;
data = [round(offset+(amp-1)*sin(2*pi*x/n_samples)) for x in range(n_samples)]
print(data)
