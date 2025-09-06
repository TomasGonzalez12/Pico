#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"
#include "hardware.h"
#include "funciones.h"

//Inicializar los pines
void init_hardware(){
    stdio_init_all();
    init_systick();

    gpio_init_mask((1 << LEDR_PIN) | (1 << LEDA_PIN) | (1 << LEDV_PIN));
    gpio_set_dir_out_masked((1 << LEDR_PIN) | (1 << LEDA_PIN) | (1 << LEDV_PIN));
    gpio_clr_mask((1 << LEDR_PIN) | (1 << LEDA_PIN) | (1 << LEDV_PIN));

    gpio_init(PULS_PIN);
    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN);
    gpio_set_input_hysteresis_enabled(LED_PIN, true);

    gpio_init(CTRL_BOMBA);
    gpio_set_dir(CTRL_BOMBA, GPIO_OUT);
    gpio_put(CTRL_BOMBA, 0);

    adc_init();
    adc_gpio_init(FLOT_CIS_PIN);
    adc_gpio_init(FLOT_TANQ_PIN);

    //Configuración de función callback del pulsador
    gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);
}

// Funciones de la bomba
void bomba_off(){
    gpio_put(CTRL_BOMBA, 1);
    gpio_put(LEDV_PIN, 0);    
}

void bomba_on(){
    gpio_put(CTRL_BOMBA, 0);
    gpio_put(LEDV_PIN, 1);
    
}

// Led Amarillo encendido/apagado
void led_amarillo(uint32_t tanque){
    if(tanque <= nivel_ledA_on){
        gpio_put(LEDA_PIN, 1);
    }
    else if(tanque >= nivel_ledA_off){
        gpio_put(LEDA_PIN,0);
    }
}

// Led Rojo encendido/apagado
void led_rojo(uint32_t cisterna){
    if(cisterna <= nivel_ledR_on){
        gpio_put(LEDR_PIN, 1);
    }
    else if(cisterna >= nivel_ledR_off){
        gpio_put(LEDR_PIN,0);
    }
}

//Función antirebote del pulsador + apagar la bomba
volatile uint32_t demora = 0;
volatile bool boton_presionado = false;

void puls_callback(uint gpio, uint32_t event_mask) {
    if(get_systick() < demora){
        return;
    }

    demora = get_systick() + REBOTE_PULS;
    
    if(gpio_get(gpio) == 0){
        boton_presionado = true;
    }     
} 

