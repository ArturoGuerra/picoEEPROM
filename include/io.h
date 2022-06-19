#pragma once
#include <stdio.h>
#include <hardware/pio.h>

typedef uint16_t addrbus_t;
typedef uint8_t databus_t;


class IO {
private:
    PIO pio;
    int sm;

public:
    IO(PIO pio, int sm);

    addrbus_t address();
    void wdata(databus_t data);
    void debug();
};