# 0.5.5 (Work In Progress)

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
