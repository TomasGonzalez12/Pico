#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "definiciones.h"
#include "funciones.h"


int main()
{
    init_config();

    //Máquina de estados
    estados_t estado_actual = led_off;

    while(1)
    {
        switch (estado_actual)
        {
        case led_off:
            level = 0;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            
            if(f_puls){
                f_puls = false;
                estado_actual = led_on;
            }    
        break;
        
        case led_on:
            level = adc_read();
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            
            if(f_puls) {
                f_puls = false;
                estado_actual = led_off;
            } 
        break;    

        default:
        break;
        }
    } 

}

