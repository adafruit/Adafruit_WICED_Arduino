/**************************************************************************/
/*!
    @file     adafruit_adc.cpp
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

#include "adafruit_adc.h"
#include "io.h"
#include "dma.h"

AdafruitADC ADC;

/* By ACTUAL testing, the small bufsize need a slower sample rate
 * to work stable, number is measured as follows (subject to change)
 * - bufsize = 128 --> max sample is 1.5
 * - bufsize = 64  --> max sample is 7.5
 * - bufsize = 32  --> max sample is 13.5
 * - bufsize = 16  --> max sample is 28.5
 * - bufsize = 8   --> max sample is 71.5
 * - bufsize = 4   --> max sample is 239.5
 * - bufsize < 4   --> not able to work
 *
 * Table to enforce the working sample rate when buffer is configured.
 * Though user can overwrite it later by setSampleRate() if needed
 */

/**
 * Constructor
 */
AdafruitADC::AdafruitADC(void)
{
  _pin[0] = 0xff;
  _pin[1] = 0xff;

  _callback = NULL;

  _buffer  = NULL;
  _bufsize = 0;

  _low_threshold = _high_threshold = 0;
}

/**
 * Begin with single analog pin
 * @param pin1
 * @return
 */
bool AdafruitADC::begin(uint8_t pin1)
{
  if (PIN_MAP[pin1].adc_channel == ADCx) return false;

  _pin[0] = pin1;

  return true;
}

#if 0
/**
 * Begin with dual analog pins
 * @param pin1
 * @param pin2
 * @return
 */
bool AdafruitADC::begin(uint8_t pin1, uint8_t pin2)
{
  if (PIN_MAP[pin1].adc_channel == ADCx || PIN_MAP[pin2].adc_channel == ADCx) return false;

  _pin[0] = pin1;
  _pin[1] = pin2;

  return true;
}
#endif

/**
 * Configure the buffer to hold samples
 * @param buffer
 * @param size
 */
void AdafruitADC::setBuffer(uint16_t* buffer, uint32_t size)
{
  _buffer   = buffer;
  _bufsize  = size;

  adc_smp_rate max_rate;

  if      ( _bufsize < 4  ) max_rate = ADC_SMPR_239_5; // wont' work though
  else if ( _bufsize < 8  ) max_rate = ADC_SMPR_239_5;
  else if ( _bufsize < 16 ) max_rate = ADC_SMPR_71_5;
  else if ( _bufsize < 32 ) max_rate = ADC_SMPR_28_5;
  else if ( _bufsize < 64 ) max_rate = ADC_SMPR_13_5;
  else if ( _bufsize < 128) max_rate = ADC_SMPR_7_5;
  else
    max_rate = ADC_SMPR_1_5;

  setSampleRate(max_rate);
}

/**
 * Attach callback which fires when buffer is full of samples
 * @param fp
 */
void AdafruitADC::attachInterrupt(void (*fp) (void) )
{
  _callback = fp;
}

void AdafruitADC::setThreshold(uint16_t low, uint16_t high)
{
  _low_threshold = low;
  _high_threshold = high;
}

bool AdafruitADC::start(uint8_t mode)
{
  adc_reg_map *regs = ADC1->regs;

  if ( mode == ADC_MODE_WATCHDOG )
  {
    regs->LTR = _low_threshold;
    regs->HTR = _high_threshold;

    regs->SR &= ~( ADC_SR_OVR | ADC_SR_EOC | ADC_SR_AWD);

    // Enable watchdog on specified channel + AWD intterupt
    regs->CR1 = (ADC_CR1_AWDEN | ADC_CR1_AWDSGL | ADC_CR1_AWDIE | PIN_MAP[_pin[0]].adc_channel);

    adc_attach_interrupt(_callback);
  }
  else
  {
    // callback & buffer are mandatory
    VERIFY(_callback != NULL);
    VERIFY( _buffer && _bufsize );

    // Setup DMA & set callback as DMA transfer complete
    // DMA2 RCC clock is already enabled by featherlib for using SDIO with radio chip

    __io uint32* adc_dr = &regs->DR;
    uint32_t dma_cr = DMA_CR_CH0 | DMA_CR_PL_HIGH | DMA_CR_DIR_P2M | DMA_CR_CIRC | DMA_CR_TCIE /*| DMA_CR_TEIE | DMA_CR_DMEIE*/;

    if (_bufsize > 1) dma_cr |= DMA_CR_MINC;

    //if ( pin_count == 1)
    {
      if ( mode == ADC_MODE_NORMAL )
      {
        dma_cr |= DMA_CR_PBURST0 | DMA_CR_PSIZE_16BITS | DMA_CR_MBURST0 | DMA_CR_MSIZE_16BITS;

        adc_dr = &regs->DR;
      }
    }

    dma_clear_isr_bits(DMA2, DMA_STREAM0);
    dma_setup_transfer(DMA2, DMA_STREAM0,
                       &regs->DR, _buffer, 0,
                       dma_cr, 0);
    dma_set_num_transfers(DMA2, DMA_STREAM0, _bufsize);
    dma_attach_interrupt(DMA2, DMA_STREAM0, _callback);
    //    dma_attach_interrupt(DMA2, DMA_STREAM0, adafruit_adc_dma_isr);
    dma_enable(DMA2, DMA_STREAM0);

    // Set up overrun interrupt, it occurred quite often with DMA + Continuos mode
    regs->CR1 = ADC_CR1_OVRIE;
    adc_attach_interrupt(adafruit_adc_overrun_isr);

    // Continuous & DMA mode
    regs->CR2 |= ADC_CR2_DMA;
  }

  // configure as input analog
  uint8_t pin_count;
  for(pin_count=0; (pin_count<2) && (_pin[pin_count] != 0xff); pin_count++)
  {
    pinMode(_pin[pin_count], INPUT_ANALOG);
  }

  regs->CR2 |= ADC_CR2_CONT;

  // Start ADC conversion
  adc_set_reg_seqlen(ADC1, 1);

  regs->SQR3 = PIN_MAP[_pin[0]].adc_channel;
  regs->CR2 |= ADC_CR2_SWSTART;

  return true;
}

void AdafruitADC::stop(void)
{
  // ADC
  adc_reg_map *regs = ADC1->regs;

  regs->CR2 &= ~(ADC_CR2_CONT | ADC_CR2_DMA);
  regs->CR1 &= ~ADC_CR1_OVRIE;

  adc_detach_interrupt();

  // DMA
  dma_disable(DMA2, DMA_STREAM0);
  dma_detach_interrupt(DMA2, DMA_STREAM0);
}

// callback isr from irq
void adafruit_adc_overrun_isr(void)
{
  adc_reg_map *regs = ADC1->regs;

  // Recover from data overrun
  if ( regs->SR  & ADC_SR_OVR )
  {
    // re-init DMA
    dma_disable(DMA2, DMA_STREAM0);
    dma_clear_isr_bits(DMA2, DMA_STREAM0);
    dma_set_m0addr(DMA2, DMA_STREAM0, ADC._buffer);
    dma_set_num_transfers(DMA2, DMA_STREAM0, ADC._bufsize);
    dma_enable(DMA2, DMA_STREAM0);

    // Clear interrupt flag
    regs->SR &= ~(ADC_SR_OVR);

    // Trigger ADC
    regs->CR2 |= ADC_CR2_SWSTART;
  }
}

#if 0
void adafruit_adc_dma_isr(void)
{
  ADC._callback();
}
#endif
