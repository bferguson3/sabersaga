import sys 
f = open(sys.argv[1], 'rb')
inby = bytearray(f.read())
f.close()
inby[0x143] = 0xc0
f = open(sys.argv[1].split('.')[0] + '.gbc', 'wb')
f.write(inby)
f.close()