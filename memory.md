# Memory

## Memory Map

![MemMap](https://cdn-learn.adafruit.com/assets/assets/000/035/751/medium800/adafruit_products_WICEDMemMap.png?1473874358)

- User Code (256KB + 20KB SRAM)
- Feather Lib (704 KB + 108KB SRAM)
- Config Data (32KB)
- USB DFU Bootloader (32KB)

## User SRAM Usage

The following information is based on FeatherLib 0.6.1:

- Arduino core : 5664 bytes (Based on [Arduino_STM32](https://github.com/rogerclarkmelbourne/Arduino_STM32))
- AdafruitTCP (or Client) : 400 bytes
- TLS support for AdafruitTCP : 2600 bytes
- TLS Indentity : 200 bytes
- AdafruitUDP : 180 bytes
- AdafruitHTTPServer : 568 bytes
- SPIFlash Fatfs : 4152 byts
- FatFile : 40 bytes
- FatDir : 50 bytes
- FileInfo : 280 bytes (Long File Name support)
