#include "eeprom.h"

EEPROM::EEPROM() {
    // Initialize EEPROM with 0x00
    for (int i = 0; i < EEPROM_MAX_SIZE; i++) {
        memory[i] = 0x00;
    }
}

// Writes one byte to EEPROM
void EEPROM::write(address_t address, data_t data) {
    memory[address] = data;
}

// Reads one byte from EEPROM
uint8_t EEPROM::read(address_t address) {
    return memory[address];
}

// Bulk writes data to EEPROM
void EEPROM::load(const data_map_t &data) {
    for (auto &byte : data) {
        memory[byte.first] = byte.second;
    }
}