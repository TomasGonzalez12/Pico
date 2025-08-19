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
    inicio,
    llenar_tanque,
    llenar_cisterna
}estado;


int main()
{
    //Inicializaciones
    init_hardware();

    //Configuración del estado inicial del sistema
    nivel_tanque = 0, nivel_cisterna = 0;
    estado estado_actual = inicio;
   
    //Máquina de estados
    while(1){
        switch (estado_actual)
        {
        case inicio:
            if(band_prom_tanque && band_prom_cis){
                valor_adc_tanque(&nivel_tanque);                                              
                valor_adc_cisterna(&nivel_cisterna);
                estado_actual = inicio;
                break;
            }
            
            if (nivel_cisterna <= nivel_bomba_on){
                estado_actual = llenar_cisterna;
            }
            else if ((nivel_cisterna >= nivel_bomba_on) && (nivel_tanque <= nivel_min_tanq)){
                estado_actual = llenar_tanque;
            }
            else{
                estado_actual = llenar_cisterna;
            }

        break;

        case llenar_cisterna:
            bomba_off();
            led_amarillo(&nivel_tanque);
            led_rojo(&nivel_cisterna);

            if(band_prom_tanque && band_prom_cis){
                valor_adc_tanque(&nivel_tanque);                                              
                valor_adc_cisterna(&nivel_cisterna);
                estado_actual = llenar_cisterna;
                break;
            }

            if(nivel_cisterna < nivel_bomba_on && nivel_tanque >= nivel_max_tanq){
                estado_actual = llenar_cisterna;
            }
            else if (nivel_cisterna >= nivel_bomba_on && (nivel_tanque <= nivel_min_tanq || nivel_tanque <= nivel_medio_tanq) )
            {
                estado_actual  = llenar_tanque;
            }
        break;

        case llenar_tanque:
            bomba_on();
            led_amarillo(&nivel_tanque);
            led_rojo(&nivel_cisterna);

            if(band_prom_tanque && band_prom_cis){
                valor_adc_tanque(&nivel_tanque);                                              
                valor_adc_cisterna(&nivel_cisterna);
                estado_actual = llenar_tanque;
                break;
            }

            if((nivel_cisterna >= nivel_bomba_on) && (nivel_tanque <= nivel_min_tanq)){
                estado_actual = llenar_tanque;
            }
            else if(nivel_cisterna <= nivel_bomba_on && nivel_tanque >= nivel_max_tanq){
                estado_actual = llenar_cisterna;
            }
        break;
                        
        default:                                                                  //Si cae en un caso que se escapa de los planteados
            bomba_off();                                                          //se prenden todos los leds y se apaga la bomba por precaución.
            gpio_set_mask((1 << LEDR_PIN) | (1 << LEDA_PIN) | (1 << LEDV_PIN));
        break;
        }
    }

}

