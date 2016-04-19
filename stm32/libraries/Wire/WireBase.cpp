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
 * @file WireBase.cpp
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

#include "WireBase.h"
#include "wirish.h"

void WireBase::begin(uint8_t self_addr) {
    tx_buf_overflow = false;
    rx_buf_idx = 0;
    rx_buf_len = 0;

    itc_msg_count = 0;
}

uint8_t WireBase::process_xfer(void)
{
  uint8_t retVal = this->process();

  // Check if the xfer chain has READ message, if yes reset the rx_buf
  for(uint8_t i=0; i<itc_msg_count; i++)
  {
    if (itc_msg[i].flags & I2C_MSG_READ)
    {
      rx_buf_idx = rx_buf_len = 0;
    }
  }

  for(uint8_t i=0; i<itc_msg_count; i++)
  {
    i2c_msg* p_msg = &itc_msg[i];

    // Gather Read bytes to rx_buf
    if (p_msg->flags & I2C_MSG_READ)
    {
      memcpy(rx_buf + rx_buf_len, p_msg->data, p_msg->xferred);
      rx_buf_len += p_msg->xferred;
    }

    free(p_msg->data);
    p_msg->data  = 0;
    p_msg->flags = 0;
  }

  tx_buf_overflow = false;
  itc_msg_count = 0;

  return retVal;
}

void WireBase::beginTransmission(uint8_t slave_address) {
    i2c_msg* p_msg = &itc_msg[itc_msg_count];

    p_msg->addr   = slave_address;
    p_msg->data   = (uint8_t*) malloc(WIRE_BUFSIZ);
    p_msg->length = 0;
    p_msg->flags  = 0;
}

void WireBase::beginTransmission(int slave_address) {
    beginTransmission((uint8_t)slave_address);
}

uint8_t WireBase::endTransmission(void) {
  return this->endTransmission(true);
}

uint8_t WireBase::endTransmission(bool stopBit)
{
  itc_msg_count++;

  if (stopBit)
  {
    if (tx_buf_overflow) return EDATA;

    return process_xfer();// Changed so that the return value from process is returned by this function see also the return line below
  }else
  {
    if ( itc_msg_count >= WIRE_MESS_NUM ) return EDATA;
    return SUCCESS;
  }
}

uint8_t WireBase::requestFrom(int address, int num_bytes, bool stopBit)
{
  if (num_bytes > WIRE_BUFSIZ) {
    num_bytes = WIRE_BUFSIZ;
  }
  i2c_msg* p_msg = &itc_msg[itc_msg_count];

  p_msg->addr   = address;
  p_msg->flags  = I2C_MSG_READ;
  p_msg->length = num_bytes;
  p_msg->data   = (uint8_t*) malloc(num_bytes);

  itc_msg_count++;

  if (stopBit)
  {
    process_xfer();
    return rx_buf_len;
  }else
  {
    if ( itc_msg_count >= WIRE_MESS_NUM ) return EDATA;
    return SUCCESS;
  }
}

//TODO: Add the ability to queue messages (adding a boolean to end of function
// call, allows for the Arduino style to stay while also giving the flexibility
// to bulk send
uint8_t WireBase::requestFrom(uint8_t address, int num_bytes) {
  return this->requestFrom((int)address, num_bytes, true);
}

uint8_t WireBase::requestFrom(int address, int numBytes) {
    return this->requestFrom(address, numBytes, true);
}

size_t WireBase::write(uint8_t value) {
  i2c_msg* p_msg = &itc_msg[itc_msg_count];

  if (p_msg->length == WIRE_BUFSIZ) {
    tx_buf_overflow = true;
    return 0;
  }

  p_msg->data[p_msg->length++] = value;

  return 1;
}

size_t WireBase::write(uint8_t const * buf, size_t len) {
    for (uint8_t i = 0; i < len; i++) {
        write(buf[i]);
    }
    return len;
}

int WireBase::available() {
    return rx_buf_len - rx_buf_idx;
}

int WireBase::read() {
    if (rx_buf_idx == rx_buf_len) {
        rx_buf_idx = 0;
        rx_buf_len = 0;
        return -1;
    } else if (rx_buf_idx == (rx_buf_len-1)) {
        int temp = rx_buf[rx_buf_idx];
        rx_buf_idx = 0;
        rx_buf_len = 0;
        return temp;
    }
    return rx_buf[rx_buf_idx++];
}

int WireBase::peek(void)
{
  return (rx_buf_idx == rx_buf_len) ? -1 : rx_buf[rx_buf_idx];
}

void WireBase::flush(void)
{
  // TODO to be implemented
}
