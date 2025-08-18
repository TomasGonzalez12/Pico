#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"
#include "hardware.h"
#include "funciones.h"


//Función antirebote del pulsador
uint32_t demora = 0;
void puls_callback(uint gpio, uint32_t event_mask) {
    demora = get_systick() + REBOTE_PULS;
} 

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

//Toman el valor de 10 muestras del adc y saca un promedio 
uint32_t tiempo = 0;
uint32_t acum_tanque = 0 , cont_tanque = 0, prom_tanque = 0;
uint32_t acum_cis = 0, cont_cis = 0, prom_cis = 0;

void valor_adc_tanque(uint32_t* tanque) {
    tiempo = get_systick();
    adc_select_input(0);   //ADC del tanque
    if (tiempo >= get_systick()) {
            tiempo += delay_ms;
             acum_tanque += adc_read();
             cont_tanque++;
            if (cont_tanque >= MUESTRAS) {
                prom_tanque = acum_tanque / cont_tanque;
                *tanque = prom_tanque;
                acum_tanque = 0;
                cont_tanque = 0;
                tiempo = 0;
            }
    } 
}     
    
void valor_adc_cisterna(uint32_t* cisterna){ 
    tiempo = get_systick();
    adc_select_input(1);   //ADC del cisterna
    if (tiempo >= get_systick()) {
            tiempo += delay_ms;
             acum_cis += adc_read();
             cont_cis++;
            if (cont_cis >= MUESTRAS) {
                prom_cis = acum_cis / cont_cis;
                *cisterna = prom_cis;
                acum_cis = 0;
                cont_cis = 0;
                tiempo = 0;
            }
    } 
}

// Led Amarillo encendido/apagado
void led_amarillo(uint32_t* tanque){
    if(*tanque < level_ledA_on){
        gpio_put(LEDA_PIN, 1);
    }
    else if(*tanque > level_ledA_off){
        gpio_put(LEDA_PIN,0);
    }
}

// Led Rojo encendido/apagado
void led_rojo(uint32_t* cisterna){
    if(*cisterna < level_ledR_on){
        gpio_put(LEDR_PIN, 1);
    }
    else if(*cisterna > level_ledR_off){
        gpio_put(LEDR_PIN,0);
    }
}

//Pulsador para apagar la bomba
void puls_bomba(){
    if (demora <= get_systick()) {
        if(gpio_get(PULS_PIN) == 0){
            bomba_off();
        }    
    }        
}