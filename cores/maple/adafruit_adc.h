/**************************************************************************/
/*!
    @file     adafruit_adc.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef ADAFRUIT_ADC_H_
#define ADAFRUIT_ADC_H_

#include "adc.h"

enum
{
  ADC_MODE_NORMAL = 0   ,  // Use only 1 ADC module, if multiple pins are define, scan mode is used
  ADC_MODE_WATCHDOG     ,  // ADC value is generally not read, callback is fired when value out of threshold
  ADC_MODE_PARALLEL     ,  // All ADC pins (up to 3) are converted simultaneously
  ADC_MODE_DOUBLE_SPEED ,  // ADC got 2x speed, use 2 ADC hardwares in interleaved mode (more power)
  ADC_MODE_TRIPLE_SPEED ,  // ADC got 3x speed, use 3 ADC hardwares in interleaved mode (most power)
};

extern "C"
{
  void adafruit_adc_overrun_isr(void);
//  void adafruit_adc_dma_isr(void);
}

class AdafruitADC
{
private:
  uint8_t   _pin[2];

  void (*_callback) (void);

  uint16_t* _buffer;
  uint32_t  _bufsize;

public:
  AdafruitADC(void);

  bool begin(uint8_t pin1);
  bool begin(uint8_t pin1, uint8_t pin2);

  void setBuffer(uint16_t* buffer, uint32_t size);
  void attachInterrupt( void (*fp) (void) );

  void setThreshold();

  void setSampleRate(adc_smp_rate rate)
  {
    adc_set_sample_rate(ADC1, rate);
  }

  bool start(uint8_t mode = ADC_MODE_NORMAL);
  void stop(void);

  void resume(void)
  {
    // disable and enable DMA to continue
    ADC1->regs->CR2 &= (~ADC_CR2_DMA);
    ADC1->regs->CR2 |= ADC_CR2_DMA;
  }

  void disable(void)  { adc_disable(ADC1); }
  void enable (void)  { adc_enable(ADC1) ; }

  uint32_t value(void) { return ADC1->regs->DR; }

  friend void adafruit_adc_overrun_isr(void);
//  friend void adafruit_adc_dma_isr(void);
};

extern AdafruitADC ADC;

#endif /* ADAFRUIT_ADC_H_ */
