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

    gpio_init(CTRL_BOMBA);
    gpio_set_dir(CTRL_BOMBA, GPIO_OUT);
    gpio_put(CTRL_BOMBA, 0);

    adc_init();
    adc_gpio_init(FLOT_BAJO_PIN);
    adc_gpio_init(FLOT_ALTO_PIN);

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

//Funciones para sacar promedio de los valores del adc
uint32_t tiempo_tanq = 0, cont_tanq = 0, acum_tanq = 0, prom_tanq = 0;
uint32_t tiempo_cis = 0, cont_cis = 0, acum_cis = 0, prom_cis = 0;
uint32_t band_prom_tanq = 1, band_prom_cis = 1;


void valor_adc_tanque(uint32_t* tanque) {
    if (get_systick() >= tiempo_tanq) {
        tiempo_tanq = get_systick() + delay_ms;
        adc_select_input(0);
        acum_tanq += adc_read();
        cont_tanq++;
        band_prom_tanq = 1;
        if (cont_tanq == MUESTRAS) {
            *tanque = acum_tanq / cont_tanq;
            acum_tanq = 0;
            cont_tanq = 0;
            band_prom_tanq = 0;
        }
    }
}

void valor_adc_cisterna(uint32_t* cisterna) {
    if (get_systick() >= tiempo_cis) {
        tiempo_cis = get_systick() + delay_ms;
        adc_select_input(1);
        acum_cis += adc_read();
        cont_cis++;
        band_prom_cis = 1;
        if (cont_cis == MUESTRAS) {
            *cisterna = acum_cis / cont_cis;
            acum_cis = 0;
            cont_cis = 0;
            band_prom_cis = 0;
        }
    }
}

// Led Amarillo encendido/apagado
void led_amarillo(uint32_t* tanque){
    if(*tanque < nivel_ledA_on){
        gpio_put(LEDA_PIN, 1);
    }
    else if(*tanque > nivel_ledA_off){
        gpio_put(LEDA_PIN,0);
    }
}

// Led Rojo encendido/apagado
void led_rojo(uint32_t* cisterna){
    if(*cisterna < nivel_ledR_on){
        gpio_put(LEDR_PIN, 1);
    }
    else if(*cisterna > nivel_ledR_off){
        gpio_put(LEDR_PIN,0);
    }
}

//Función antirebote del pulsador + apagar la bomba
volatile uint32_t demora = 0;
void puls_callback(uint gpio, uint32_t event_mask) {
    if(get_systick() < demora){
        return;
    }

    demora = get_systick() + REBOTE_PULS;
    
    if (demora <= get_systick()) {
        if(gpio_get(gpio) == 0){
            bomba_off();
        }    
    } 
} 

