#include "io.h"
#include "eeprom.h"
#include <stdio.h>
#include <pico/stdlib.h>

const data_t LOW_RESET_VECTOR = 0x00;
const data_t HIGH_RESET_VECTOR = 0xF0;

const data_map_t data = {
    // First instruction of the program
    {0x7000, 0xEA},
    {0x7001, 0xEA},
    {0x7002, 0xEA},
    {0x7003, 0xEA},
    {0x7004, 0xEA},
    {0x7005, 0xEA},
    {0x7006, 0xEA},
    {0x7007, 0xEA},
    {0x7008, 0xEA},
    // Reset Vectors
    {0x7FFA, LOW_RESET_VECTOR},
    {0x7FFB, HIGH_RESET_VECTOR},
    {0x7FFC, LOW_RESET_VECTOR},
    {0x7FFD, HIGH_RESET_VECTOR},
    {0x7FFE, LOW_RESET_VECTOR},
    {0x7FFF, HIGH_RESET_VECTOR},
};

int main() {
    stdio_init_all();

    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, true);
    
    EEPROM eeprom = EEPROM();
    eeprom.load(data);

    IO io = IO(pio, sm);
    
    while(true) {
        addrbus_t address = io.address();
        data_t data = eeprom.read(address);        
        io.wdata(data);
        printf("A: 0x%04X D: 0x%02X\n", address, data);
    }
}