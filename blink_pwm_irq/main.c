/*
La salida es un pwm con un rango de frecuencias de  1Hz - 36.6kHz, que el usuario puede elegir por consola.
Para frecuancias entre 1 Hz y 143Hz implementa una irq (por wrap), 
y entre 144Hz a 36.6k por formula (f_pwm = f_count/div*wrap+1).
El valor máximo está limitado por la resolución del adc (4096) y el valor máximo de div (255).
Para variar el brillo en frecuencias bajas (1 - 143 Hz) con el potenciometro solo se puede ajustando el 
nivel primero y luego presionar el boton para ajustar la nueva frecuencia. Para frecuencias mayores se puede usar el
potenciometro en ejecucion como un dimmer.
*/

#include <stdio.h>
#include <inttypes.h> 
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "definiciones.h"
#include "funciones.h"
#include "systick.h"

estados_t estado_actual = nulo;

int main() {
    init_config();

    printf("-- Presione el botón para elegir frecuencia --\n\n");
    printf("-- Para cambiar frecuencia vuelva a presionar el botón --\n\n");

    while(1){
        if(flag.puls_presionado){
            reset_var_puls();
            flag.config_irq_frec_ok = false;
            flag.config_formula_frec_ok = false;
            pwm_set_enabled(slice_num, false);
            pwm_clear_irq(slice_num);
            ingresar_frec_input();
            pwm_set_enabled(slice_num, true);                  
        }

        switch (estado_actual){
            case irq_frec:
                if(!flag.config_irq_frec_ok){
                    config_irq_frec();
                }
            break;
            
            case formula_frec:
                if(!flag.config_formula_frec_ok){
                    config_formula_frec();
                }
            break;
        }

        if(flag.config_irq_frec_ok){
            if(led.f_pwm > 0 && led.f_input > 0 && led.f_input < 144){
                if (wrap_cont < led.duty) {
                    level = 0;
                    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
                }else if(wrap_cont >= led.duty && wrap_cont < led.num_wraps){
                    level = WRAP;
                    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
                }else{
                    level = 0;
                    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
                    wrap_cont = 0;
                }   
            } 
        }

        if(flag.config_formula_frec_ok){
            level = adc_read();
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            led.duty = ((level * 100)/WRAP); // Duty expresado en numero de wraps
            printf("Duty: %"PRIu32"%%\n", led.duty);
        }

    }
}

