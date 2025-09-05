#include <stdio.h>

#include "pico/stdlib.h"

#include "button.h"

void printbits(uint8_t x)
{
    for(int i=sizeof(x)<<3; i; i--)
        putchar('0'+((x>>(i-1))&1));
}

void cli_run() {
    bool connected = stdio_usb_connected();
    if (!connected) {
        return;
    }

    //Output raw values from buttons and knobs.
    uint8_t buttons = button_read();
    printbits(buttons);
    putchar('\n');
}