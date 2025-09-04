#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "button.h"

static void update_led(bool led_on) {
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
}

static mutex_t core1_io_lock;
static void core1_loop() {
    while (true) {
        if (mutex_try_enter(&core1_io_lock, NULL)) {
            update_led(stdio_usb_connected());
            mutex_exit(&core1_io_lock);
        }
        sleep_ms(1000);
    }
}

void printbits(uint8_t x)
{
    for(int i=sizeof(x)<<3; i; i--)
        putchar('0'+((x>>(i-1))&1));
}

void hid_update() {
    uint8_t buttons = button_read();

    printbits(buttons);
    putchar('\n');
}

void core0_loop() {
    uint64_t next_frame = 0;
    while (true) {
        //tud_task();

        button_update();
        hid_update();

        sleep_until(next_frame);
        next_frame += 1000;
    }
}

void led_init() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void init() {
    stdio_init_all();
    stdio_usb_init();

    //tusb_init();
    mutex_init(&core1_io_lock);

    led_init();
    button_init();

    sleep_ms(50);
}

int main(void) {
    init();
    multicore_launch_core1(core1_loop);
    core0_loop();
    return 0;
}