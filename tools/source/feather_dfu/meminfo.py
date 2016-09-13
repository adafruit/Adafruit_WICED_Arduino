import usb.core
import usb.util
import usb.control
import time
from sdep import *

#***************************************************
# DEBUG (Stack + Heap + Thread) commands

def runArduinoHeapSize():
    print "Arduino    Heap  : ",
    resp = sdep.execute(SDEP_CMD_ARDUINO_HEAPSIZE)
    heap_used =  (resp[3] << 24) + (resp[2] << 16) + (resp[1] << 8) + resp[0]
    heap_total = (resp[7] << 24) + (resp[6] << 16) + (resp[5] << 8) + resp[4]

    if len(resp) > 0:
        if heap_total > 0:
            percent =  heap_used*100/heap_total
        else:
            percent = 0
        print heap_used, "/", heap_total, '(', percent, '%)',  ' free =', heap_total-heap_used

def runStackSize():
    print "Featherlib Stack : ",
    resp = sdep.execute(SDEP_CMD_STACKSIZE)

    if len(resp) > 0:
        print (resp[3] << 24) + (resp[2] << 16) + (resp[1] << 8) + resp[0],"/",
        print (resp[7] << 24) + (resp[6] << 16) + (resp[5] << 8) + resp[4]


def runHeapSize():
    print "Featherlib Heap  : ",
    resp = sdep.execute(SDEP_CMD_HEAPSIZE)
    heap_used =  (resp[3] << 24) + (resp[2] << 16) + (resp[1] << 8) + resp[0]
    heap_total = (resp[7] << 24) + (resp[6] << 16) + (resp[5] << 8) + resp[4]

    if len(resp) > 0:
        if heap_total > 0:
            percent =  heap_used*100/heap_total
        else:
            percent = 0
        print heap_used, "/", heap_total, '(', percent, '%)',  ' free =', heap_total-heap_used

def runThreadList():
    print
    resp = sdep.execute(SDEP_CMD_THREADLIST_PRINT)
    print "".join(map(chr, resp))

runArduinoHeapSize()
print
runStackSize()
runHeapSize()
runThreadList()