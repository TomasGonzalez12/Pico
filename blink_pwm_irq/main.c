/*
Genera un pwm con un rango de frecuencias de  1Hz - 36.6kHz, que el usuario puede elegir por consola.
Para frecuancias entre 1 Hz y 144Hz implementa una irq (por wrap), 
y entre 154Hz a 36.6k por formula (f_pwm = f_count/div*wrap+1).
El valor máximo está lumitado por la resolucion del adc (4096) y el valor maximo de div (255).
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

