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
 *  @brief Arduino-style PWM implementation.
 */

#include "libmaple_types.h"
#include "timer.h"

#include "boards.h"
#include "pwm.h"

// debug
#include "adafruit_feather.h"

#include "wirish_math.h"

void analogWrite(uint8 pin, int val)
{
  if (pin >= BOARD_NR_GPIO_PINS) return;
  timer_dev *dev = PIN_MAP[pin].timer_device;
  if (dev == NULL || dev->type == TIMER_BASIC) return;

  // configure as PWM if not
  if ( TIMER_OC_MODE_PWM_1 != timer_oc_get_mode(dev, PIN_MAP[pin].timer_channel) )
  {
    pinMode(pin, PWM);
  }

  // map compare according to the current reload
  uint16 compare = map(val, 0, 255, 0, timer_get_reload(dev));

  timer_set_compare(dev, PIN_MAP[pin].timer_channel, compare);
}

void pwmWrite(uint8 pin, uint16 duty_cycle)
{
  if (pin >= BOARD_NR_GPIO_PINS) return;
  timer_dev *dev = PIN_MAP[pin].timer_device;
  if (dev == NULL || dev->type == TIMER_BASIC) return;

  timer_set_compare(dev, PIN_MAP[pin].timer_channel, duty_cycle);
}

// Adafruit Modification
//void pwmFrequency(uint8 pin, uint32 hz)
//{
//
//}

/**
 *
 * @param pin
 * @param us
 *
 * from HardwareTimer::setPeriod()
 */
void pwmPeriod(uint8 pin, uint32 us)
{
  if (pin >= BOARD_NR_GPIO_PINS) return;
  timer_dev *dev = PIN_MAP[pin].timer_device;
  if (dev == NULL || dev->type == TIMER_BASIC) return;

  // Use 16-bit for simple Timer management
  const uint16 full_overflow = 0xFFFF;

  uint32 cycle = us * CYCLES_PER_MICROSECOND;
  uint16 prescaler = (uint16)(cycle / full_overflow);
  uint16 reload = (uint16) round(cycle / prescaler);

  DBG_INT(prescaler);
  DBG_INT(reload);

  timer_set_prescaler(dev, prescaler);
  timer_set_reload(dev, reload);
}
