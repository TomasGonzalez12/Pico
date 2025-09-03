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
#include "promedios.h"

//Estados 
typedef enum estado{
    inicio,
    llenar_tanque,
    llenar_cisterna
}estado_t;

int main()
{
    //Inicializaciones
    init_hardware();

    //Configuración del estado inicial del sistema
    bool mediciones_listas;
    estado_t estado_actual = inicio;
    uint32_t nivel_tanque, nivel_cisterna;
   
    //Máquina de estados
    while(1){
        switch (estado_actual)
        {
        case inicio:
            mediciones_listas = get_band_prom();
            if(!mediciones_listas){
                get_valores();
                estado_actual = inicio;
                break;
            }

            if(mediciones_listas){
                reset_banderas(mediciones_listas);
                nivel_tanque = get_nivel_tanq();
                nivel_cisterna = get_nivel_cis();

                if (nivel_cisterna <= nivel_bomba_on){
                    estado_actual = llenar_cisterna;
                }
                else if ((nivel_cisterna >= nivel_bomba_on) && (nivel_tanque <= nivel_min_tanq)){
                    estado_actual = llenar_tanque;
                }
                else{
                    estado_actual = llenar_cisterna;
                }
            }
        break;

        case llenar_cisterna:
            bomba_off();
            led_amarillo(nivel_tanque);
            led_rojo(nivel_cisterna);
            mediciones_listas = get_band_prom();

            if(!mediciones_listas){
                get_valores();
                estado_actual = llenar_cisterna;
                break;
            }

            if(mediciones_listas){
                reset_banderas(mediciones_listas);
                nivel_tanque = get_nivel_tanq();
                nivel_cisterna = get_nivel_cis();

                if(nivel_cisterna < nivel_bomba_on && nivel_tanque >= nivel_max_tanq){
                    estado_actual = llenar_cisterna;
                }
                else if (nivel_cisterna >= nivel_bomba_on && (nivel_tanque <= nivel_min_tanq) )
                {
                    estado_actual  = llenar_tanque;
                }
            }      
        break;

        case llenar_tanque:
            bomba_on();
            led_amarillo(nivel_tanque);
            led_rojo(nivel_cisterna);
            mediciones_listas = get_band_prom();

            if(!mediciones_listas){
                get_valores();
                estado_actual = llenar_tanque;
                break;
            }

            if(mediciones_listas){
                reset_banderas(mediciones_listas);
                nivel_tanque = get_nivel_tanq();
                nivel_cisterna = get_nivel_cis();

                if((nivel_cisterna >= nivel_bomba_on) && (nivel_tanque <= nivel_min_tanq)){
                    estado_actual = llenar_tanque;
                }
                else if(nivel_cisterna <= nivel_bomba_on && nivel_tanque >= nivel_max_tanq){
                    estado_actual = llenar_cisterna;
                }
            }
        break;
                        
        default:                                                                  //Si cae en un caso que se escapa de los planteados
            bomba_off();                                                          //se prenden todos los leds y se apaga la bomba por precaución.
            gpio_set_mask((1 << LEDR_PIN) | (1 << LEDA_PIN) | (1 << LEDV_PIN));
        break;
        }
    }

}

