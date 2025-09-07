#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"

#define LED_PIN 16
#define BT_PIN 15
#define DELAY_BT 20

volatile uint32_t demora = 0, lapso = 0;

void puls_callback(uint gpio, uint32_t event_mask);

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BT_PIN);
    gpio_set_dir(BT_PIN, GPIO_IN);
    gpio_pull_up(BT_PIN);
    gpio_set_input_hysteresis_enabled(BT_PIN, true);

    /* Aquí se configura el pin fuente de interrupción, el evento (flanco descendente) y la función callback */
    gpio_set_irq_enabled_with_callback(BT, GPIO_IRQ_EDGE_FALL, true, puls_callback);

    while (true) {
        if (gpio_get(BT) == 1)
            {
                if (demora <= get_systick()) {
                    if(gpio_get(BT) == 1){
                        gpio_xor_mask(1 << LED_PIN);
                        sleep_ms(167);
                    }
                }
            }
        
    }
}

void puls_callback(uint gpio, uint32_t event_mask) {
    demora = get_systick() + DELAY_BT;
}