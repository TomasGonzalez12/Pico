#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED_PIN 16

#define F_PWM 100 // Hz
#define DUTY 0.5f

#define F_COUNTER 150000000UL // 150MHz
#define WRAP 65535 // 16 bits resolucion (0 - 65535)
#define DIV ((float)F_COUNTER / (F_PWM * (WRAP + 1))) //22.9 (0 - 255)

#define F_LED 2  // Hz
#define num_wraps (uint8_t)(F_PWM / F_LED)  // Número de wraps por ciclo
#define t_off (uint8_t)(num_wraps / 2)      // 50% duty
#define t_on num_wraps                      // Ciclo completo 

uint32_t level = 0;
uint slice_num = 0;

uint8_t flag_led = 0;
volatile uint32_t wrap_cont = 0;

void on_pwm_wrap(void);

int main() {
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

    while (1) {
        
    }
}

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