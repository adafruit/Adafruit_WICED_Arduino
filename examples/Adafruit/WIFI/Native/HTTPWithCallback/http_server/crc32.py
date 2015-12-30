import sys
import binascii

def CRC32_from_file(filename):
    buf = open(filename,'rb').read()
    buf = (binascii.crc32(buf) & 0xFFFFFFFF)
    return "%08X" % buf

for eachFile in sys.argv[1:]:
    print CRC32_from_file(eachFile)