import sys 
import os 
# Write header data 
obz = []
f = open(sys.argv[1], 'rb')
obz = f.read()
f.close()
fn = os.path.basename(sys.argv[1]).split('.')[0]
f = open(fn + '.h', 'w')
f.write("// png2gbc\nconst unsigned char " + fn + "["+str(len(obz))+"] = {\n\t")
i = 0
while i < len(obz):
    f.write('0x{0:0{1}X}'.format(obz[i],2) + ', ')
    if (i+1) % 16 == 0:
        f.write('\n\t')
    i += 1
f.write('\n};\n')
f.close()
