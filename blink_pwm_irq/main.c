/*
Hace parpadear un led a frecuancias entre 1 Hz y 8Hz usando irq (por wrap), y frecuancias mayores usando la formula (f_pwm = f_count/div*wrap+1)
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "funciones.h"

int main() {
    init_config();

    printf("-- Presione el botón para elegir frecuencia --\n\n");
    printf("-- Para cambiar frecuencia vuelva a presionar el botón --\n\n");
    
    while(1);
}

