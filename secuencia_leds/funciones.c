#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "definiciones.h"
#include "funciones.h"

// Configuraciones generales
void init_config(){
    stdio_init_all();
    init_systick();

    gpio_init_mask(LED_MASK);
    gpio_set_dir_out_masked(LED_MASK);
    gpio_clr_mask(LED_MASK);

    gpio_init(PULS_PIN);
    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN);
    gpio_set_input_hysteresis_enabled(PULS_PIN, true);

    gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);
}

//Flags
flags_s flag = {0};

void reset_led_flags(){
    flag.ledR_on = false;
    flag.ledA_on = false;
    flag.ledV_on = false;
}


//Antirrebote del botón
volatile uint32_t ahora, ventana;
volatile bool puls_presionado = false;

void puls_callback(uint gpio, uint32_t event_mask){
    ahora = get_systick();

    if(ahora <= ventana) return;

    puls_presionado = true;
    ventana = ahora + DELAY_MS;
}