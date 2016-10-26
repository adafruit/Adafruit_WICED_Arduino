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

#define TIMER_MAX_RELOAD    0xFFFF

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

  if (val > 255) val = 255;

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

// Adafruit Modification to change PWM Period/Frequency
void pwmPeriod(uint8 pin, uint32 us)
{
  if (pin >= BOARD_NR_GPIO_PINS) return;
  timer_dev *dev = PIN_MAP[pin].timer_device;
  if (dev == NULL || dev->type == TIMER_BASIC) return;

  // Get timer's max speed in hz
  uint32 max_speed = rcc_dev_timer_clk_speed(dev->clk_id);

  // period in cpu cycles
  uint32 cycle = us * (max_speed / 1000000UL);

  uint16 prescaler = (uint16) (cycle / TIMER_MAX_RELOAD);
  uint16 reload    = (uint16) round(cycle / (prescaler+1));

  // Re-map compare to preserve duty cycle
  uint16 compare   =  timer_get_compare(dev, PIN_MAP[pin].timer_channel);

  compare = map(compare, 0, timer_get_reload(dev), 0, reload);

  timer_set_prescaler(dev, prescaler);
  timer_set_reload(dev, reload);
  timer_set_compare(dev, PIN_MAP[pin].timer_channel, compare);
}
