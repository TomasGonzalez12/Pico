#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 22

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_xor_mask(1 << LED_PIN);
        sleep_ms(167);
    }
}