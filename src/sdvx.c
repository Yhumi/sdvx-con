#include <stdio.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"

#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/structs/ioqspi.h"
#include "hardware/structs/sio.h"

#include "tusb.h"
#include "usb_descriptors.h"

#include "cli.h"
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

struct __attribute__((packed)) {
    uint8_t buttons;
    uint8_t joy[2];
} hid_report, old_hid_report;

#define ALL_BUTTON_MASK 0x7f

void hid_update() {
    uint8_t buttons = button_read();

    hid_report.buttons = buttons & ALL_BUTTON_MASK;

    if (tud_hid_ready()) {
        if ((memcmp(&hid_report, &old_hid_report, sizeof(hid_report)) != 0) &&
             tud_hid_report(REPORT_ID_JOYSTICK, &hid_report, sizeof(hid_report))) {
            old_hid_report = hid_report;
        }
    }
}

void core0_loop() {
    uint64_t next_frame = 0;
    while (true) {
        tud_task();

        button_update();
        hid_update();

        cli_run();

        sleep_until(next_frame);
        next_frame += 1000;
    }
}

void led_init() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void init() {
    tusb_init();
    stdio_init_all();

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

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen)
{
    printf("Get from USB %d-%d\n", report_id, report_type);
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize)
{
    //Hi
}