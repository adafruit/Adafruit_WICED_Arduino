/**************************************************************************/
/*!
    @file     adafruit_http.cpp
    @author   huynguyen

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

#include "adafruit_wifi.h"
#include "adafruit_http.h"

/******************************************************************************/
/*!
    @brief Instantiates a new instance of the AdafruitFeather class
*/
/******************************************************************************/
AdafruitHTTP::AdafruitHTTP()
{
  rx_callback   = NULL;
  cert_addr     = 0;
  timeout       = 1000;
  isTLSEnable   = DISABLE;
  this->reset();
}

void AdafruitHTTP::reset()
{
  http_state    = IDLE;
  http_handle   = 0;
  byteRead      = 0;
}

void AdafruitHTTP::initCACerts(const char* ca_cert)
{
  if (ca_cert == NULL)
    cert_addr = 0;
  else
    cert_addr = (uint32_t)ca_cert;
}

void AdafruitHTTP::deinitCACerts()
{
  cert_addr = 0;
}

void AdafruitHTTP::enableTLS()
{
  isTLSEnable = ENABLE;
}

void AdafruitHTTP::disableTLS()
{
  isTLSEnable = DISABLE;
}

void AdafruitHTTP::setTimeout(uint32_t ms)
{
  timeout = ms;
}

int AdafruitHTTP::sendRequest(const char* url, const char* content, uint8_t method)
{
  if (http_handle != 0) this->close();
  if (url == NULL || url == "") return ERROR_INVALIDPARAMETER;
  if (method != GET_METHOD && method != POST_METHOD) return ERROR_INVALIDPARAMETER;
  uint16_t content_len = 0;
  if (content != NULL) content_len = strlen(content);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = strlen(url), .p_value = url          },
      { .len = content_len, .p_value = content      },
      { .len = 1,           .p_value = &method      },
      { .len = 4,           .p_value = &timeout     },
      { .len = 1,           .p_value = &isTLSEnable },
      { .len = 4,           .p_value = &cert_addr   },
      { .len = 4,           .p_value = &rx_callback },
  };

  int error = FEATHERLIB->sdep_execute_n(SDEP_CMD_HTTPSENDREQUEST,
                                         sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                         para_arr, NULL, &http_handle);

  if (error == ERROR_NONE) http_state = REQUEST_SENT;
  return error;
}

long AdafruitHTTP::getPacketDataLength()
{
  if ( http_handle == 0 ) return (-1);

  if (http_state < REQUEST_SENT) return (-1);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &http_handle },
      { .len = 4, .p_value = &timeout     },
  };

  uint16_t packetDataLength;
  int error = FEATHERLIB->sdep_execute_n(SDEP_CMD_HTTPDATALENGTH,
                                         sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                         para_arr, NULL, &packetDataLength);

  if (error == ERROR_NONE)
  {
    return packetDataLength;
  }
  return (-1);
}

int AdafruitHTTP::getResponseCode()
{
  if ( http_handle == 0 ) return (-1);

  if (http_state != REQUEST_SENT) return (-1);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &http_handle },
      { .len = 4, .p_value = &timeout     },
  };

  uint16_t responseCode;
  int error = FEATHERLIB->sdep_execute_n(SDEP_CMD_HTTPRESPONSECODE,
                                         sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                         para_arr, NULL, &responseCode);

  if (error == ERROR_NONE && responseCode >= 100 && responseCode <= 599) return responseCode;
  return (-1);
}

int AdafruitHTTP::extractHeaderValue(const char* header_name, char* value)
{
  if ( http_handle == 0 ) return (-1);

  if (http_state != REQUEST_SENT) return (-1);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4,                   .p_value = &http_handle },
      { .len = 4,                   .p_value = &timeout     },
      { .len = strlen(header_name), .p_value = header_name  },
  };

  int error = FEATHERLIB->sdep_execute_n(SDEP_CMD_HTTPEXTRACTHEADER,
                                         sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                         para_arr, NULL, value);
  return error;
}

int AdafruitHTTP::endOfHeaderReached()
{
  if ( http_handle == 0 ) return (-1);

  if (http_state != REQUEST_SENT) return (-1);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &http_handle },
      { .len = 4, .p_value = &timeout     },
  };

  uint8_t result;
  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute_n(SDEP_CMD_HTTPENDOFHEADER,
                                                  sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                                  para_arr, NULL, &result), 0);

  if (result) http_state = HEADER_PASSED;

  return result;
}

int AdafruitHTTP::skipHeader()
{
  if ( http_handle == 0 ) return (-1);

  if (http_state != REQUEST_SENT) return (-1);

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &http_handle },
      { .len = 4, .p_value = &timeout     },
  };

  int error = FEATHERLIB->sdep_execute_n(SDEP_CMD_HTTPSKIPHEADER,
                                         sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                         para_arr, NULL, NULL);

  if (error == ERROR_NONE) http_state = HEADER_PASSED;

  return error;
}

int AdafruitHTTP::read()
{
  if ( http_handle == 0 ) return (-1);

  uint8_t b;
  return ( this->read(&b, 1) > 0 ) ? b : (-1);

  if (this->read(&b, 1) > 0)
  {
    if (http_state == HEADER_PASSED) byteRead++;
    return b;
  }
  else
    return (-1);
}

int AdafruitHTTP::read(uint8_t* buf, size_t size)
{
  if ( http_handle == 0 ) return (-1);

  uint16_t size16 = (uint16_t) size;

  sdep_cmd_para_t para_arr[] =
  {
      { .len = 4, .p_value = &http_handle },
      { .len = 2, .p_value = &size16      },
      { .len = 4, .p_value = &timeout     },
  };

  // TODO check case when read bytes < size
  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute_n(SDEP_CMD_TCP_READ,
                                                  sizeof(para_arr)/sizeof(sdep_cmd_para_t),
                                                  para_arr, &size16, buf), -1);
  if (http_state == HEADER_PASSED) byteRead += size;
  return size;
}

int AdafruitHTTP::available()
{
  if ( http_handle == 0 ) return 0;

  uint8_t result;
  VERIFY(ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_TCP_AVAILABLE, 4, &http_handle, NULL, &result), 0);

  return result;
}

void AdafruitHTTP::setReceivedCallback( int (*fp) (void*) )
{
  rx_callback = fp;
}

void AdafruitHTTP::close()
{
  if ( http_handle == 0 ) return;

  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_CLOSE, 4, &http_handle, NULL, NULL);
  this->reset();
}
