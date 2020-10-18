# 0.6.7

- Fixes an issue related to the SPI class name, which prevents the compilation of WICED targeted user code that requires SPI support via `SPIClass`. Under WICED, the SPI class is named as `HardwareSPI` in `cores/maple/libmaple/SPI.h` in contrast to the Arduino Core, which as noted, defines the class as `SPIClass`. It is the latter that is most commonly referenced by many other libraries. By providing an alias between the `HardwareSPI` and `SPIClass` names via a `#define`, any previously encountered `'SPIClass' has not been declared` compilation errors should be resolved.

# 0.6.6

- Fix issue uploading with windows 10 by using good-old feather_dfu

# 0.6.5

## Arduino

- Add profileCount() to get the number of profiles
- fix #66 rename errno() to errnum()
- add setNtpServer()
- Close #69 Write feather_dfu using C + libusb instead of using python due to version conflict/confusion
- SoftAP
	- Add IP Address to join/leave callback. Signature changed from (const uint8_t mac[6]) --> (const uint8_t mac[6], uint32_t ipv4)
	- Add clientIP(id) to FeatherAP
- HTTP Server
	- Add connect/disconnect callback support
	- Fix bug when device left our SoftAP without cleaning up opened sockets
	- Add clientNum() to get active client number
- Remove Feather Lib (Debug) Section

## Featherlib

- Fix #71 incorrect sub-second ISO8601 timestamp
- Fix ntpserver bug
- Fix wifi disconnect to properly clean up system state 

# 0.6.2

- Fix Serial incorrect Read() behavior
- PWM enhancement
	- Make analogWrite() compatible with existing Arduino core (duty cycle 0-255)
	- add pwmPeriod(pin, us) pwmFrequency(pin, hz) to change the PWM period/frequency
	- add Dimmer, pwmPeriod, pwmRGB example sketches
- Add WiFi country get/set support getWifiCountry(), setWifiCountry()
	- setWifiCountry() will save the country code to NVM section which will last through power lost or reset
- Add AdafruitADC (ADC) to support highspeed sampling (up to 2MSPS) and analog watchdog

# 0.6.1

- fix #46 : delayMicroseconds() and micros() are not correct
- add #60 pulseIn() function
- fix #61 : mqtt publish without requiring to subcribe
- resolve #62 : make sure while(!Serial) have delay() and add comment to explain why
- add flow-control for USB Serial
- fix HTTPServer example issue due to modern browse's parallel loading
- fix accessing failed malloc pointer
- solve #59 worst-case scenario for mqtt keepalive, causing AWS disconnection
- solve #59 AWS reconnection issue
- add MQTT setBufferSize() to explicitly change the TX/RX for mqtt client. Default is 256 bytes each

# 0.6.0

- Increase Arduino code size from 128KB to 256KB
- Increase Arduino SRAM from 16KB to 20KB
- rename menu Featherlib (Beta) --> Featherlib (Debug)
- print warning message when Featherlib & Arduino library version are not matched
- fix get TCP peer information when SOCKET is CLOSED
- Move memory allocation for UDP from featherlib to arduino code
- change sdep parameter length from uint16_t to uint32_t
- Debug Feather
	- Retrieve Arduino Heap usage: dbgHeapTotal(), dbgHeapUsed(), dbgHeapFree()
	- Retrieve Featherlib Heap usage: dbgFeatherlibHeapTotal(), dbgFeatherlibHeapFree(), dbgFeatherlibHeapUsed()
	- add printThreadlist() and dbgThreadInfo()
- add AdafruitBase64
- add AdafruitUrlencode
- move AdafruitCrc32 to AdafruitCrypto
- AdafruitTCP : automatically free up resource when disconnected even user does not register disconnect callback
- AdafruitHTTP
	- add get() with keyvalues data support
	- move base64Decode to its own class in AdafruitCrypto
	- move urlencode to its own class
	- add response processing function
		- respParseHeader
		- respStatus
		- respContentLength
- AdafruitTwitter
	- add checkDirectMessage()
	- add setDirectMessageRecvCallback()
- SoftAP support with FeatherAP
	- add setJoinCallback(), setLeaveCallback()
	- add clientNum() clientMAC()
	- add interface properties to AdafruitTCP & AdafruitTCPServer, change constructors accordingly.
	- add interface() to return current bound interface (STATION or SoftAP)
	- add started()
	- enhance AdafruitTCP/AdafruitTCPServer to support both STA & SoftAP interface
	- enhance AdafruitUDP to support both STA & SoftAP interface
	- add APChatServer, APClientRSSI, APTcpServer examples
- AdafruitHTTPServer support
	- Static, Dynamic Page support (SimpleHTTPServer example)
	- Page resource generation with advance page design (D3Graphic example)
	- add tools/pyresource to generate C header for HTTP Server
	- support resource files in SPI Flash
- Enable SPI Flash support with AdafruitFatfs, FatFile, FatDir, FileInfo 

# 0.5.5

- rename HardWire to TwoWire to stay compatible with Arduino
- fix #43 : Implement UDP as buffered stream (enabled by default), can be changed using usePacketBuffering()
- Add .config() to manual set static IP
- Add TLS Mutual Verification support (Client Identity verification) with .tlsSetIdentity()
- "VeriSign Class 3 Public Primary Certification Authority - G5" replace "Starfield Services Root Certificate Authority - G2" for AWS IOT 
- Add AdafruitTwitter to send Tweet, and send Direct Message
- Add AdafruitSHA1 to perform SHA1, and HMAC-SHA1 computation
- AdafruitHTTP
	- Change post() API: will perform URLEncode for Post Values
	- add postWithoutURLencoded(): skip URLEncode for values
	- add postRaw(): post data as binary, useful for binary data.
	- add urlEncode(), urlEncodeLength(), base64Encode() as static method

## Known Issues

- When connecting with TLS Mutual Verification (client verification) such as AWS IOT, if Identity is not set, it will cause an hardfault due to Broadcom SSL library ssl_write_certificate() when handshaking.

# 0.5.2

- Add Servo library suport
- Fix issue where 0xff is misinterpreted as EOF with USBSerial
- Fix EXT GPIO interrupt conflict between Featherlib & Arduino
- Fix #32 PWM only work after 3-5 minutes
- Fix compiling issues with existing library

# 0.5.1

- `Baltimore CyberTrust Root` replace `VeriSign Class 3 Public Primary Certification Authority - G5` in default root certificate list
- Fix #38 `Disconnect callback for MQTT and TCP don't imply that connected() method returns false` 
- Fix #36, #37 : Rework I2C with repeated START for MPL3115A2 sensor and alike.
- Use pre-compiled feather_dfu for Windows
- Fix Linux's rules for USB VID/PID
- Update signed driver for Windows

# Version 0.5.0

- First Release
