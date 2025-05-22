#include <stdio.h>
#include "pico/stdlib.h"

#define LED 22
#define BOT 15

//bool flag = 0;

void parpadeoLed(uint gpio, int t_ms);

int main()
{
    //Configuración
    stdio_init_all();

    gpio_init_mask(1 << LED | 1 << BOT);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_set_dir(BOT, GPIO_IN);
    gpio_pull_up(BOT);

    //Secuencia
    parpadeoLed(LED, 167); 
    
}

void parpadeoLed(uint gpio, int t_ms){
    while (true) {
        gpio_xor_mask(1 << gpio);
        sleep_ms(t_ms);
    }
}