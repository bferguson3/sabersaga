
# Write header data 
obz = []
f = open(sys.argv[1].split('.')[0] + '.bin', 'rb')
obz = f.read()
f.close()

f = open(sys.argv[1].split('.')[0] + '.h', 'w')
f.write("// png2gbc\nconst unsigned char " + sys.argv[1].split('.')[0] + "["+str(len(obz))+"] = {\n\t")
i = 0
while i < len(obz):
    f.write('0x{0:0{1}X}'.format(obz[i],2) + ', ')
    if (i+1) % 16 == 0:
        f.write('\n\t')
    i += 1
f.write('\n};\n')
f.close()
