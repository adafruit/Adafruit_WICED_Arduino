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
	_sndIP = ip;
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
  if (_sndIP == 0 || _sndPort == 0) return 0;

  uint8_t para2[6];
  memcpy(para2  , &_sndIP  , 4);
  memcpy(para2+4, &_sndPort, 2);

  return (ERROR_NONE == FEATHERLIB->sdep_execute_extend(SDEP_CMD_UDP_WRITE, 4, &_udp_handle, 6, para2, NULL, NULL) ) ? size : 0;
}

int WiFiUDP::parsePacket()
{
  if (_udp_handle == 0) return 0;

  struct ATTR_PACKED {
    uint32_t remote_ip;
    uint16_t remote_port;
    uint32_t packet_size;
  } response;

  if ( ERROR_NONE != FEATHERLIB->sdep_execute_extend(SDEP_CMD_UDP_PACKET_INFO, 4, &_udp_handle, 4, &_timeout, NULL, &response) ) return 0;

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

  uint16_t len = (uint16_t) size;
  uint8_t para2[6];

  memcpy(para2, &size, 2);        // uint16_t only
  memcpy(para2+2, &_timeout, 4);  // timeout

  return (ERROR_NONE == FEATHERLIB->sdep_execute_extend(SDEP_CMD_UDP_READ, 4, &_udp_handle, 6, para2, &len, buf) ) ? len : (-1);
}

int WiFiUDP::peek()
{
  uint8_t data;
  return (ERROR_NONE == FEATHERLIB->sdep_execute_extend(SDEP_CMD_UDP_PEEK, 4, &_udp_handle, 4, &_timeout, NULL, &data)) ? (int) data : EOF;
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
