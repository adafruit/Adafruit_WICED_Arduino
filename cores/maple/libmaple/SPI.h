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
 * @file HardwareSPI.h
 * @brief High-level SPI interface
 *
 * This is a "bare essentials" polling driver for now.
 */

/* TODO [0.1.0] Remove deprecated methods. */

#include "libmaple_types.h"
#include "spiF2.h"
#include "boards.h"
#include "wirish.h"

#ifndef _SPI_H_
#define _SPI_H_

#define SPI_MODE0 SPI_MODE_0
#define SPI_MODE1 SPI_MODE_1
#define SPI_MODE2 SPI_MODE_2
#define SPI_MODE3 SPI_MODE_3

#define SPI_CLOCK_DIV2    SPI_BAUD_PCLK_DIV_2
#define SPI_CLOCK_DIV4    SPI_BAUD_PCLK_DIV_4
#define SPI_CLOCK_DIV8    SPI_BAUD_PCLK_DIV_8
#define SPI_CLOCK_DIV16   SPI_BAUD_PCLK_DIV_16
#define SPI_CLOCK_DIV32   SPI_BAUD_PCLK_DIV_32
#define SPI_CLOCK_DIV64   SPI_BAUD_PCLK_DIV_64
#define SPI_CLOCK_DIV128  SPI_BAUD_PCLK_DIV_128
#define SPI_CLOCK_DIV256  SPI_BAUD_PCLK_DIV_256

#define SPI_HAS_TRANSACTION 1

class SPISettings {
public:
	SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
	  init_AlwaysInline(clock, bitOrder, dataMode);
	}

	SPISettings() {
	  init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0);
	}

private:
	uint8_t baud_control;
	uint8_t bitOrder;
	uint8_t dataMode;

	void init_AlwaysInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) __attribute__((__always_inline__)) {
		this->bitOrder     = (bitOrder == MSBFIRST) ? SPI_FRAME_MSB : SPI_FRAME_LSB;
		this->dataMode     = dataMode;
		this->baud_control = SPI_BAUD_PCLK_DIV_256; // minimum as default

		for(uint8_t i=0; i < 8; i++)
		{
		  if ( ( STM32_PCLK2 / (1 << (i+1)) ) <= clock )
		  {
		    this->baud_control = (i << 3);
		    break;
		  }
		}
	}

	friend class HardwareSPI;
};


/**
 * @brief Wirish SPI interface.
 *
 * This implementation uses software slave management, so the caller
 * is responsible for controlling the slave select line.
 */
class HardwareSPI {

private:
    spi_dev *spi_d;
    int8_t _ss_pin;
    SPISettings _setting;

    void reconfigure(void);

public:
    /**
     * @param spiPortNumber Number of the SPI port to manage.
     */
    HardwareSPI(uint32_t spiPortNumber);

    /*
     * Set up/tear down
     */
    void begin(void);
    void end(void);

    void beginTransaction(SPISettings settings);
    void endTransaction(void);

    // IO
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void *buf, size_t count);
    void transfer(void const *tx_buf, void *rx_buf, size_t count);

    // Arduino compatible's stubs
    void attachInterrupt(void) { }
    void detachInterrupt(void) { }

    void setClockDivider(uint8_t clockDivider);
    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t dataMode);

#if 0
    void beginSlave(uint32_t bitOrder, uint32_t mode);
    void beginSlave(void);
#endif
};


extern HardwareSPI SPI;

// Fix an issue related to SPI class naming, defined above as `HardwareSPI`, and defined,
// by the Arduino Core as `SPIClass`. This fix resolves the issue by aliasing the two names.
// See https://forums.adafruit.com/viewtopic.php?f=24&t=161264 for more information and the
// inspiration for this bugfix. Credit to `adafruit_support_mike` who on Fri Jan 24, 2020 in
// the forum noted that the issue should be resolvable by including a #define mapping between
// the two names. By placing the define here, we obviate the need to include the define in
// other libraries or user code that depend upon `SPIClass` when building code for the WICED.
#define SPIClass HardwareSPI;

// Fix Compiler issue with sensor library
extern uint8_t SPCR;

#endif

