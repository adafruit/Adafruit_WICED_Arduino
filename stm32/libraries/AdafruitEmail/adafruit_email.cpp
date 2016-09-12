/**************************************************************************/
/*!
    @file     adafruit_email.cpp
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

#include "adafruit_email.h"

/******************************************************************************/
/**
 * Constructor
 */
/******************************************************************************/
AdafruitEmail::AdafruitEmail(void)
{
  this->reset();
}

/******************************************************************************/
/**
 * @brief Initialize Email account
 * @param email_address   Email e.g support@adafruit.com
 * @param username        normally is the same as email address
 * @param password        Email password
 */
/******************************************************************************/
bool AdafruitEmail::begin(const char* email_address, const char* username, const char* password)
{
  _email_address = email_address;
  _username      = username;
  _password      = password;

// Debuging purpose
#if defined(DEBUG_EMAIL_ADDRESS) && defined(DEBUG_EMAIL_USERNAME) && defined (DEBUG_EMAIL_PASSWORD)
  _email_address = DEBUG_EMAIL_ADDRESS;
  _username      = DEBUG_EMAIL_USERNAME;
  _password      = DEBUG_EMAIL_PASSWORD;
#endif

}

bool AdafruitEmail::stop(void)
{
  this->reset();
  return true;
}

void AdafruitEmail::reset(void)
{
  _email_address = NULL;
  _username      = NULL;
  _password      = NULL;

  _smtp_server   = NULL;
  _smtp_port     = 0;
  _smtp_tls      = false;

  _to_addrs      = NULL;
  _cc_addrs      = NULL;
  _bcc_addrs     = NULL;

  _signature     = NULL;
}

/******************************************************************************/
/**
 * Configure SMTP server
 * @param smtp_server   SMTP server e.g smtp.gmail.com
 * @param port          SMTP out port e.g 587
 * @param tls_enabled   TLS option
 * @return true if succeeded, false otherwise
 */
/******************************************************************************/
bool AdafruitEmail::outgoingServer(const char * smtp_server, uint16_t port, bool tls_enabled)
{
  _smtp_server = smtp_server;
  _smtp_port   = port;
  _smtp_tls    = tls_enabled;

  return (_smtp_server != NULL) && (_smtp_port != 0);
}


/******************************************************************************/
/**
 * Set Recipients
 * @param to_addrs    To list, comma separated.
 * @param cc_addrs    CC List, comma separated.
 * @param bcc_addrs   BCC List, comma separated.
 * @return true if succeeded, false otherwise
 */
/******************************************************************************/
bool AdafruitEmail::recipients(const char* to_addrs, const char* cc_addrs, const char* bcc_addrs)
{
  _to_addrs  = to_addrs;
  _cc_addrs  = cc_addrs;
  _bcc_addrs = bcc_addrs;

#ifdef DEBUG_EMAIL_ADDRESS
  _to_addrs  = DEBUG_EMAIL_ADDRESS;
#endif

  return (_to_addrs != NULL);
}


/******************************************************************************/
/**
 *
 * @param sign
 * @return true if succeeded, false otherwise
 */
/******************************************************************************/
bool AdafruitEmail::signature(const char* sign)
{
  _signature = sign;
  return true;
}


/******************************************************************************/
/**
 *
 * @param subject
 * @param content
 * @return true if succeeded, false otherwise
 */
/******************************************************************************/
bool AdafruitEmail::send(const char* subject, const char* content)
{
  VERIFY ( _to_addrs && _smtp_server && _smtp_port != 0 );
  VERIFY ( _email_address && _username && _password );

  // Currently verification is a must for TLS encryption
  uint8_t  tls_option = (_smtp_tls) ? TLS_VERIFICATION_REQUIRED : TLS_NO_VERIFICATION;

  sdep_cmd_para_t para_arr[] =
  {
      // Account
      { .len = strlen(_email_address) + 1 , .p_value = _email_address            },
      { .len = strlen(_username) + 1      , .p_value = _username                 },
      { .len = strlen(_password) + 1      , .p_value = _password                 },

      // Server
      { .len = strlen(_smtp_server) + 1   , .p_value = _smtp_server              },
      { .len = sizeof(_smtp_port)         , .p_value = &_smtp_port               },
      { .len = 1                          , .p_value = &_smtp_tls                },
      { .len = 1                          , .p_value = &tls_option               },

      // Recipeints Addresses (To, CC, BCC) are comma separated
      { .len = strlen(_to_addrs) + 1                    , .p_value = _to_addrs   },
      { .len = _cc_addrs  ? (strlen(_cc_addrs ) + 1) : 0, .p_value = _cc_addrs   },
      { .len = _bcc_addrs ? (strlen(_bcc_addrs) + 1) : 0, .p_value = _bcc_addrs  },

      // Mail
      { .len = strlen(subject) + 1                       , .p_value = subject    },
      { .len = strlen(content) + 1                       , .p_value = content    },
      { .len = _signature ? (strlen(_signature) + 1) : 0 , .p_value = _signature },

  };
  uint8_t para_count = sizeof(para_arr)/sizeof(sdep_cmd_para_t);

  return sdep_n(SDEP_CMD_EMAIL_SEND, para_count, para_arr, NULL, NULL);
}
