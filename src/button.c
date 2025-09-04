#include "button.h"

#include <stdint.h>
#include <stdbool.h>

#include "hardware/gpio.h" 
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "board_defs.h"

static const uint8_t button_gpio[] = BT_DEFS;
#define BUTTON_GPIO_LEN (sizeof(button_gpio))

static bool sw_val[BUTTON_GPIO_LEN]; /* true if pressed */
static uint64_t sw_freeze_time[BUTTON_GPIO_LEN];

static uint8_t button_reading;

void button_init() {
    for (int i = 0; i < BUTTON_GPIO_LEN; i++) {
        sw_val[i] = false;
        sw_freeze_time[i] = 0;
        int8_t gpio = button_gpio[i];
        gpio_init(gpio);
        gpio_set_function(gpio, GPIO_FUNC_SIO);
        gpio_set_dir(gpio, GPIO_IN);
        gpio_pull_up(gpio);
    }

    button_reading = 0;
    for (int i = 0; i < BUTTON_GPIO_LEN; i++) {
        sw_val[i] = !gpio_get(button_gpio[i]);
        if (sw_val[i]) {
            button_reading |= (1 << i);
        }
    }
}

uint8_t button_gpio_len()
{
    return BUTTON_GPIO_LEN;
}

#define DEBOUNCE_FREEZE_TIME_US 3000
void button_update()
{
    uint64_t now = time_us_64();
    uint8_t buttons = 0;

    for (int i = BUTTON_GPIO_LEN - 1; i >= 0; i--) {
        bool sw_pressed = !gpio_get(button_gpio[i]);
        
        if (now >= sw_freeze_time[i]) {
            if (sw_pressed != sw_val[i]) {
                sw_val[i] = sw_pressed;
                sw_freeze_time[i] = now + DEBOUNCE_FREEZE_TIME_US;
            }
        }

        buttons <<= 1;
        if (sw_val[i]) {
            buttons |= 1;
        }
    }

    button_reading = buttons;
}

uint8_t button_read()
{
    return button_reading;
}