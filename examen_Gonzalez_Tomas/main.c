#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "systick.h"
#include "definiciones.h"
#include "funciones.h"
#include "numeros.h"

typedef enum{
    inicio,
    progre,
    regre,
    boton_pres
}estados_s;


int main()
{
    init_config();
    uint slice_num = pwm_gpio_to_slice_num(BUZZER); //lo vuelvo a definir para salvar el error de compilacion que me da

    //Estado inicial
    uint32_t cont = 0;
    uint32_t lapso1, lapso2;
    uint32_t dato;
    estados_s estado_actual = inicio;
    
    //Maquina de estados
    while(1){
        switch (estado_actual)
        {
        case inicio:
            cont = 0;
            estado_actual = boton_pres;
            if(f_puls1){
                f_puls1 = false;
                estado_actual = boton_pres;
            }
        break;

        case boton_pres:
            if(f_puls1){
                if(lapso1 < SLEEP ){
                    pwm_set_enabled(slice_num, true);
                    lapso1++;
                    break;
                }
                f_puls1 = false;
                lapso1 = 0;
                pwm_set_enabled(slice_num, false);
                estado_actual= progre;
            }else if (f_puls2){
                if(lapso2 < SLEEP ){
                    pwm_set_enabled(slice_num, true);
                    lapso2++;
                    break;
                }
                f_puls2 = false;
                lapso2 = 0;
                pwm_set_enabled(slice_num, false);
                estado_actual = inicio;
            }
        break;

        case progre:
            if(f_puls1){
                if(cont > 0 && cont <= 9 && !f_puls2){
                    f_puls1 = false;
                    display_digito(&cont);
                    cont++;
                    estado_actual = progre;
            }
            else if(cont > 0 && cont == 9 && !f_puls2){
                f_puls1 = false;
                estado_actual = regre;
            }else if(f_puls2){
                f_puls2 = false;
                estado_actual = inicio;
            }
            }
            

        break;

        case regre:
            if(f_puls1){
                if(cont < 0 && cont == 9 && !f_puls2){
                f_puls1 = false;
                cont--;
                display_digito(&cont);
            }
            else if( cont == 0 && !f_puls2){
                estado_actual = progre;
            }else if(f_puls2){
                f_puls2 = false;
                estado_actual = inicio;
            }
            }
        break;
        
        default:
        break;
        }

    }
    
}