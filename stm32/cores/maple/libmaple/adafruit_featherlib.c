/**************************************************************************/
/*!
    @file     adafruit_featherlib.c
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

#include "adafruit_featherlib.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// INTERNAL OBJECT & FUNCTION DECLARATION
//--------------------------------------------------------------------+
extern void __attribute__((noreturn)) start_c(void);
extern void USBSerial_callback(uint32_t eid, void* p_data);

//extern void adafruit_wifi_connect_callback(void);
extern void adafruit_wifi_disconnect_callback(void);

extern err_t adafruit_tcpserver_connect_callback    (void* socket, void* p_tcpserver);
extern err_t adafruit_tcp_receive_callback    (void* socket, void* p_tcp);
extern err_t adafruit_tcp_disconnect_callback (void* socket, void* p_tcp);

extern err_t adafruit_udp_receive_callback(void* socket, void* p_udp);

void adafruit_mqtt_subscribed_callback(char* topic_data, size_t topic_len, uint8_t* mess_data, size_t len, void* callback_func, void* arg) ATTR_WEAK;
void adafruit_mqtt_disconnect_callback(void* p_mqtt) ATTR_WEAK;

//--------------------------------------------------------------------+
// IMPLEMENTATION
//--------------------------------------------------------------------+
ATTR_USED adafruit_arduino_t const adafruit_arduino =
{
    .arduino_magic       = CFG_ARDUINO_CODE_MAGIC,
    .version_str         = CFG_ARDUINO_CODE_VERSION,
    .startup             = start_c,

    .cdc_serial_event_cb = USBSerial_callback,

    // wifi
//    .wifi_connect_callback    = adafruit_wifi_connect_callback,
    .wifi_disconnect_callback   = adafruit_wifi_disconnect_callback,

    // TCP
    .tcpserver_connect_callback = adafruit_tcpserver_connect_callback,
    .tcp_receive_callback       = adafruit_tcp_receive_callback,
    .tcp_disconnect_callback    = adafruit_tcp_disconnect_callback,

    // UDP
    .udp_receive_callback       = adafruit_udp_receive_callback,

    // MQTT
    .mqtt_subscribed_callback   = adafruit_mqtt_subscribed_callback,
    .mqtt_disconnect_callback   = adafruit_mqtt_disconnect_callback,
};
