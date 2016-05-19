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
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief Wirish SPI implementation.
 */

#include "SPI.h"
#include "timer.h"
#include "util.h"
#include "rcc.h"

#include "wirish.h"
#include "boards.h"

ATTR_UNUSED uint8_t SPCR;

struct spi_pins {
    int8_t nss;
    int8_t sck;
    int8_t miso;
    int8_t mosi;
};

static const spi_pins* dev_to_spi_pins(spi_dev *dev);

static void enable_device(spi_dev *dev,
                          bool as_master,
                          uint8_t baud,
                          spi_cfg_flag endianness,
                          spi_mode mode);

static const spi_pins board_spi_pins[] __FLASH__ =
{
    {BOARD_SPI1_NSS_PIN,
     BOARD_SPI1_SCK_PIN,
     BOARD_SPI1_MISO_PIN,
     BOARD_SPI1_MOSI_PIN},

    {BOARD_SPI3_NSS_PIN,
     BOARD_SPI3_SCK_PIN,
     BOARD_SPI3_MISO_PIN,
     BOARD_SPI3_MOSI_PIN},
};

HardwareSPI SPI(1);

/*
 * Constructor
 */

HardwareSPI::HardwareSPI(uint32_t spi_num) {
    switch (spi_num) {
    case 1:
        this->spi_d = SPI1_F2;
        break;
//    case 2:
//        this->spi_d = SPI2_F2;
//        break;
    case 3:
        this->spi_d = SPI3_F2;
        break;
    default:
        ASSERT(0);
    }
}

/*
 * Set up/tear down
 */

void HardwareSPI::begin(void)
{
  enable_device(this->spi_d, true, SPI_BAUD_PCLK_DIV_8, SPI_FRAME_MSB, (spi_mode) SPI_MODE0);
}

void HardwareSPI::reconfigure(void)
{
  uint32_t baud = _setting.baud_control;

  // SPI3 has slower speed than SPI1
  if (this->spi_d == SPI3_F2 && baud > 0)
  {
    baud -= (1<<3);
  }

  spi_reconfigure(this->spi_d, SPI_DFF_8_BIT | SPI_SW_SLAVE | SPI_SOFT_SS |
                               _setting.bitOrder | baud | SPI_CR1_MSTR | _setting.dataMode);
}

void HardwareSPI::beginTransaction(SPISettings settings)
{
  _setting.baud_control = settings.baud_control;
  _setting.bitOrder     = settings.bitOrder;
  _setting.dataMode     = settings.dataMode;

  reconfigure();
}

void HardwareSPI::setClockDivider(uint8_t clockDivider)
{
  if (_setting.baud_control != clockDivider)
  {
    _setting.baud_control = clockDivider;
    reconfigure();
  }
}

void HardwareSPI::setBitOrder(uint8_t bitOrder)
{
  bitOrder = ((bitOrder == MSBFIRST) ? SPI_FRAME_MSB : SPI_FRAME_LSB);
  if (_setting.bitOrder != bitOrder)
  {
    _setting.bitOrder = bitOrder;
    reconfigure();
  }
}

void HardwareSPI::setDataMode(uint8_t dataMode)
{
  if (_setting.dataMode != dataMode)
  {
    _setting.dataMode = dataMode;
    reconfigure();
  }
}

void HardwareSPI::endTransaction(void)
{

}

void HardwareSPI::end(void)
{
  if (!spi_is_enabled(this->spi_d)) return;

  // Follows RM0008's sequence for disabling a SPI in master/slave
  // full duplex mode.
  while (spi_is_rx_nonempty(this->spi_d)) {
    // FIXME [0.1.0] remove this once you have an interrupt based driver
    volatile uint16_t rx __attribute__((unused)) = spi_rx_reg(this->spi_d);
  }
  while (!spi_is_tx_empty(this->spi_d)) {}

  while (spi_is_busy(this->spi_d)) {}

  spi_peripheral_disable(this->spi_d);
}

uint8_t HardwareSPI::transfer(uint8_t byte)
{
  // wait until tx buffer is empty
  while ( !spi_is_tx_empty(this->spi_d) ) yield();

  // send byte
  spi_tx_reg(this->spi_d, (uint16_t) byte);

  // wait until data is received
  while ( !spi_is_rx_nonempty(this->spi_d) ) yield();

  return (uint8_t) spi_rx_reg(this->spi_d);
}

uint16_t HardwareSPI::transfer16(uint16_t data)
{
  union { uint16_t val; struct { uint8_t first; uint8_t second; }; } in, out;
  in.val = data;

  if ( !(this->spi_d->regs->CR1 & SPI_CR1_LSBFIRST) )
  {
    uint8_t temp = in.first;
    in.first = in.second;
    in.second = temp;
  }

  out.first  = transfer(in.first);
  out.second = transfer(in.second);

  if ( !(this->spi_d->regs->CR1 & SPI_CR1_LSBFIRST) )
  {
    uint8_t temp = out.first;
    out.first = out.second;
    out.second = temp;
  }

  return out.val;
}

void HardwareSPI::transfer(void *buf, size_t count)
{
  uint8_t* p_data = (uint8_t*) buf;
  while( count-- )
  {
    *p_data = transfer(*p_data);
    p_data++;
  }
}

void HardwareSPI::transfer(void const *tx_buf, void *rx_buf, size_t count)
{
  uint8_t* p_tx = (uint8_t*) tx_buf;
  uint8_t* p_rx = (uint8_t*) rx_buf;

  while( count-- )
  {
    *p_rx++ = transfer(*p_tx++);
  }
}

