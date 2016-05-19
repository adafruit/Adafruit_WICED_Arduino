#ifndef Arduino_h
#define Arduino_h
#include "WProgram.h"
#include "WCharacter.h"
#endif

#include "variant.h"

#define clockCyclesPerMicrosecond()  CYCLES_PER_MICROSECOND
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
