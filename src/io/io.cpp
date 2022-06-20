#include "io.h"
#include "bus.pio.h"

#define DATABUS_BASE_PIN 2
#define DATABUS_LEN 8

#define ADDRESSBUS_BASE_PIN 10
#define ADDRESSBUS_LEN 19

#define ADDRESSBUS_RMASK 0x1FFF
#define ADDRESSBUS_LMASK 0x6000
#define ADDRESSBUS_LSHIFT 3

static inline void io_init(PIO pio, int sm, uint offset);

IO::IO(PIO pio, int sm) {
    this->pio = pio;
    this->sm = sm;

    uint offset = pio_add_program(pio, &bus_program);
    io_init(pio, sm, offset);
}

addrbus_t IO::address() {
    uint32_t pins = pio_sm_get_blocking(this->pio, this->sm);
    //return pins;
    return ((pins >> ADDRESSBUS_LSHIFT) & ADDRESSBUS_LMASK) | (pins & ADDRESSBUS_RMASK);
}

void IO::wdata(databus_t data) {
    pio_sm_put_blocking(this->pio, this->sm, data);
}

void IO::debug() {
    uint pin_mask = 0;
    uint pin_dirs = 0;

    for (int i = 0; i < DATABUS_LEN; i++) {
        uint pin = DATABUS_BASE_PIN + i;
        pio_gpio_init(pio, pin);
        pin_mask |= (1 << pin);
        pin_dirs |= (1 << pin);
    }
    
    for (int i = 0; i < ADDRESSBUS_LEN; i++) {
        uint pin = ADDRESSBUS_BASE_PIN + i;
        pio_gpio_init(pio, pin);
        pin_mask |= (1 << pin);
    }

    printf("M: 0x%08X\n", pin_mask);
    printf("D: 0x%08X\n", pin_dirs);
}

static inline void io_init(PIO pio, int sm, uint offset) {    
    uint pin_mask = 0;
    uint pin_dirs = 0;

    
    for (int i = 0; i < ADDRESSBUS_LEN; i++) {
        uint pin = ADDRESSBUS_BASE_PIN + i;
        pio_gpio_init(pio, pin);
        pin_mask |= (1 << pin);
    }
    for (int i = 0; i < DATABUS_LEN; i++) {
        uint pin = DATABUS_BASE_PIN + i;
        pio_gpio_init(pio, pin);
        pin_mask |= (1 << pin);
        pin_dirs |= (1 << pin);
    }
    
    pio_sm_set_pindirs_with_mask(pio, sm, pin_dirs, pin_mask);

    pio_sm_config c = bus_program_get_default_config(offset);

    sm_config_set_in_pins(&c, ADDRESSBUS_BASE_PIN);
    sm_config_set_out_pins(&c, DATABUS_BASE_PIN, DATABUS_LEN);
    sm_config_set_set_pins(&c, DATABUS_BASE_PIN, DATABUS_LEN);

    sm_config_set_in_shift(&c, false, false, ADDRESSBUS_LEN);
    sm_config_set_out_shift(&c, true, false, DATABUS_LEN);
    sm_config_set_jmp_pin(&c, 28);

    pio_sm_init(pio, sm, offset + bus_offset_entry_point, &c);
    pio_sm_set_enabled(pio, sm, true);
}

