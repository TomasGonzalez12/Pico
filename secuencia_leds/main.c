/*
    Al presionar el boton prende tres leds (rojo, amarillo, verde) en secuencia a una frecuancia definida en SLEEP,
    presionando otra vez el botón se apaga.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "definiciones.h"
#include "funciones.h"

typedef enum{
    rojo_on,
    amarillo_on,
    verde_on,
    boton_pres
}estados_e;

int main()
{
    init_config();

    //Estado inicial
    estados_e estado_actual = boton_pres;

    //Máquina de estados
    while(1){
        switch (estado_actual)
        {
            case boton_pres:
                gpio_clr_mask(LED_MASK);
                reset_led_flags();
                if(puls_presionado){
                    puls_presionado = false;
                    estado_actual = rojo_on;
                    lapso = get_systick() + SLEEP;
                }
            break;

            case rojo_on:
                LED_RON;
                flag.ledR_on = true;

                if(get_systick() < lapso && flag.ledR_on && !puls_presionado){
                    estado_actual = rojo_on;
                }
                else if(puls_presionado){
                    puls_presionado = false;
                    estado_actual = boton_pres;
                }
                else{
                    LED_ROFF;
                    lapso += SLEEP;
                    estado_actual = amarillo_on;
                }
            break;

            case amarillo_on:
                LED_AON;
                flag.ledA_on = true;

                if(get_systick() < lapso && flag.ledA_on && !puls_presionado){
                    estado_actual = amarillo_on;
                }
                else if(puls_presionado){
                    puls_presionado = false;
                    estado_actual = boton_pres;
                }
                else{
                    LED_AOFF;
                    lapso += SLEEP;
                    estado_actual = verde_on;
                }
            break;

            case verde_on:
                LED_VON;
                flag.ledV_on = true;

                if(get_systick() < lapso && flag.ledV_on && !puls_presionado){
                    estado_actual = verde_on;
                }
                else if(puls_presionado){
                    puls_presionado = false;
                    estado_actual = boton_pres;
                }
                else{
                    LED_VOFF;
                    lapso += SLEEP;
                    estado_actual = rojo_on;
                }
            break;

        }


    }


}
