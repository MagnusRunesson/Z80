#!/bin/sh
zasm -uwy first.asm
bin2c first.bin Controller_Z84C0008PEG/first first
bin2c first.bin EEPROM_flasher_AT28C256/rom_image rom_image
