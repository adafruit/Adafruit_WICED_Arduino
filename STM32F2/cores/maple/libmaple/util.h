/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file util.h
 * @brief Miscellaneous utility macros and procedures.
 */

#include "libmaple_types.h"

#ifndef _UTIL_H_
#define _UTIL_H_

#ifdef __cplusplus
extern "C"{
#endif


/*
 * Bit manipulation
 */

/** 1 << the bit number */
#define BIT(shift)                     (1UL << (shift))
/** Mask shifted left by 'shift' */
#define BIT_MASK_SHIFT(mask, shift)    ((mask) << (shift))
/** Bits m to n of x */
#define GET_BITS(x, m, n) ((((uint32)x) << (31 - (n))) >> ((31 - (n)) + (m)))
/** True if v is a power of two (1, 2, 4, 8, ...) */
#define IS_POWER_OF_TWO(v)  ((v) && !((v) & ((v) - 1)))

#define BIT_SET(x, n)               ( (x) | BIT(n) )
#define BIT_CLR(x, n)               ( (x) & (~BIT(n)) )
#define BIT_TEST(x, n)              ( ((x) & BIT(n)) ? true : false )


//--------------------------------------------------------------------+
// MACROS
//--------------------------------------------------------------------+
#define U16_HIGH_U8(u16)            ((uint8_t) (((u16) >> 8) & 0x00ff))
#define U16_LOW_U8(u16)             ((uint8_t) ((u16)       & 0x00ff))
#define U16_TO_U8S_BE(u16)          U16_HIGH_U8(u16), U16_LOW_U8(u16)
#define U16_TO_U8S_LE(u16)          U16_LOW_U8(u16), U16_HIGH_U8(u16)

#define U32_B1_U8(u32)              ((uint8_t) (((u32) >> 24) & 0x000000ff)) // MSB
#define U32_B2_U8(u32)              ((uint8_t) (((u32) >> 16) & 0x000000ff))
#define U32_B3_U8(u32)              ((uint8_t) (((u32) >>  8) & 0x000000ff))
#define U32_B4_U8(u32)              ((uint8_t) ((u32)         & 0x000000ff)) // LSB

#define U32_FROM_U8(b1, b2, b3, b4) ((uint32_t) (((b1) << 24) + ((b2) << 16) + ((b3) << 8) + (b4)))
#define U32_FROM_U16(high, low)     ((uint32_t) (((high) << 16) | (low)))
#define U16_FROM_U8(high, low)      ((uint32_t) (((high) << 8) | (low)))

#define U32_TO_U8S_BE(u32)          U32_B1_U8(u32), U32_B2_U8(u32), U32_B3_U8(u32), U32_B4_U8(u32)
#define U32_TO_U8S(u32)             U32_B4_U8(u32), U32_B3_U8(u32), U32_B2_U8(u32), U32_B1_U8(u32)

#define BIN8(x)                     ((uint8_t)  (0b##x))
#define BIN16(b1, b2)               ((uint16_t) (0b##b1##b2))
#define BIN32(b1, b2, b3, b4)       ((uint32_t) (0b##b1##b2##b3##b4))

#define __swap32(x)    __builtin_bswap32(x)    ///< built-in function to swap Endian of 32-bit number
#define __swap16(u16)  __builtin_bswap16(u16)  ///< built-in function to swap Endian of 16-bit number

#define memclr(buffer, size)  memset(buffer, 0, size)

/*
 * Failure routines
 */

void __error(int num);
void _fail(const char*, int, const char*);
void throb(void);

/*
 * Asserts and debug levels
 */

#define DEBUG_NONE      0
#define DEBUG_FAULT     1
#define DEBUG_ALL       2

/**
 * \def DEBUG_LEVEL
 *
 * Controls the level of assertion checking.
 *
 * The higher the debug level, the more assertions will be compiled
 * in.  This increases the amount of debugging information, but slows
 * down (and increases the size of) the binary.
 *
 * The debug levels, from lowest to highest, are DEBUG_NONE,
 * DEBUG_FAULT, and DEBUG_ALL.  The default level is DEBUG_ALL.
 */

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_ALL
#endif

#if 0
#if DEBUG_LEVEL >= DEBUG_ALL
#define ASSERT(exp)                              \
    if (exp) {                                   \
    } else {                                     \
        _fail(__FILE__, __LINE__, #exp);         \
    }
#else
#define ASSERT(exp) (void)((0))
#endif

#if DEBUG_LEVEL >= DEBUG_FAULT
#define ASSERT_FAULT(exp)                       \
    if (exp) {                                  \
    } else {                                    \
        _fail(__FILE__, __LINE__, #exp);        \
    }
#else
#define ASSERT_FAULT(exp) (void)((0))
#endif
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
