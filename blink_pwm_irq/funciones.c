#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "definiciones.h"
#include "funciones.h"

//Variables globales a funciones.c
uint32_t level = 0;
uint slice_num = 0;

uint8_t flag_led = 0;
volatile uint32_t wrap_cont = 0;

//Configuraciones generales
void init_config(){
    stdio_init_all();

    // Configuración de PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LED_PIN);

    // Configuración de interrupción
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, DIV);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    pwm_set_enabled(slice_num, true);
}

//Interrupcion PWM
void on_pwm_wrap() {
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(slice_num);
   
    wrap_cont++; 

    if(F_PWM > 0 && F_LED > 0 && F_LED < 9){
        if (wrap_cont < t_off) {
            if (level == 0) {
                //level = 0;  
                pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
                //flag_led = 0;
            }
        }
        else if (wrap_cont < t_on) {
            if (level == 0) {
                level = WRAP; 
                pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
                //flag_led = 1;
            }
        }
        else {
            wrap_cont = 0; 
            //flag_led = 0;
            level = 0;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
        }
    }    
}