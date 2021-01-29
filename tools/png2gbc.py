import sys
import numpy
from PIL import Image, ImageDraw

# open image 
img = Image.open(sys.argv[1])
iarr = numpy.asarray(img)
#new = Image.new('L', (iarr.shape[1], iarr.shape[2]), 0)
# 8x8 pixels at a time, compress index data 4:1
obz = []
h = iarr.shape[0]
w = iarr.shape[1]
for yt in range(0, int(h / 8)):
    for xt in range(0, int(w / 8)):
        for y in range((yt * 8), (yt * 8) + 8): #single tile
            for x in range(xt * 8, (xt * 8) + 8, 8): 
                ob = 0
                for b in range (0, 4): 
                    ob += (iarr[y][x + b] << (3-b)*2)
                    #print(iarr[y][x + b], end=' ')
                obz.append(ob)
                ob2 = 0
                for b in range (4, 8): 
                    ob2 += (iarr[y][x + b] << (3-(b-4))*2)
                    #print(iarr[y][x + b], end=' ')
                obz.append(ob2)
            #print('')
        #print('')

# Write binary data
f = open(sys.argv[1].split('.')[0] + '.bin', 'wb')
i = 0
while i < len(obz):
    f.write(bytes([obz[i]]))
    i += 1
f.close()
