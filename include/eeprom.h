#pragma once
#include <stdio.h>
#include <map>

#define EEPROM_MAX_SIZE 0x8000

typedef uint16_t address_t;
typedef uint8_t data_t;

class EEPROM {
public:
    data_t read(address_t address);
};