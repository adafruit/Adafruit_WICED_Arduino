/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
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
 * @file   feather.cpp
 * @brief  feather board file.
 */

#ifdef BOARD_feather

#include "feather.h"
//#include "fsmc.h"
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

#include "wirish_types.h"

void boardInit(void)
{
}

/* Port, Timer, ADC, PortBit, Timer Channel, ADC Channel */
extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] =
{
  /* PA0  */  {GPIOA, TIMER5, ADC1,  0, 1,    0},
  /* PA1  */  {GPIOA, TIMER5, ADC1,  1, 2,    1},
  /* PA2  */  {GPIOA, TIMER5, ADC1,  2, 3,    2},
  /* PA3  */  {GPIOA, TIMER5, ADC1,  3, 4,    3},
  /* PA4  */  {GPIOA,   NULL, ADC1,  4, 0,    4},
  /* PA5  */  {GPIOA,   NULL, ADC1,  5, 0,    5},

  /* PA6  */  {GPIOA,   NULL, ADC1,  6, 1,    6},  // TIMER13 CH1
  /* PA7  */  {GPIOA,   NULL, ADC1,  7, 0,    7},  // TIMER14 CH1
                                                    {GPIOA,   NULL, NULL,  8, 0, ADCx}, /* PA8 (n/a) */
  /* PA9  */  {GPIOA, TIMER1, NULL,  9, 2, ADCx},  // UART1_TXD
  /* PA10 */  {GPIOA, TIMER1, NULL, 10, 3, ADCx},  // UART1_RXD
                                                    {GPIOA,   NULL, NULL, 11, 0, ADCx}, /* PA11 (n/a) */ // remap out
                                                    {GPIOA,   NULL, NULL, 12, 0, ADCx}, /* PA12 (n/a) */
  /* PA13 */  {GPIOA,   NULL, NULL, 13, 0, ADCx}, // SWDIO
  /* PA14 */  {GPIOA,   NULL, NULL, 14, 0, ADCx}, // SWCLK
  /* PA15 */  {GPIOA, TIMER2, NULL, 15, 1, ADCx}, // LED

                                                    {GPIOB, TIMER3, ADC1,  0, 3,    8}, /* PB0 (n/a) */
                                                    {GPIOB, TIMER3, ADC1,  1, 4,    9}, /* PB1 (n/a) */
                                                    {GPIOB,   NULL, NULL,  2, 0, ADCx}, /* PB2 (n/a) */
  /* PB3  */  {GPIOB, TIMER2, NULL,  3, 2, ADCx}, // SPI3_SCK
  /* PB4  */  {GPIOB, TIMER3, NULL,  4, 1, ADCx}, // SPI3_MISO
  /* PB5  */  {GPIOB, TIMER3, NULL,  5, 2, ADCx}, // SPI3_MOSI
  /* PB6  */  {GPIOB, TIMER4, NULL,  6, 1, ADCx}, // I2C1_SCL
  /* PB7  */  {GPIOB, TIMER4, NULL,  7, 2, ADCx}, // I2C1_SDA
                                                    {GPIOB,   NULL, NULL,  8, 0, ADCx}, /* PB8  (n/a) */ // remap out
                                                    {GPIOB,   NULL, NULL,  9, 0, ADCx}, /* PB9  (n/a) */ // remap out
                                                    {GPIOB,   NULL, NULL, 10, 0, ADCx}, /* PB10 (n/a) */
                                                    {GPIOB,   NULL, NULL, 11, 0, ADCx}, /* PB11 (n/a) */
                                                    {GPIOB,   NULL, NULL, 12, 0, ADCx}, /* PB12 (n/a) */
                                                    {GPIOB,   NULL, NULL, 13, 0, ADCx}, /* PB13 (n/a) */
  /* PB14 */  {GPIOB,   NULL, NULL, 14, 0, ADCx}, // USB
  /* PB15 */  {GPIOB,   NULL, NULL, 15, 0, ADCx}, // USB

                                                    {GPIOC,   NULL, ADC1,  0, 0,   10}, /* PC0 (n/a) */
                                                    {GPIOC,   NULL, ADC1,  1, 0,   11}, /* PC1 (n/a) */
  /* PC2  */  {GPIOC,   NULL, ADC1,  2, 0,   12},
  /* PC3  */  {GPIOC,   NULL, ADC1,  3, 0,   13},
                                                    {GPIOC,   NULL, ADC1,  4, 0,   14}, /* PC4 (n/a)  */
  /* PC5  */  {GPIOC,   NULL, ADC1,  5, 0,   15},
                                                    {GPIOC, TIMER8, NULL,  6, 1, ADCx}, /* PC6 (n/a) */
  /* PC7  */  {GPIOC, TIMER8, NULL,  7, 2, ADCx},
};

extern const uint8 boardPWMPins[BOARD_NR_PWM_PINS] __FLASH__ = {
    PA0, PA1, PA2, PA3, PA9, PA10, PA15, PB3, PB4, PB5, PB6, PB7, PC7
};

ASSERT_STATIC( sizeof(boardPWMPins) == BOARD_NR_PWM_PINS );

extern const uint8 boardADCPins[BOARD_NR_ADC_PINS] __FLASH__ = {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PC2, PC3, PC5
};

ASSERT_STATIC( sizeof(boardADCPins) == BOARD_NR_ADC_PINS );

extern const uint8 boardUsedPins[BOARD_NR_USED_PINS] __FLASH__ = {
    PB14, PB15, // USB
};

#endif
