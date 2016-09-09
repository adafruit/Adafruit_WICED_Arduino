import usb.core
import usb.util
import usb.control
import time
from sdep import *

#***************************************************
# DEBUG (Stack + Heap + Thread) commands

def runStackSize():
    print "Stack usage: ",
    resp = sdep.execute(SDEP_CMD_STACKSIZE)

    if len(resp) > 0:
        print (resp[3] << 24) + (resp[2] << 16) + (resp[1] << 8) + resp[0],"/",
        print (resp[7] << 24) + (resp[6] << 16) + (resp[5] << 8) + resp[4]


def runHeapSize():
    print "Heap usage :",
    resp = sdep.execute(SDEP_CMD_HEAPSIZE)
    heap_used =  (resp[3] << 24) + (resp[2] << 16) + (resp[1] << 8) + resp[0]
    heap_total = (resp[7] << 24) + (resp[6] << 16) + (resp[5] << 8) + resp[4]

    if len(resp) > 0:
        print heap_used, "/", heap_total, '(free ', heap_total-heap_used, ')'

def runThreadList():
    print "Thread List"
    resp = sdep.execute(SDEP_CMD_THREADLIST_PRINT)

runStackSize()
runHeapSize()
runThreadList()