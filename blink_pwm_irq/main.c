/*
Hace parpadear un led a frecuancias entre 1 Hz y 8 HZ (por debajo de las que permite lograr por formula: f_count/div*wrap+1)
que es de 9 Hz para arriba.
Para esto usa interrupciones de pwm(por wrap) e interrupciones de boton para que el usuario pare la secuencua y cambie la frecuencia.
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

