#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "hardware/pwm.h"
#include <stdbool.h>

#define LED_PIN 16
#define PULS_PIN 15
#define DELAY_PULS 30

#define F_PWM 10 //Hz
#define DUTY 0.5f
#define F_COUNTER 150000000UL//150MHz
#define WRAP 65535 //16 bits resolucion
#define DIV ((float) F_COUNTER/(F_PWM * (WRAP + 1)))
//#define LEVEL ((uint16_t)(DUTY * (WRAP + 1)))

typedef enum estados{
    parp_off,
    parp_on
}estados_t;

volatile uint32_t demora = 0; 
volatile bool boton_presionado = false;
void puls_callback(uint gpio, uint32_t event_mask); 

uint16_t level;
 
int main()
{
    stdio_init_all();
    init_systick();

    //Configuración botón
    gpio_init(PULS_PIN);
    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN);

    gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);
    
    //Configuración PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
   
    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, DIV);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    pwm_set_enabled(slice_num, true);

    //Máquina de estados
    estados_t estado_actual = parp_off;
    
    while(1)
    {
        switch (estado_actual)
        {
        case parp_off:
            level = 0;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            
            if(boton_presionado){
                boton_presionado = false;
                estado_actual = parp_on;
            }    
        break;
        
        case parp_on:
            level = DUTY * (WRAP + 1);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            
            if(boton_presionado) {
                boton_presionado = false;
                estado_actual = parp_off;
            } 
        break;    

        default:
        break;
        }
    } 

}

void puls_callback(uint gpio, uint32_t event_mask){
    if (gpio == PULS_PIN && event_mask == GPIO_IRQ_EDGE_FALL){
        if (get_systick() < demora) return;  // Antirrebote
        demora = get_systick() + DELAY_PULS;
        boton_presionado = true; // Señal para el main
    }
}