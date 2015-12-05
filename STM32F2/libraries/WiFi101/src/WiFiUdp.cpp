/*
  WiFiUdp.cpp - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <string.h>
#include "WiFi101.h"
#include "WiFiUdp.h"
#include "WiFiClient.h"
//#include "WiFiServer.h"

/* Constructor. */
WiFiUDP::WiFiUDP()
{
	_rcvSize = 0;
	_rcvPort = 0;
	_rcvIP = 0;

	_udp_handle = 0;
}

/* Start WiFiUDP socket, listening at local port PORT */
uint8_t WiFiUDP::begin(uint16_t port)
{
  uint8_t para[3];
  para[0] = WIFI_INTERFACE_STATION;
  memcpy(para+1, &port, 2);

  return ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_UDP_CREATE, sizeof(para), para, NULL, &_udp_handle);
}

int WiFiUDP::available()
{
  if (_udp_handle == 0) return 0;

  uint32_t rx_byte=0;
  FEATHERLIB->sdep_execute(SDEP_CMD_UDP_CLOSE, 4, &_udp_handle, NULL, &rx_byte);

  return rx_byte;
}

/* Release any resources being used by this WiFiUDP instance */
void WiFiUDP::stop()
{
  if (_udp_handle == 0) return;

  FEATHERLIB->sdep_execute(SDEP_CMD_UDP_CLOSE, 4, &_udp_handle, NULL, NULL);
  _udp_handle = 0;
}

int WiFiUDP::beginPacket(const char *host, uint16_t port)
{
	IPAddress ip;
	if (WiFi.hostByName(host, ip)) {
	  this->beginPacket(ip, port);
	}

	return 0;
}

int WiFiUDP::beginPacket(IPAddress ip, uint16_t port)
{
	_sndIP = (uint32_t) ip;
	_sndPort = port;
	
	return 1;
}

int WiFiUDP::endPacket()
{
  _sndIP = 0;
  _sndPort = 0;

	return 1;
}

size_t WiFiUDP::write(uint8_t byte)
{
  return write(&byte, 1);
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size)
{
  if (_udp_handle == 0 || _sndIP == 0 || _sndPort == 0) return 0;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4   , .p_value = &_udp_handle },
      { .len = 4   , .p_value = &_sndIP      },
      { .len = 2   , .p_value = &_sndPort    },
      { .len = size, .p_value = buffer       }
  };

  VERIFY(ERROR_NONE == FEATHERLIB->sdep_command(SDEP_CMD_UDP_WRITE,
                                                sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                NULL, NULL), 0);
  return size;
}

int WiFiUDP::parsePacket()
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

  VERIFY(ERROR_NONE == FEATHERLIB->sdep_command(SDEP_CMD_UDP_PACKET_INFO,
                                                sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                NULL, &response), 0);

  _rcvIP   = response.remote_ip;
  _rcvPort = response.remote_port;
  _rcvSize = response.packet_size;

  return _rcvSize;
}

int WiFiUDP::read()
{
  uint8_t b;
  return ( this->read(&b, 1) > 0 ) ? (int) b : (-1);
}

int WiFiUDP::read(unsigned char* buf, size_t size)
{
  if ( _udp_handle == 0 ) return -1;

  uint16_t size16 = (uint16_t) size;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4   , .p_value = &_udp_handle },
      { .len = 2   , .p_value = &size16      },
      { .len = 4   , .p_value = &_timeout    },
  };

  VERIFY(ERROR_NONE == FEATHERLIB->sdep_command(SDEP_CMD_UDP_READ,
                                                sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                &size16, buf), -1);

  return size16;
}

int WiFiUDP::peek()
{
  uint8_t data;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &_udp_handle },
      { .len = 4, .p_value = &_timeout    },
  };

  VERIFY(ERROR_NONE == FEATHERLIB->sdep_command(SDEP_CMD_UDP_PEEK,
                                                sizeof(para_arr)/sizeof(sdep_cmd_para_t), para_arr,
                                                NULL, &data), -1);

  return (int) data;
}

void WiFiUDP::flush()
{
  if (_udp_handle == 0) return;

  FEATHERLIB->sdep_execute(SDEP_CMD_UDP_FLUSH, 4, &_udp_handle, NULL, NULL);

//	while (available())
//		read();
}

IPAddress  WiFiUDP::remoteIP()
{
	return _rcvIP;
}

uint16_t  WiFiUDP::remotePort()
{
	return _rcvPort;
}
