#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "definiciones.h"
#include "funciones.h"

//Configuraciones de componentes y hardware

uint16_t level = 0;
uint slice_num = 0;

void init_config(){
    stdio_init_all();
    init_systick();

    //Configuración botón
    gpio_init(PULS_PIN);
    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN);

    gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);

    //Configuración adc
    adc_init();
    adc_gpio_init(POTE);
    adc_select_input(0);
    
    //Configuración PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LED_PIN);
    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, DIV);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    pwm_set_enabled(slice_num, true);
}

// Antirebote + delay del boton
volatile uint32_t demora = 0;
volatile uint8_t boton_presionado = 0;

void puls_callback(uint gpio, uint32_t event_mask){
    if (gpio == PULS_PIN && event_mask == GPIO_IRQ_EDGE_FALL){
        if (get_systick() < demora){
            return;
        }
        
        demora = get_systick() + DELAY_PULS;
        boton_presionado = 1;
    }
}