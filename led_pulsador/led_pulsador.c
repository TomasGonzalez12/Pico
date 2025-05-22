#include <stdio.h>
#include "pico/stdlib.h"

#define LED 22
#define BOT 15

int main()
{
    //Configuración
    stdio_init_all();

    gpio_init_mask(1 << LED | 1 << BOT);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_set_dir(BOT, GPIO_IN);
    gpio_pull_up(BOT);

    //Condicional

    bool flag = 0;
    
    while(true){
        if((flag == 0) && (gpio_get(BOT)== 0)){
            sleep_ms(20);
            if(gpio_get(BOT) == 0){
                gpio_xor_mask(1 << LED);
                flag = 1;
            }
        }

        if(gpio_get(BOT)== 1){
            flag = 0;
        }
    }  
}
