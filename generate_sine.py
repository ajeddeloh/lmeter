from math import pi, sin

# rail to rail would be 2048, be the lm234 can only do 0.5V from the bottom rail under load.
# give it some extra headroom anyway
# Increasing this should improve SNR if you have a more capable op amp like the MCP6294
amp = 1024;

# change to 128 if switching to 8 bit mode
offset = 2048

n_samples = [512, 448, 384, 320, 256]; # 10.5 to 21kHz

filename = "src/sine_data/sine%d.h"
for sample_length in n_samples:
    with open(filename % sample_length, 'w') as f:
        data = [str(round(offset+(amp)*sin(2*pi*x/sample_length))) for x in range(sample_length)]
        f.write(",".join(data))
