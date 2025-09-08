#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"

#define LED_PIN     16
#define PULS_PIN    15

#define DELAY_MS    20
#define SLEEP       250

volatile uint32_t ahora = 0, demora = 0;
uint32_t lapso = 0;
volatile bool f_puls = false;

typedef enum{
    off,
    parp
}estados_t;

estados_t estado_actual = off;

void puls_callback(uint gpio, uint32_t event_mask);

int main() {
    stdio_init_all();
    init_systick();

    gpio_init(LED_PIN);
    gpio_init(PULS_PIN);

    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); /*me aseguro que inicie bajo*/

    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN); /* Conecta la reistencia de pull down interna (redundante) */
    gpio_set_input_hysteresis_enabled(PULS_PIN, true); /* habilita la histéresis que ayuda discriminar mejor el rebote */

    /* Aquí se configura el pin fuente de interrupción, el evento (flanco descendente) y la función callback */
    gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);

    while (1) {
        switch (estado_actual) {
            case off:
                if (f_puls) {
                    f_puls = false;
                    estado_actual = parp;
                }
            break;

            case parp:
                if (lapso <= get_systick()) {
                    gpio_xor_mask( 1 << LED_PIN);
                    lapso += SLEEP; 
                }

                if (f_puls) {
                    f_puls = false;
                    gpio_put(LED_PIN, 0);
                    estado_actual = off; 
                }
            break;
        }
    }   

}

/* Callback de atención de interrupción */
void puls_callback(uint gpio, uint32_t event_mask) {
    ahora = get_systick();

    if (ahora < demora) {
        return;
    }

    f_puls = true;
    demora = ahora + DELAY_MS;
}