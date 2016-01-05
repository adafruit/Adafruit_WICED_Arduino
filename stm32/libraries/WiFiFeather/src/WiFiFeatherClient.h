/*
  WiFiFeatherClient.h - Library for Arduino Wifi shield.
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

#ifndef WIFIFEATHERCLIENT_H
#define WIFIFEATHERCLIENT_H

#include <Arduino.h>
#include <Client.h>
#include <IPAddress.h>

enum
{
  TCP_STATUS_CLOSED           = 1,               /* Connection is closed state   */
  TCP_STATUS_LISTEN_STATE     = 2,               /* Server listen state          */
  TCP_STATUS_SYN_SENT         = 3,               /* SYN sent state               */
  TCP_STATUS_SYN_RECEIVED     = 4,               /* SYN received state           */
  TCP_STATUS_ESTABLISHED      = 5,               /* Connection established state */
  TCP_STATUS_CLOSE_WAIT       = 6,               /* Close Wait state             */
  TCP_STATUS_FIN_WAIT_1       = 7,               /* Finished Wait 1 state        */
  TCP_STATUS_FIN_WAIT_2       = 8,               /* Finished Wait 2 state        */
  TCP_STATUS_CLOSING          = 9,               /* Closing state                */
  TCP_STATUS_TIMED_WAIT       = 10,              /* Timed wait state             */
  TCP_STATUS_LAST_ACK         = 11,              /* Last ACK state               */
};

class WiFiClient : public Client {

public:
	WiFiClient();
//	WiFiClient(uint8_t sock, uint8_t parentsock = 0);
//	WiFiClient(const WiFiClient& other);

	uint8_t status();
	void usePacketBuffering(bool isEnable);

	int connectSSL(IPAddress ip, uint16_t port, char const* common_name = NULL);
	int connectSSL(const char* host, uint16_t port, char const* common_name = NULL);

	virtual int connect(IPAddress ip, uint16_t port);
	virtual int connect(const char* host, uint16_t port);

	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buf, size_t size);

	using Print::write;

	virtual int available();
	virtual int read();
	virtual int read(uint8_t *buf, size_t size);
	virtual int peek();
	virtual void flush();
	virtual void stop();
	virtual uint8_t connected();
	virtual operator bool();

	// callback
	void setReceiveCallback( int (*fp) (void*, void*) );
	void setDisconnectCallback( int (*fp) (void*, void*));

protected:
	uint32_t _tcp_handle;

	// buffer written until network packet is full ~1500 or flush() is called
	// default is false
	bool     _packet_buffering;

	int (*_disconnect_callback) (void *, void*);
	int (*_rx_callback) (void *, void*);

	void install_callback(void);
};

#endif /* WIFIFEATHERCLIENT_H */
