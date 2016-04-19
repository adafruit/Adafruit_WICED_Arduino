/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs LLC.
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
 * @file WireBase.h
 * @author Trystan Jones <crenn6977@gmail.com>
 * @brief Wire library, following the majority of the interface from Arduino.
 *        Provides a 'standard' interface to I2C (two-wire) communication for
 *        derived classes.
 */

/*
 * Library created by crenn to allow a system which would provide users the
 * 'standardised' Arduino method for interfacing with I2C devices regardless of
 * whether it is I2C hardware or emulating software.
 */

#ifndef _WIREBASE_H_
#define _WIREBASE_H_

#include "wirish.h"
#include <libmaple/i2c.h>

#define WIRE_BUFSIZ     32
#define WIRE_MESS_NUM   6

/* return codes from endTransmission() */
#define SUCCESS   0        /* transmission was successful */
#define EDATA     1        /* too much data */
#define ENACKADDR 2        /* received nack on transmit of address */
#define ENACKTRNS 3        /* received nack on transmit of data */
#define EOTHER    4        /* other error */

class WireBase : public Stream { // Abstraction is awesome!
protected:
    i2c_msg itc_msg[WIRE_MESS_NUM];
    uint8_t itc_msg_count;

    uint8_t rx_buf[WIRE_BUFSIZ];      /* receive buffer */
    uint8_t rx_buf_idx;               /* first unread idx in rx_buf */
    uint8_t rx_buf_len;               /* number of bytes read */

//    uint8_t tx_buf[WIRE_BUFSIZ];      /* transmit buffer */
//    uint8_t tx_buf_idx;  // next idx available in tx_buf, -1 overflow
    boolean tx_buf_overflow;

    // Force derived classes to define process function
    virtual uint8_t process() = 0;

    uint8_t process_xfer(void);
public:
    WireBase() {}
    ~WireBase() {}

    /*
     * Initialises the class interface
     */
    // Allow derived classes to overwrite begin function
    virtual void begin(uint8_t = 0x00);

    /*
     * Sets up the transmission message to be processed
     */
    void beginTransmission(uint8_t);

    /*
     * Allow only 8 bit addresses to be used
     */
    void beginTransmission(int);

    /*
     * Call the process function to process the message if the TX
     * buffer has not overflowed.
     */
    uint8_t endTransmission(void);

    /*
     * Call the process function to process the message if the TX
     * buffer has not overflowed.
     */
    uint8_t endTransmission(bool stopBit);

    /*
     * Request bytes from a slave device and process the request,
     * storing into the receiving buffer.
     */
    uint8_t requestFrom(uint8_t, int);

    /*
     * Allow only 8 bit addresses to be used when requesting bytes
     */
    uint8_t requestFrom(int, int);

    uint8_t requestFrom(int address, int num_bytes, bool stopBit);

    /*
     * Stack up bytes to be sent when transmitting
     */
    virtual size_t write(uint8_t);

    /*
     * Stack up bytes from the array to be sent when transmitting
     */
    virtual size_t write(const uint8_t*, size_t);

    /*
     * Return the amount of bytes that is currently in the receiving buffer
     */
    virtual int available();

    /*
     * Return the value of byte in the receiving buffer that is currently being
     * pointed to
     */
    virtual int read();
    virtual int peek(void);
    virtual void flush(void);
};

#endif // _WIREBASE_H_
