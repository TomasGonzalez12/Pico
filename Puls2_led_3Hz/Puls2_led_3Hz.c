#include <stdio.h>
#include "pico/stdlib.h"

#define LED 22
#define BT 15
#define t_rebote 30

typedef enum estado{
    suelto_off,
    presionado_parp,
    suelto_parp,
    presionado_off
}estado;

//Estado inicial
estado estado_actual = suelto_off;

void ledParpadeo(uint gpio, int t_ms, estado estado);

int main(){
    //Inicialización
    stdio_init_all();

    gpio_init_mask(1 << LED | 1 << BT);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 0);
    gpio_set_dir(BT, GPIO_IN);
    gpio_pull_up(BT);

    //Secuencia
     while(1){
     switch (estado_actual)
     {
        case suelto_off:
            if(gpio_get(BT) == 1)
            {
                sleep_ms(t_rebote);
                if(gpio_get(BT) == 1)
                {
                    gpio_put(LED,0);
                    estado_actual = suelto_off;
                }
            }
            else if (gpio_get(BT) == 0)
            {
                sleep_ms(t_rebote);
                if(gpio_get(BT) == 0)
                {
                    estado_actual = presionado_parp;
                }
            }  
        break;

        case presionado_parp:
            if(gpio_get(BT) == 0)
            {
                ledParpadeo(LED, 167, presionado_parp);
                estado_actual = presionado_parp;
            }
            else if (gpio_get(BT) == 1)
            {
                sleep_ms(t_rebote);
                if(gpio_get(BT) == 1){
                    estado_actual = suelto_parp;
                }
            }
        break;

        case suelto_parp:
            if(gpio_get(BT) == 1)
            {
                ledParpadeo(LED, 167, suelto_parp);
                estado_actual = suelto_parp;
            }
            else if (gpio_get(BT) == 0)
            {
                sleep_ms(t_rebote);
                if (gpio_get(BT) == 0)
                {
                    estado_actual = presionado_off;
                }
            } 
        break;

        case presionado_off:
            if(gpio_get(BT) == 0)
            {
                gpio_put(LED, 0);
                estado_actual = presionado_off;
            }
            else if (gpio_get(BT) == 1)
            {
                sleep_ms(t_rebote);
                if(gpio_get(BT) == 1)
                {
                    estado_actual = suelto_off;
                }
            }   
        break;

        default:
        break;
     }
}
    
}


void ledParpadeo(uint gpio, int t_ms, estado estado){
    int flag = 1;
    while(((estado == presionado_parp) || (estado = suelto_parp)) && (flag ==1)){
        gpio_xor_mask(1 << LED);
        sleep_ms(167);
        flag = 0;
    }   
}
