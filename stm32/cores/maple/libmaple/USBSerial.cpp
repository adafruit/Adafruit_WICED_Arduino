/**************************************************************************/
/*!
    @file     USBSerial.cpp
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
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

#include "libmaple.h"
#include "boards.h"
#include "adafruit_featherlib.h"
#include "USBSerial.h"

USBSerial Serial;

int USBSerial::available(void)
{
  return FEATHERLIB->file_available(FILENO_USB_SERIAL);
}

int USBSerial::peek(void)
{
  return FEATHERLIB->file_peek(FILENO_USB_SERIAL);
}

int USBSerial::read(void)
{
  char ch;
  FEATHERLIB->file_read(FILENO_USB_SERIAL, (char*)&ch, 1);
  return (int) ch;
}

// DEBUGGING with SWO
#if defined(DBG_ENABLE) && DBG_ENABLE == 3

USBSerial::operator bool()
{
  return true;
}

size_t USBSerial::write(uint8_t ch)
{
  FEATHERLIB->file_write(STDOUT_FILENO, (char*)&ch, 1);
}

size_t USBSerial::write(const uint8_t *buffer, size_t size)
{
  FEATHERLIB->file_write(STDOUT_FILENO, (char*)buffer, size);
}

#else

USBSerial::operator bool()
{
  return this->isConnected;
}

size_t USBSerial::write(uint8_t ch)
{
  return FEATHERLIB->file_write(FILENO_USB_SERIAL, (char*)&ch, 1);
}

size_t USBSerial::write(const uint8_t *buffer, size_t size)
{
  return FEATHERLIB->file_write(FILENO_USB_SERIAL, (char*)buffer, size);
}

#endif


// Callback from featherlib when there is status change
void USBSerial_callback(uint32_t eid, void* p_data)
{
  enum
  {
    HARDWARESERIAL_EVENT_DISCONNECT = 0,
    HARDWARESERIAL_EVENT_CONNECT = 1,
  };

  switch (eid)
  {
    case HARDWARESERIAL_EVENT_DISCONNECT:
    case HARDWARESERIAL_EVENT_CONNECT:
      Serial.isConnected = eid;
    break;

    default: break;
  }
}
