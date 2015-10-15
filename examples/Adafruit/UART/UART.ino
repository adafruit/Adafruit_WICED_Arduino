/**************************************************************************/
/*!
    @file     UART
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
#include <Arduino.h>

void setup() {
  Serial.begin (115200);  // USB monitor
  Serial1.begin(115200);  // HW UART1
  Serial2.begin(115200);  // HW UART2
  
  while (!Serial) delay(1);
  Serial.println ("UART demo: Serial Monitor");
  Serial1.println("UART demo: HWUART1");
  Serial2.println("UART demo: HWUART2");
}

void printAll(char ch)
{
  Serial.print (ch);
  Serial1.print(ch);
  Serial2.print(ch);
}

void loop() {
  char ch;
  
  // From Serial monitor to All
  if ( Serial.available() )
  {
    ch = (char) Serial.read();
    printAll(ch); 
  }
  
  // From HW UART1 to All
  if ( Serial1.available() )
  {
    ch = (char) Serial1.read();
    printAll(ch); 
  }
  
  // From HW UART2 to All
  if ( Serial2.available() )
  {
    ch = (char) Serial2.read();
    printAll(ch); 
  }

}
