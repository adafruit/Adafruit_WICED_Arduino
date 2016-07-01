/**************************************************************************/
/*!
    @file     adafruit_email.h
    @author   hathach

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

#ifndef _ADAFRUIT_EMAIL_H_
#define _ADAFRUIT_EMAIL_H_

#include <Arduino.h>
#include <Client.h>
#include <IPAddress.h>
#include <adafruit_feather.h>

class AdafruitEmail :  public AdafruitSDEP
{
protected:
  enum { EMAIL_INTERNAL_DATA_SIZE = AdafruitTCP::TCP_SOCKET_HANDLE_SIZE + 20 }; // wiced_email_account_internal

  // Account
  const char* _email_address;
  const char* _username;
  const char* _password;

  // Server
  const char* _smtp_server;
  uint16_t    _smtp_port;
  bool        _smtp_tls;

  // Recipeints Addresses (To, CC, BCC) are comma separated
  const char* _to_addrs;
  const char* _cc_addrs;
  const char* _bcc_addrs;

  const char* _signature;

  void reset(void);

public:
  AdafruitEmail(void);

  bool begin(const char* email_address, const char* username, const char* password);
  bool stop(void);

  bool outgoingServer(const char * smtp_server, uint16_t port, bool tls_enabled);
  bool recipients(const char* to_addrs, const char* cc_addrs = NULL , const char* bcc_addrs = NULL);
  bool signature(const char* sign);
  bool send(const char* subject, const char* content);


};

#endif /* _ADAFRUIT_EMAIL_H_ */
