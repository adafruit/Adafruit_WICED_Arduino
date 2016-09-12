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
 * @brief Generic board initialization routines.
 *
 * By default, we bring up all Maple boards to 72MHz, clocked off the
 * PLL, driven by the 8MHz external crystal. AHB and APB2 are clocked
 * at 72MHz.  APB1 is clocked at 36MHz.
 */
#include "boards.h"

#include "flash.h"
#include "rcc.h"
#include "nvic.h"
#include "systick.h"
#include "gpio.h"
#include "adc.h"
#include "timer.h"

static void setupADC(void);
static void setupTimers(void);
static void adcDefaultConfig(const adc_dev* dev);
static void timerDefaultConfig(timer_dev*);

void init(void)
{
  // ADAFRUIT: most of MCU core setup is done in featherlib
  SetupClockAdafruitFeather();
  boardInit();
  setupADC();
  setupTimers();
}

/* You could farm this out to the files in boards/ if e.g. it takes
 * too long to test on Maple Native (all those FSMC pins...). */
bool boardUsesPin(uint8 pin) {
    for (int i = 0; i < BOARD_NR_USED_PINS; i++) {
        if (pin == boardUsedPins[i]) {
            return true;
        }
    }
    return false;
}

static void setupADC() {
  setupADC_F2();
  adc_foreach(adcDefaultConfig);
}


static void setupTimers() {
  timer_foreach(timerDefaultConfig);
}

static void adcDefaultConfig(const adc_dev *dev) {
    adc_init(dev);

    adc_set_extsel(dev, ADC_SWSTART);
    adc_set_exttrig(dev, true);

    adc_enable(dev);
    adc_calibrate(dev);
    adc_set_sample_rate(dev, ADC_SMPR_55_5);
}

static void timerDefaultConfig(timer_dev *dev) {
    timer_adv_reg_map *regs = (dev->regs).adv;
    const uint16 full_overflow = 0xFFFF;
    const uint16 half_duty = 0x8FFF;

    timer_init(dev);
    timer_pause(dev);

    regs->CR1 = TIMER_CR1_ARPE;
    regs->PSC = 1;
    regs->SR = 0;
    regs->DIER = 0;
//    regs->EGR = TIMER_EGR_UG;

    switch (dev->type) {
    case TIMER_ADVANCED:
        regs->BDTR = TIMER_BDTR_MOE | TIMER_BDTR_LOCK_OFF;
        // fall-through
    case TIMER_GENERAL:
        timer_set_reload(dev, full_overflow);
        #if 0  // disable default mode is PWM !!!
        for (int channel = 1; channel <= 4; channel++) {
            timer_set_compare(dev, channel, half_duty);
            timer_oc_set_mode(dev, channel, TIMER_OC_MODE_PWM_1, TIMER_OC_PE);
        }
        #endif
        // fall-through
    case TIMER_BASIC:
        break;
    }

    // UG must be placed after reload to take affect
    regs->EGR = TIMER_EGR_UG;
    timer_resume(dev);
}
