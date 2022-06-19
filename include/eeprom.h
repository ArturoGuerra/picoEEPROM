#pragma once
#include <stdio.h>
#include <map>

#define EEPROM_MAX_SIZE 0x8000
//#define EEPROM_MAX_SIZE 0x10000

typedef uint16_t address_t;
typedef uint8_t data_t;
typedef std::map<address_t, data_t> data_map_t;

class EEPROM {
private:
    data_t memory[EEPROM_MAX_SIZE];
public:
    EEPROM();
    void write(address_t address, data_t data);
    data_t read(address_t address);
    void load(const data_map_t &data);
};