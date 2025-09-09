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
    regre
}estados_e;
estados_e estado_actual = inicio;

uint32_t cont = 0;
uint32_t ventanaP1 = 0;
bool f_blink = false;

int main()
{
    init_config();
    
    //Maquina de estados
    while(1){
        switch (estado_actual)
        {
        case inicio:
            cont = 0;
            ventanaP1 = 0;
            gpio_clr_mask(bcd_mask);
            estado_actual = progre;
        break;

        case progre:
            if(f_reset){
                f_reset = false;
                estado_actual = inicio;
            }

            if(f_blink){
                if(get_systick() - ventanaP1 >= SLEEP){
                    gpio_put(BUZZER, 0);
                    f_blink = false;
                }    
            }

            if(f_puls1){
                f_puls1 = false;
                if(cont >= 0 && cont <= 9 && !f_reset){
                    gpio_put(BUZZER, 1);
                    display_digito(&cont);
                    ventanaP1 = get_systick();
                    f_blink = true;
                    cont++;

                    if(cont > 0 && cont > 9 && !f_reset){  
                        cont = 8;
                        estado_actual = regre;
                    }
                }
            }
            

        break;

        case regre:
            if(f_reset){
                f_reset = false;
                estado_actual = inicio;
            }

            if(f_blink){
                if(get_systick() - ventanaP1 >= SLEEP){
                    gpio_put(BUZZER, 0);
                    f_blink = false;
                }    
            }

            if(f_puls1){
                f_puls1 = false;
                if(cont >= 0 && cont <= 9 && !f_reset){
                    gpio_put(BUZZER, 1);
                    display_digito(&cont);
                    ventanaP1 = get_systick();
                    f_blink = true;
                    cont--;

                    if(cont == 0 && !f_reset){
                        estado_actual = progre;
                    }
                }
            }
        break;
        }
    }
    
}