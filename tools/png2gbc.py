import sys
import os 
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
                bina_o = '' # high byte first 
                binb_o = ''
                for b in range(0, 8):
                    bina = bin(iarr[y][x + b])[2:]
                    if(len(bina) == 1):
                        bina = '0' + bina
                    bina_o += bina[0]
                    bina = bin(iarr[y][x + b])[2:]
                    if(len(bina) == 1):
                        bina = '0' + bina
                    binb_o += bina[1]
                bina_o = int(bina_o, 2)
                binb_o = int(binb_o, 2)
                obz.append(bina_o)
                obz.append(binb_o)
            #print('')   
        #print('')
print(obz)
# Write binary data
f = open('.\/res\/' + os.path.splitext(os.path.basename(sys.argv[1]))[0] + '.bin', 'wb')
i = 0
while i < len(obz):
    f.write(bytes([obz[i]]))
    i += 1
f.close()
