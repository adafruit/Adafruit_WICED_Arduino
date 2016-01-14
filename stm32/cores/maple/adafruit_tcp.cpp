/**************************************************************************/
/*!
    @file     adafruit_tcp.cpp
    @author   ktown

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

#include "adafruit_wifi.h"
#include "adafruit_tcp.h"

/******************************************************************************/
/*!
    @brief Instantiates a new instance of the AdafruitTCP class
*/
/******************************************************************************/
//AdafruitTCP::AdafruitTCP(uint16_t socket_number)
AdafruitTCP::AdafruitTCP(void)
{
  rx_callback   = NULL;
  timeout       = 1000;
//  socket        = socket_number;
  this->reset();
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
void AdafruitTCP::reset()
{
  tcp_state    = IDLE;
  tcp_handle   = 0;
  bytesRead    = 0;
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
void AdafruitTCP::setTimeout(uint32_t ms)
{
  timeout = ms;
}

bool AdafruitTCP::connect(IPAddress ip, uint16_t port)
{
  uint32_t ipv4 = (uint32_t) ip;
  uint8_t is_tls = 0;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &ipv4     },
      { .len = 2, .p_value = &port     },
      { .len = 1, .p_value = &is_tls   },
      { .len = 4, .p_value = &this->timeout },
  };

  int err = FEATHERLIB->sdep_execute_n(SDEP_CMD_TCP_CONNECT,
                                       sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                       NULL, &this->tcp_handle);
  VERIFY(err == ERROR_NONE, false);

//  this->install_callback();

  return true;
}

bool AdafruitTCP::connect(const char* host, uint16_t port)
{
  uint8_t ipv4[4];
  VERIFY(ERROR_NONE == feather.dnsLookup(host, ipv4), false);

  return this->connect( IPAddress(ipv4), port);
}

/******************************************************************************/
/*!
    @brief  Reads a single byte from the response stream

    @return Returns -1 if no data was available, or the total number of bytes
            read so far if a byte was able to be read.
*/
/******************************************************************************/
int AdafruitTCP::read()
{
  if ( tcp_handle == 0 ) return (-1);

  uint8_t b;

  if (this->read(&b, 1) > 0)
  {
    bytesRead++;
    return b;
  }
  else
    return (-1);
}

/******************************************************************************/
/*!
    @brief  Reads the specified number of bytes, placing them in 'buf'

    @return The total number of bytes read so far
*/
/******************************************************************************/
int AdafruitTCP::read(uint8_t* buf, size_t size)
{
  if ( tcp_handle == 0 ) return (-1);

  uint16_t size16 = (uint16_t) size;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &tcp_handle },
      { .len = 2, .p_value = &size16      },
      { .len = 4, .p_value = &timeout     },
  };

  // TODO check case when read bytes < size
  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute_n(SDEP_CMD_TCP_READ,
                                                  sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                                  para_arr, &size16, buf), -1);
  bytesRead += size;
  return size;
}

/******************************************************************************/
/*!
    @brief  Writes the specified bumber of bytes
*/
/******************************************************************************/
int AdafruitTCP::write(const char* content, uint16_t len)
{
  if (tcp_handle != 0) return 0; //this->close();

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4  , .p_value = &tcp_handle},
      { .len = len, .p_value = content    }
  };

  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute_n(SDEP_CMD_TCP_WRITE,
                                                sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                NULL, NULL), 0);
  this->flush();
}


void AdafruitTCP::flush()
{
  if ( tcp_handle == 0 ) return;

  // flush is flush read !!!!
  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_FLUSH, 4, &tcp_handle, NULL, NULL);

//	while (available())
//		read();
}

/******************************************************************************/
/*!
    @brief  Returns the number of bytes available in the response FIFO
*/
/******************************************************************************/
int AdafruitTCP::available()
{
  if ( tcp_handle == 0 ) return 0;

  uint8_t result;
  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_TCP_AVAILABLE, 4, &tcp_handle, NULL, &result), 0);

  return result;
}

/******************************************************************************/
/*!
    @brief  Sets the data received callback for the user code
*/
/******************************************************************************/
void AdafruitTCP::setReceivedCallback( int (*fp) (void*, void*) )
{
  rx_callback = fp;
}

/******************************************************************************/
/*!
    @brief  Closes the TCP socket and resets any counters
*/
/******************************************************************************/
void AdafruitTCP::close()
{
  if ( tcp_handle == 0 ) return;

  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_CLOSE, 4, &tcp_handle, NULL, NULL);
  this->reset();
}
