/*Microcontroladores 2025  INSPT-UTN
TP ADC
Alumno: González Tomás
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"
#include "hardware.h"
#include "funciones.h"

//Estados 
typedef enum estado{
    llenar_tanque,
    llenar_cisterna
}estado;


int main()
{
    //Inicializaciones
    init_hardware();

    //Configuración del estado inicial del sistema
    uint32_t nivel_tanque = 0, nivel_cisterna = 0;
    estado estado_actual;

    //Primer lectura de adc al prender el sistema, sin promediar.
    init_adc(&nivel_tanque, &nivel_cisterna);
    
    if((nivel_cisterna >= level_bomba_on) && (nivel_tanque <= nivel_min_sup)){
        estado_actual = llenar_tanque;
    }
    else if ((nivel_cisterna <= level_bomba_on) &&  (nivel_tanque >= nivel_max_sup)){
        estado_actual = llenar_cisterna;
    }
    else if(nivel_cisterna >= level_bomba_on && (gpio_get(LEDR_PIN) == 0) && nivel_tanque <=nivel_min_sup ){   //Acá tomo el estado del boton para que se habilite la cisterna cuando 
        estado_actual = llenar_tanque;                                                                         //ya este bien cargado y no se prenda y apague cerca del valor de level_bomba_on
    }

    //Maquina de estados
    while(1){
        switch (estado_actual)
        {
        case llenar_tanque:
            valor_adc_tanque(&nivel_tanque);                                              
            valor_adc_cisterna(&nivel_cisterna);

            if((nivel_cisterna >= level_bomba_on) && (nivel_tanque <= nivel_min_sup)){
                bomba_on();//Bomba y LedV ON
                led_amarillo(&nivel_tanque);
                led_rojo(&nivel_cisterna);
                estado_actual = llenar_tanque;
            }
            else if(nivel_cisterna <= level_bomba_on && nivel_tanque >= nivel_max_sup){
                estado_actual = llenar_cisterna;
            }
        break;
                 
        case llenar_cisterna:
            valor_adc_tanque(&nivel_tanque);
            valor_adc_cisterna(&nivel_cisterna);

            if(nivel_cisterna < level_bomba_on && nivel_tanque >= nivel_max_sup){
                bomba_off();
                led_amarillo(&nivel_tanque);
                led_rojo(&nivel_cisterna);
                estado_actual = llenar_cisterna;
            }
            else if (nivel_cisterna >= level_bomba_on && (gpio_get(LEDR_PIN) == 0) && nivel_tanque <=nivel_min_sup )
            {
                estado_actual  = llenar_tanque;
            }
        break;  
               
        default:                                                                //Si cae en un caso que se escapa de los planteados, se prenden todos
            bomba_off();                                                          //los leds y se apaga la bomba por precaución.
            gpio_set_mask((1 << LEDR_PIN) | (1 << LEDA_PIN) | (1 << LEDV_PIN));
        break;
        }
    }

}

