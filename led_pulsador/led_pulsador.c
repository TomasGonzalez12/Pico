#include <stdio.h>
#include "pico/stdlib.h"

#define LED 22
#define BT 15

typedef enum estado_BT{
    presionado_on,
    presionado_off,
    suelto_on,
    suelto_off
}estado_BT;

typedef enum estado_Led{
    on,
    off,
    parpadeo
}estado_Led;


int main(){
    //Inicialización
    stdio_init_all();

    gpio_init_mask(1 << LED | 1 << BT);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 0);
    gpio_set_dir(BT, GPIO_IN);
    gpio_pull_up(BT);
    gpio_set_input_hysteresis_enabled(LED_PIN, true);

    //Secuencia
     estado_BT estado_actual = suelto_off;
     estado_Led estado_Led_actual = off;

     while(1){
     switch (estado_actual)
     {
        case suelto_off:
            if(gpio_get(BT) == 1)
            {
                gpio_put(LED, 0);
                estado_actual = suelto_off;
                estado_Led_actual = off;
            } 
            else if (gpio_get(BT) == 0)
            {
                gpio_put(LED, 1);
                estado_actual = presionado_on;
                estado_Led_actual = on;
            }  
        break;
     
        case presionado_on:
             if(gpio_get(BT) == 0)
            {
                gpio_put(LED, 1);
                estado_actual = presionado_on;
                estado_Led_actual = on;
            } 
            else if (gpio_get(BT) == 1)
            {
                gpio_put(LED, 1);
                estado_actual = suelto_on;
                estado_Led_actual = on;
            }  
        break;

         case suelto_on:
             if(gpio_get(BT) == 1)
            {
                gpio_put(LED, 1);
                estado_actual = suelto_on;
                estado_Led_actual = on;
            } 
            else if (gpio_get(BT) == 0)
            {
                gpio_put(LED, 0);
                estado_actual = presionado_off;
                estado_Led_actual = off;
            }  
        break;

        case presionado_off:
             if(gpio_get(BT) == 0)
            {
                gpio_put(LED, 0);
                estado_actual = presionado_off;
                estado_Led_actual = off;
            } 
            else if (gpio_get(BT) == 1)
            {
                gpio_put(LED, 0);
                estado_actual = suelto_off;
                estado_Led_actual = off;
            }  
        break;

     default:
        break;
     }
}
    
}
