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
    estados_t estado_actual = parp_off;
    
    
    while(1)
    {
        switch (estado_actual)
        {
        case parp_off:
            level = 0;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            
            if(boton_presionado){
                boton_presionado = 0;
                estado_actual = parp_on;
            }    
        break;
        
        case parp_on:
            level = adc_read();
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            
            if(boton_presionado) {
                boton_presionado = 0;
                estado_actual = parp_off;
            } 
        break;    

        default:
        break;
        }
    } 

}

