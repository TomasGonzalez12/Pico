#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"

#define REBOTE_PULS 30

#define LEDR_PIN 11
#define LEDA_PIN 12
#define LEDV_PIN 13

#define FLOT_BAJO_PIN 27 // ADC1 Cisterna
#define FLOT_ALTO_PIN 26 //ADC0 Tanque
#define PULS_PIN 18
#define CTRL_BOMBA 16

// Tanque
#define nivel_max_sup 1740
#define nivel_min_sup 465
#define cap_sup (nivel_max_sup - nivel_min_sup)
#define nivel_medio_sup (nivel_min_sup + cap_sup/2)
#define level_ledA_on (nivel_min_sup + cap_sup/4)
#define level_ledA_off (nivel_min_sup + cap_sup * 3/4)

// Cisterna
#define nivel_max_inf 2040
#define nivel_min_inf 707
#define cap_inf (nivel_max_inf - nivel_min_inf)
#define level_ledR_on (nivel_min_inf + cap_inf/4)
#define level_ledR_off (nivel_min_inf + cap_inf * 3/4)
#define level_bomba_on (nivel_min_inf + cap_inf/2)

//Estados 
typedef enum estado{
    tanque_lleno,
    tanque_medio,
    tanque_vacio,
}estado;

//Antirebote del pulsador
volatile uint32_t demora = 0;
// void puls_callback(uint gpio, uint32_t event_mask);

// Prototipo de las funciones
void bomba_off();
void bomba_on();
void leer_adc(uint32_t* cisterna, uint32_t* tanque);
void led_amarillo(uint32_t* tanque);
void led_rojo(uint32_t* cisterna);

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
    //gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);

    //Configuración del estado inicial del sistema
    uint32_t nivel_tanque = 0, nivel_cisterna = 0;
    estado estado_actual;

    leer_adc(&nivel_cisterna, &nivel_tanque);
    
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
                bomba_on();//Bomba y LedV ON
                led_amarillo(&nivel_tanque);
                led_rojo(&nivel_cisterna);
                estado_actual = tanque_vacio;
            }
            else if(nivel_cisterna <= nivel_min_inf && nivel_tanque >= nivel_max_sup){
                estado_actual = tanque_lleno;
            }
        break;
                 
        case tanque_lleno:
            leer_adc(&nivel_cisterna, &nivel_tanque);
            if(nivel_cisterna <= nivel_min_inf && nivel_tanque >= nivel_max_sup){
                bomba_off();
                led_amarillo(&nivel_tanque);
                led_rojo(&nivel_cisterna);
                estado_actual = tanque_lleno;
            }
        break;

        case tanque_medio:
            leer_adc(&nivel_cisterna, &nivel_tanque);
            if((gpio_get(LEDA_PIN) != 0) && nivel_cisterna >= level_bomba_on && nivel_tanque >= nivel_medio_sup){
                bomba_off();
                led_rojo(&nivel_cisterna);
                estado_actual = tanque_medio;
            }

        break;    
        
        default:
            break;
        }
    }

}

//Función antirebote del pulsador
/*void puls_callback(uint gpio, uint32_t event_mask) {
    demora = get_systick() + REBOTE_PULS;
}*/ 

// Funciones de la bomba
void bomba_off(){
    gpio_put(CTRL_BOMBA, 1);
    gpio_put(LEDV_PIN, 0);
}

void bomba_on(){
    gpio_put(CTRL_BOMBA, 0);
    gpio_put(LEDV_PIN, 1);
}

//Funcion leer valor de ambos ADC
void leer_adc(uint32_t* cisterna, uint32_t* tanque) {
    adc_select_input(1); 
    *cisterna = adc_read();
    adc_select_input(0);
    *tanque = adc_read();
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