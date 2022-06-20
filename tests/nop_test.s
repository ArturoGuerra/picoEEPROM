    .org 0x8000
    .org 0xF000
start:
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    .org 0xFFFC
    .word start
    .word start