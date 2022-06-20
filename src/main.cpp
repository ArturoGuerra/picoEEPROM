#include "io.h"
#include "eeprom.h"
#include <stdio.h>
#include <pico/stdlib.h>

int main() {
    stdio_init_all();

    PIO pio = pio0;
    int sm = pio_claim_unused_sm(pio, true);
    
    EEPROM eeprom = EEPROM();
    IO io = IO(pio, sm);
    
    while(true) {
        addrbus_t address = io.address();
        data_t data = eeprom.read(address);        
        io.wdata(data);
        printf("A: 0x%04X D: 0x%02X\n", address, data);
    }
}