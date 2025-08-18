#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"
#include "hardware.h"

//Estados 
typedef enum estado{
    tanque_lleno,
    tanque_medio,
    tanque_vacio,
    cisterna_llena
}estado;

//Antirebote del pulsador
volatile uint32_t demora = 0;
void puls_callback(uint gpio, uint32_t event_mask);

void bomba_off();
void bomba_on();

int main()
{
    //Inicializaciones
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

    uint32_t estado_tanque = 0, estado_cisterna = 0;
    
    adc_select_input(FLOT_BAJO_PIN);
    estado_cisterna = adc_read();


}

//Función antirebote del pulsador
void puls_callback(uint gpio, uint32_t event_mask) {
    demora = get_systick() + REBOTE_PULS;
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