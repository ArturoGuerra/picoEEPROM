#include "eeprom.h"

extern "C" uint8_t _binary_rom_bin_start[];

// Reads one byte from EEPROM
uint8_t EEPROM::read(address_t address) {
    return *(_binary_rom_bin_start + address);
}