import glob
import binascii

for filename in glob.glob('*.txt'):
    with open(filename, 'rb') as fin:
        data = fin.read();
        print "CRC32 of '" + filename + "' \t: " + ("%d" % (binascii.crc32(data) & 0xFFFFFFFF)) + ' len = %d' % len(data)
