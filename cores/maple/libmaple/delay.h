/**
 * @file delay.h
 * @brief Delay implementation
 */

#include "libmaple_types.h"
#include "stm32.h"
#include "dwt.h"

#ifndef _DELAY_H_
#define _DELAY_H_

// To stay compatible with AVR
#define _delay_us delay_us
#define _delay_ms delay

#if 0
/**
 * @brief Delay the given number of microseconds.
 *
 * @param us Number of microseconds to delay.
 */
static inline void delay_us(uint32 us) {
    us *= STM32_DELAY_US_MULT;

    /* fudge for function call overhead  */
    //us--;
    asm volatile("   mov r0, %[us]          \n\t"
                 "1: subs r0, #1            \n\t"
                 "   bhi 1b                 \n\t"
                 :
                 : [us] "r" (us)
                 : "r0");
}
#else

// ADAFRUIT : It is monitored that function has 600ns for overhead
// we apply the correction for a exact delay
#define DELAYUS_CORRECTION  6*(F_CPU / 10000000L)

/**
 * @brief Delay the given number of microseconds using DWT.
 * DWT is already configured by featherlib
 *
 * @param us Number of microseconds to delay.
 */
static inline void delay_us(uint32 us)
{
  uint32_t current_time = DWT->CYCCNT;
  uint32_t duration     = us * (F_CPU / 1000000L) - DELAYUS_CORRECTION;

  while ( (DWT->CYCCNT - current_time) <  duration ) {}
}

#endif

static inline void delay_ns100(uint32 us) {
    us *= STM32_DELAY_US_MULT;
	us /= 10;

    /* fudge for function call overhead  */
    //us--;
    asm volatile("   mov r0, %[us]          \n\t"
                 "1: subs r0, #1            \n\t"
                 "   bhi 1b                 \n\t"
                 :
                 : [us] "r" (us)
                 : "r0");
}

#endif

