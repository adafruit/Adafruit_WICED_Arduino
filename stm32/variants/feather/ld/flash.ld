/*
 * Adafruit WICED (STM32F205RG, high density) linker script for
 * Flash builds.
 */

MEMORY
{
  ram (rwx) : ORIGIN = 0x2001B000, LENGTH = 20K
  rom (rx)  : ORIGIN = 0x080C0000, LENGTH = 256K
}

/* GROUP(libcs4_stm32_high_density.a) */

REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

_FLASH_BUILD = 1;
INCLUDE common.inc
