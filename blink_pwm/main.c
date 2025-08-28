#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED_PIN 16
#define F_PWM 9 //Hz
#define DUTY 0.9f
#define F_COUNTER 150000000UL //150MHz
#define WRAP 65535 //16 bits resolucion
#define DIV ((float) F_COUNTER/(F_PWM * (WRAP + 1)))
#define LEVEL ((uint16_t)(DUTY * (WRAP + 1)))
 
int main()
{
    stdio_uart_init();
    
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
   
    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, DIV);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, LEVEL);
    pwm_set_enabled(slice_num, true);

    while(1){
        tight_loop_contents();
    }

}
