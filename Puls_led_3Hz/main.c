#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"

#define LED 16
#define BT 15
#define REBOTE 30
#define SLEEP 333

typedef enum estado{
    suelto_off,
    presionado_parp,
    suelto_parp,
    presionado_off
}estado;

volatile uint32_t demora = 0, lapso = 0;

void puls_callback(uint gpio, uint32_t event_mask);

int main(){
    //Inicialización
    stdio_init_all();
    init_systick();

    gpio_init_mask(1 << LED | 1 << BT);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 0);
    gpio_set_dir(BT, GPIO_IN);
    gpio_pull_up(BT);
    gpio_set_input_hysteresis_enabled(BT, true);

    //Estado inicial
    estado estado_actual = suelto_off;

    /* Aquí se configura el pin fuente de interrupción, el evento (flanco descendente) y la función callback */
    gpio_set_irq_enabled_with_callback(BT, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, puls_callback);

    //Secuencia
     while(1){
     switch (estado_actual)
     {
        case suelto_off:
            if(gpio_get(BT) == 1)
            {
                gpio_put(LED, 0);
                estado_actual = suelto_off;
            }
            else if (gpio_get(BT) == 0)
            {
                if (demora <= get_systick()) {
                    if(gpio_get(BT) == 0){
                        gpio_put(LED, 1);
                        lapso = get_systick() + SLEEP;
                        estado_actual = presionado_parp;
                    }
                }
            }  
        break;

        case presionado_parp:
            if(gpio_get(BT) == 0)
            { 
                if (lapso <= get_systick()) {
                    gpio_xor_mask( 1 << LED);
                    lapso += SLEEP; 
                }
               estado_actual = presionado_parp; 
            }
            else if (gpio_get(BT) == 1)
            {
                if (demora <= get_systick()) {
                    if(gpio_get(BT) == 1){
                        estado_actual = suelto_parp;
                    }
                }
            }
        break;

        case suelto_parp:
            if(gpio_get(BT) == 1)
            {
                if (lapso <= get_systick()) {
                    gpio_xor_mask( 1 << LED);
                    lapso += SLEEP; 
                }
                estado_actual = suelto_parp;
            }
            else if (gpio_get(BT) == 0)
            {
                if (demora <= get_systick()) {
                    if (gpio_get(BT) == 0){
                        gpio_put(LED, 0);
                        estado_actual = presionado_off;
                    }
            }
            } 
        break;

        case presionado_off:
            if(gpio_get(BT) == 0)
            {
                gpio_put(LED, 0);
                lapso = 0;
                estado_actual = presionado_off;
            }
            else if (gpio_get(BT) == 1)
            {
                if (demora <= get_systick()) {
                    if(gpio_get(BT) == 1){
                        estado_actual = suelto_off;
                    }
                }
            }   
        break;

        default:
        break;
     }
}
    
}

void puls_callback(uint gpio, uint32_t event_mask) {
    demora = get_systick() + REBOTE;
}

