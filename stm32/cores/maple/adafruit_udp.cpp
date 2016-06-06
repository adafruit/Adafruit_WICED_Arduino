/**************************************************************************/
/*!
    @file     adafruit_udp.cpp
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

#include "adafruit_feather.h"
#include "adafruit_udp.h"

/******************************************************************************/
/*!
    @brief Instantiates a new instance of the AdafruitUDP class
*/
/******************************************************************************/
AdafruitUDP::AdafruitUDP()
{
  this->reset();
}

/******************************************************************************/
/*!
    @brief
*/
/******************************************************************************/
void AdafruitUDP::reset(void)
{
  _udp_handle       = 0;
  _bytesRead        = 0;

  _rcvPort          = 0;
  _rcvIP            = 0;
  _sndIP            = 0;
  _sndPort          = 0;

  _packet_buffering = true;

  rx_callback       = NULL;
  _timeout          = ADAFRUIT_UDP_TIMEOUT;
}

/******************************************************************************/
/*!
    @brief Start UDP socket, listening at local port
*/
/******************************************************************************/
uint8_t AdafruitUDP::begin(uint16_t port)
{
  if ( !Feather.connected() ) return 0;

  uint8_t interface = WIFI_INTERFACE_STATION;
  uint32_t this_value = (uint32_t) this;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 1, .p_value = &interface  },
      { .len = 2, .p_value = &port       },
      { .len = 4, .p_value = &this_value },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t) - (rx_callback == NULL ? 1 : 0);

  return sdep_n(SDEP_CMD_UDP_CREATE, para_count, para_arr, NULL, &_udp_handle);
}

/******************************************************************************/
/*!
    @brief Stop UDP socket, release all resources
*/
/******************************************************************************/
void AdafruitUDP::stop()
{
  if (_udp_handle == 0) return;

  sdep(SDEP_CMD_UDP_CLOSE, 4, &_udp_handle, NULL, NULL);
  this->reset();
}

/******************************************************************************/
/*!
    @brief  It starts processing the next available incoming packet, checks for
            the presence of a UDP packet, and reports the size. must be called before
            reading the buffer with UDP.read().
*/
/******************************************************************************/
int AdafruitUDP::parsePacket()
{
  if (_udp_handle == 0) return 0;

  struct ATTR_PACKED {
    uint32_t remote_ip;
    uint16_t remote_port;
    uint32_t packet_size;
  } response;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4   , .p_value = &_udp_handle },
      { .len = 4   , .p_value = &_timeout    },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  // This function can be used for polling, TIMEOUT is a 'valid' error
  // It should not trigger error verbose and/or halted
  this->skip_next_error(ERROR_TIMEOUT);
  VERIFY_RETURN(sdep_n(SDEP_CMD_UDP_PACKET_INFO, para_count, para_arr, NULL, &response), 0);

  _rcvIP   = response.remote_ip;
  _rcvPort = response.remote_port;

  return (int) response.packet_size;
}

/******************************************************************************/
/*!
    @brief  Get remote IP
*/
/******************************************************************************/
IPAddress  AdafruitUDP::remoteIP()
{
	return _rcvIP;
}

/******************************************************************************/
/*!
    @brief  Get remote Port
*/
/******************************************************************************/
uint16_t  AdafruitUDP::remotePort()
{
	return _rcvPort;
}

/******************************************************************************/
/*!
    @brief  Reads a single byte from the response stream

    @return Returns -1 if no data was available
*/
/******************************************************************************/
int AdafruitUDP::read()
{
  uint8_t b;
  return ( this->read(&b, 1) > 0 ) ? (int) b : EOF;
}

/******************************************************************************/
/*!
    @brief  Reads the specified number of bytes, placing them in 'buf'

    @return The total number of bytes read so far
*/
/******************************************************************************/
int AdafruitUDP::read(unsigned char* buf, size_t size)
{
  if ( _udp_handle == 0 ) return 0;

  uint16_t size16 = (uint16_t) size;
  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &_udp_handle },
      { .len = 2, .p_value = &size16      },
      { .len = 4, .p_value = &_timeout     },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  uint16_t read_count=0;
  VERIFY_RETURN(sdep_n(SDEP_CMD_UDP_READ, para_count, para_arr, &read_count, buf), -1);

  _bytesRead += read_count;
  return read_count;
}