/*
 * Auxiliary functions
 */

static void configure_gpios(spi_dev *dev, bool as_master);

static const spi_pins* dev_to_spi_pins(spi_dev *dev) {
  switch (dev->clk_id) {
    case RCC_SPI1: return board_spi_pins;
//    case RCC_SPI2: return board_spi_pins + 1;
    case RCC_SPI3: return board_spi_pins + 2;
    default:       return NULL;
  }
}

/* Enables the device in master or slave full duplex mode.  If you
 * change this code, you must ensure that appropriate changes are made
 * to HardwareSPI::end(). */
static void enable_device(spi_dev *dev,
                          bool as_master,
                          uint8_t baud,
                          spi_cfg_flag endianness,
                          spi_mode mode) {
//    spi_baud_rate baud = determine_baud_rate(dev, freq);
    uint32_t cfg_flags = (endianness | SPI_DFF_8_BIT | SPI_SW_SLAVE |
                        (as_master ? SPI_SOFT_SS : 0));

    spi_init(dev);
    configure_gpios(dev, as_master);
    if (as_master) {
        spi_master_enable(dev, (spi_baud_rate) baud, mode, cfg_flags);
    } else {
        spi_slave_enable(dev, mode, cfg_flags);
    }
}

static void disable_pwm(const stm32_pin_info *i) {
    if (i->timer_device) {
        timer_set_mode(i->timer_device, i->timer_channel, TIMER_DISABLED);
    }
}

static void configure_gpios(spi_dev *dev, bool as_master) {
  const spi_pins *pins = dev_to_spi_pins(dev);

  if (!pins) return;

  const stm32_pin_info *nssi = (pins->nss >= 0) ? &PIN_MAP[pins->nss] : NULL;
  const stm32_pin_info *scki = &PIN_MAP[pins->sck];
  const stm32_pin_info *misoi = &PIN_MAP[pins->miso];
  const stm32_pin_info *mosii = &PIN_MAP[pins->mosi];

  if(nssi) disable_pwm(nssi);
  disable_pwm(scki);
  disable_pwm(misoi);
  disable_pwm(mosii);

  if(dev->clk_id <= RCC_SPI2) {
    if(nssi) {
      if(!as_master) {
        gpio_set_af_mode(nssi->gpio_device,  scki->gpio_bit, 5);
      }
    }
    gpio_set_af_mode(scki->gpio_device,  scki->gpio_bit, 5);
    gpio_set_af_mode(misoi->gpio_device, misoi->gpio_bit, 5);
    gpio_set_af_mode(mosii->gpio_device, mosii->gpio_bit, 5);
  } else {
    if(nssi) {
      if(!as_master) {
        gpio_set_af_mode(nssi->gpio_device,  scki->gpio_bit, 6);
      }
    }
    gpio_set_af_mode(scki->gpio_device,  scki->gpio_bit, 6);
    gpio_set_af_mode(misoi->gpio_device, misoi->gpio_bit, 6);
    gpio_set_af_mode(mosii->gpio_device, mosii->gpio_bit, 6);
  }

  if(nssi) {
    spi_gpio_cfg(as_master,
                 nssi->gpio_device,
                 nssi->gpio_bit,
                 scki->gpio_device,
                 scki->gpio_bit,
                 misoi->gpio_bit,
                 mosii->gpio_bit);
  } else {
    spi_gpio_cfg(as_master,
                 NULL,
                 -1,
                 scki->gpio_device,
                 scki->gpio_bit,
                 misoi->gpio_bit,
                 mosii->gpio_bit);
  }
}

#if 0

static const spi_baud_rate baud_rates[] __FLASH__ = {
    SPI_BAUD_PCLK_DIV_2,
    SPI_BAUD_PCLK_DIV_4,
    SPI_BAUD_PCLK_DIV_8,
    SPI_BAUD_PCLK_DIV_16,
    SPI_BAUD_PCLK_DIV_32,
    SPI_BAUD_PCLK_DIV_64,
    SPI_BAUD_PCLK_DIV_128,
    SPI_BAUD_PCLK_DIV_256,
};

/*
 * Note: This assumes you're on a LeafLabs-style board
 * (CYCLES_PER_MICROSECOND == 72, APB2 at 72MHz, APB1 at 36MHz).
 */
static spi_baud_rate determine_baud_rate(spi_dev *dev, SPIFrequency freq) {
    if (rcc_dev_clk(dev->clk_id) == RCC_APB2 && freq == SPI_234_375KHZ) {
        /* APB2 peripherals are too fast for 140.625 KHz */
        ASSERT(0);
        return (spi_baud_rate)~0;
    }
    return (rcc_dev_clk(dev->clk_id) == RCC_APB2 ?
            baud_rates[freq + 1] :
            baud_rates[freq]);
}

void HardwareSPI::beginSlave(uint32_t bitOrder, uint32_t mode) {
    if (mode >= 4) {
        ASSERT(0);
        return;
    }
    spi_cfg_flag end = bitOrder == MSBFIRST ? SPI_FRAME_MSB : SPI_FRAME_LSB;
    spi_mode m = (spi_mode)mode;
    enable_device(this->spi_d, false, (SPIFrequency)0, end, m);
}

void HardwareSPI::beginSlave(void) {
    this->beginSlave(MSBFIRST, 0);
}
#endif
