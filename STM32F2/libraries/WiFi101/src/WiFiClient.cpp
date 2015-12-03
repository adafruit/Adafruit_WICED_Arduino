/*
  WiFiClient.cpp - Library for Arduino Wifi shield.
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

#include "WiFi101.h"
#include "WiFiClient.h"

WiFiClient::WiFiClient()
{
  _tcp_handle        = 0;
  _packet_buffering = false;
}

#if 0
WiFiClient::WiFiClient(uint8_t sock, uint8_t parentsock)
{
	// Spawn connected TCP client from TCP server socket:
	_socket = sock;
	_flag = SOCKET_BUFFER_FLAG_CONNECTED;
	if (parentsock) {
		_flag |= (parentsock - 1) << SOCKET_BUFFER_FLAG_PARENT_SOCKET_POS;
	}
	_head = 0;
	_tail = 0;
	for (int sock = 0; sock < TCP_SOCK_MAX; sock++) {
		if (WiFi._client[sock] == this)
			WiFi._client[sock] = 0;
	}
	WiFi._client[_socket] = this;
	
	// Add socket buffer handler:
	socketBufferRegister(_socket, &_flag, &_head, &_tail, (uint8 *)_buffer);
	
	// Enable receive buffer:
	recv(_socket, _buffer, SOCKET_BUFFER_MTU, 0);

	m2m_wifi_handle_events(NULL);
}

WiFiClient::WiFiClient(const WiFiClient& other)
{
	_socket = other._socket;
	_flag = other._flag;
	_head = 0;
	_tail = 0;
	for (int sock = 0; sock < TCP_SOCK_MAX; sock++) {
		if (WiFi._client[sock] == this)
			WiFi._client[sock] = 0;
	}
	WiFi._client[_socket] = this;
	
	// Add socket buffer handler:
	socketBufferRegister(_socket, &_flag, &_head, &_tail, (uint8 *)_buffer);
	
	// Enable receive buffer:
	recv(_socket, _buffer, SOCKET_BUFFER_MTU, 0);

	m2m_wifi_handle_events(NULL);
}
#endif

void WiFiClient::usePacketBuffering(bool isEnable)
{
  _packet_buffering = isEnable;
}

int WiFiClient::connectSSL(const char* host, uint16_t port)
{
  return 0;
#if 0
  return connect(host, port, SOCKET_FLAGS_SSL);
#endif
}

int WiFiClient::connectSSL(IPAddress ip, uint16_t port)
{
  return 0;
#if 0
  return connect(ip, port, SOCKET_FLAGS_SSL, 0);
#endif
}

int WiFiClient::connect(const char* host, uint16_t port)
{
  int err;
  IPAddress ip;

  err = WiFi.hostByName(host, ip);
  VERIFY( err == 1, err);

	return this->connect(ip, port);
}

int WiFiClient::connect(IPAddress ip, uint16_t port)
{
  typedef struct ATTR_PACKED {
    uint8_t  interface;
    uint8_t  is_tls;
    uint32_t ipv4;
    uint16_t port;
    uint32_t timeout_ms;
  } sdep_tcp_connect_t;

  sdep_tcp_connect_t connect_input;
  connect_input.interface  = WIFI_INTERFACE_STATION;
  connect_input.is_tls     = false;
  connect_input.ipv4       = (uint32_t) ip;
  connect_input.port       = port;
  connect_input.timeout_ms = _timeout;

  int err = FEATHERLIB->sdep_execute(SDEP_CMD_TCP_CONNECT,
                                     sizeof(sdep_tcp_connect_t), &connect_input,
                                     NULL, &_tcp_handle);

  VERIFY( err == ERROR_NONE, -err);

  return 1;
}

size_t WiFiClient::write(uint8_t b)
{
	return write(&b, 1);
}

size_t WiFiClient::write(const uint8_t *buf, size_t size)
{
  if ( _tcp_handle == 0 ) return 0;

  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute_extend(SDEP_CMD_TCP_WRITE, 4, &_tcp_handle, size, buf, NULL, NULL), 0);

  // if packet is not buffered --> send out immediately
  if (!_packet_buffering) this->flush();

  return size;
}

int WiFiClient::available()
{
  if ( _tcp_handle == 0 ) return 0;

  uint8_t result;
  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_TCP_AVAILABLE, 4, &_tcp_handle, NULL, &result), 0);

  return result;
}

int WiFiClient::read()
{
  uint8_t b;
  return ( this->read(&b, 1) > 0 ) ? b : (-1);
}

int WiFiClient::read(uint8_t* buf, size_t size)
{
  if ( _tcp_handle == 0 ) return -1;

  uint8_t para2[6];

  memcpy(para2, &size, 2); // uint16_t only
  memcpy(para2+2, &_timeout, 4); // timeout

  if( ERROR_NONE != FEATHERLIB->sdep_execute_extend(SDEP_CMD_TCP_READ, 4, &_tcp_handle, 6, para2, NULL, buf) )
  {
    return -1;
  }

  return size;
}

int WiFiClient::peek()
{
  if ( _tcp_handle == 0 ) return EOF;

  uint8_t ch;
  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute_extend(SDEP_CMD_TCP_PEEK, 4, &_tcp_handle, 4, &_timeout, NULL, &ch), EOF);

  return (int) ch;
}

void WiFiClient::flush()
{
  if ( _tcp_handle == 0 ) return;

  // flush is flush read !!!!
  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_FLUSH, 4, &_tcp_handle, NULL, NULL);

//	while (available())
//		read();
}

void WiFiClient::stop()
{
  if ( _tcp_handle == 0 ) return;

  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_CLOSE, 4, &_tcp_handle, NULL, NULL);
  _tcp_handle = 0;
}

uint8_t WiFiClient::connected()
{
  if ( _tcp_handle == 0 ) return 0;

  uint8_t s = status();
  return !(s == TCP_STATUS_LISTEN_STATE || s == TCP_STATUS_CLOSED ||
           s == TCP_STATUS_FIN_WAIT_1 || s == TCP_STATUS_FIN_WAIT_2 ||
           (s == TCP_STATUS_CLOSED && !available()) );
}

uint8_t WiFiClient::status()
{
  if ( _tcp_handle == 0 ) return TCP_STATUS_CLOSED;

  uint8_t status = TCP_STATUS_CLOSED;
  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_STATUS, 4, &_tcp_handle, NULL, &status);

  return status;
}

WiFiClient::operator bool()
{
	return _tcp_handle != 0;
}