/******************************************************************************/
/*!
    @brief  Returns the first bytes without removing it from FIFO
*/
/******************************************************************************/
int AdafruitUDP::peek()
{
  if ( _udp_handle == 0 ) return EOF;

  uint8_t data;
  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &_udp_handle },
      { .len = 4, .p_value = &_timeout    },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  VERIFY_RETURN(sdep_n(SDEP_CMD_UDP_PEEK, para_count, para_arr, NULL, &data), -1);
  return (int) data;
}

/******************************************************************************/
/*!
    @brief  Returns the number of bytes available in the response FIFO
*/
/******************************************************************************/
int AdafruitUDP::available()
{
  if ( _udp_handle == 0 ) return 0;

  int32_t result = 0;
  VERIFY_RETURN(sdep(SDEP_CMD_UDP_AVAILABLE, 4, &_udp_handle, NULL, &result), 0);

  return result;
}

/******************************************************************************/
/*!
    @brief  Starts a connection to write UDP data to the remote connection
*/
/******************************************************************************/
int AdafruitUDP::beginPacket(const char *host, uint16_t port)
{
	IPAddress ip;
	if ( !Feather.hostByName(host, ip) ) return 0;

	return this->beginPacket(ip, port);
}

/******************************************************************************/
/*!
    @brief  Starts a connection to write UDP data to the remote connection
*/
/******************************************************************************/
int AdafruitUDP::beginPacket(IPAddress ip, uint16_t port)
{
	_sndIP   = (uint32_t) ip;
	_sndPort = port;

	return 1;
}

/******************************************************************************/
/*!
    @brief  Called after writing UDP data to the remote connection.
*/
/******************************************************************************/
int AdafruitUDP::endPacket()
{
  this->flush();

  _sndIP   = 0;
  _sndPort = 0;

	return 1;
}


/******************************************************************************/
/*!
    @brief  Writes a byte
*/
/******************************************************************************/
size_t AdafruitUDP::write(uint8_t byte)
{
  return write(&byte, 1);
}

/******************************************************************************/
/*!
    @brief  Writes the specified umber of bytes
*/
/******************************************************************************/
size_t AdafruitUDP::write(const uint8_t* buffer, size_t size)
{
  if (_udp_handle == 0 || _sndIP == 0 || _sndPort == 0) return 0;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4   , .p_value = &_udp_handle },
      { .len = 4   , .p_value = &_sndIP      },
      { .len = 2   , .p_value = &_sndPort    },
      { .len = size, .p_value = buffer       }
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  VERIFY_RETURN(sdep_n(SDEP_CMD_UDP_WRITE, para_count, para_arr, NULL, NULL), 0);

  // if packet is not buffered --> send out immediately
  if (!_packet_buffering) this->flush();

  return size;
}

/******************************************************************************/
/*!
    @brief  Flush
*/
/******************************************************************************/
void AdafruitUDP::flush()
{
  if (_udp_handle == 0) return;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4   , .p_value = &_udp_handle },
      { .len = 4   , .p_value = &_sndIP      },
      { .len = 2   , .p_value = &_sndPort    },
  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  sdep_n(SDEP_CMD_UDP_FLUSH, para_count, para_arr, NULL, NULL);
}

/******************************************************************************/
/*!
    @brief  Sets the data received callback for the user code
*/
/******************************************************************************/
void AdafruitUDP::setReceivedCallback( udpcallback_t fp )
{
  rx_callback = fp;
}


//--------------------------------------------------------------------+
// Callbacks
//--------------------------------------------------------------------+
/******************************************************************************/
/*!
    @brief This callback is invoked when there is data received
*/
/******************************************************************************/
err_t adafruit_udp_receive_callback(void* socket, void* p_udp)
{
  AdafruitUDP* pUDP = (AdafruitUDP*) p_udp;

  // Integrity check
  if ( *((uint32_t*) pUDP->_udp_handle) == ((uint32_t) socket) )
  {
    if (pUDP->rx_callback) pUDP->rx_callback();
  }

  return ERROR_NONE;
}
