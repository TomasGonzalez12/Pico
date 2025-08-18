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
}estado;

//Antirebote del pulsador
volatile uint32_t demora = 0;
void puls_callback(uint gpio, uint32_t event_mask);

// Prototipo de las funciones
void bomba_off();
void bomba_on();
void leer_adc(uint32_t* cisterna, uint32_t* tanque);

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

    //Configuración del estado inicial del sistema
    uint32_t nivel_tanque = 0, nivel_cisterna = 0;
    estado estado_actual;
    
    if((nivel_cisterna >= level_bomba_on) && (nivel_tanque <= nivel_min_sup)){
        estado_actual = tanque_vacio;
    }
    else if ((nivel_cisterna <= nivel_min_inf) &&  (nivel_tanque >= nivel_max_sup)){
        estado_actual = tanque_lleno;
    }
    else{
        estado_actual = tanque_medio;
    }
    
    //Maquina de estados
    while(1){
        switch (estado_actual)
        {
        case tanque_vacio:
            leer_adc(&nivel_cisterna, &nivel_tanque);
            if((nivel_cisterna >= level_bomba_on) && (nivel_tanque <= nivel_min_sup)){
                bomba_on();
            }
            break;
        
        default:
            break;
        }
    }

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

void leer_adc(uint32_t* cisterna, uint32_t* tanque) {
    adc_select_input(FLOT_BAJO_PIN); 
    *cisterna = adc_read();
    adc_select_input(FLOT_ALTO_PIN);
    *tanque = adc_read();
}