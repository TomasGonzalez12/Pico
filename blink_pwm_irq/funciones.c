#include <stdio.h>
#include <math.h>
#include <inttypes.h> 
#include <string.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "definiciones.h"
#include "funciones.h"


//Variables globales a funciones.c
uint8_t flag_input = 0;
uint32_t level = 0;
uint slice_num = 0;

volatile uint8_t entrada_valida = 0; //flag para validar entrada de ingresar_frec_input()

uint32_t f_pwm = 1;//Hz
uint32_t f_input = 1; //HZ
uint32_t duty = 0;
float div  = 1000;
estados_t estado_actual = nulo;
uint16_t num_wraps = 0;  // Número de wraps por ciclo
uint8_t t_off = 0; // 50% duty
uint8_t t_on = 0; 
volatile uint32_t wrap_cont = 0;

volatile uint32_t demora = 0;


//Configuraciones generales
void init_config(){
    stdio_init_all();

    //Boton
    gpio_init(PULS_PIN);
    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN);

    gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);

    //ADC
    adc_init();
    adc_gpio_init(POTE);
    adc_select_input(0);

    // Configuración de PWM
    gpio_set_function(input_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(input_PIN);

    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);

    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, div);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    pwm_set_enabled(slice_num, false);
}

//Leer valor de f_input ingresado por el usuario y validarlo
void ingresar_frec_input(){
    char buffer[10];

    entrada_valida = 0;

    while (entrada_valida == 0) {
        printf("---Ingrese un valor de frecuencia (1 - 8)[Hz]---\n");

        // Leer línea de entrada
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Validar si es un entero limpio (sin caracteres extra)
            char extra;
            if (sscanf(buffer, "%" SCNu8 " %c", &f_input, &extra) == 1) {
                if (f_input >= 1 && f_input <= 143){
                    printf("El valor ingresado es %"PRIu8"\n", f_input);
                    estado_actual = irq_frec;
                    entrada_valida = 1;
                }else if(f_input >= 144 && f_input <= 36600){
                    printf("El valor ingresado es %"PRIu8"\n", f_input);
                    estado_actual = formula_frec;
                    entrada_valida = 1;       
                }else{
                    printf("Valor inválido.\n");
                }
            }else{
                printf("Error al leer entrada.\n");
            }
        }
    }    
}

//Seleccion de estado segun f_input
void selc_frec_trabajo(){
    switch (estado_actual)
    {
        case irq_frec:
            f_pwm = 1000; //Valor fijo, para asegurar que el valor de div de entre el rango valido (0 -255), que no haya redondeos que den 0, etc
            div = ((float)F_COUNTER / (f_pwm * (WRAP + 1)));
            pwm_set_clkdiv(slice_num, div);
            num_wraps = (uint16_t)roundf(((float)f_pwm /f_input)); // wraps por ciclo
            level = adc_read();
            sleep_ms(100);
            duty = ((level * num_wraps)/WRAP); // Duty expresado en numero de wraps
            printf("Duty: %"PRIu16"%%\n", ((duty*100)/num_wraps));
            level = WRAP; // Asi arranca apagado;
            //pwm_set_output_polarity(slice_num, true, false);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            irq_set_enabled(PWM_IRQ_WRAP, true);
        break;
        
        case formula_frec:
            irq_set_enabled(PWM_IRQ_WRAP, false);
            f_pwm = f_input;
            div = ((float)F_COUNTER / (f_pwm * (WRAP + 1)));
            level = adc_read();
            pwm_set_clkdiv(slice_num, div);
            pwm_set_wrap(slice_num, WRAP);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
        break;
        
        default:
        break;
    }    
}

//Callback boton
void puls_callback(uint gpio, uint32_t event_mask) {
    gpio_acknowledge_irq(gpio, event_mask); 
    if (gpio == PULS_PIN && event_mask == GPIO_IRQ_EDGE_FALL){
        pwm_set_enabled(slice_num, false);
        pwm_clear_irq(slice_num);
        ingresar_frec_input();
        selc_frec_trabajo();
        sleep_ms(100);
        pwm_set_enabled(slice_num, true);
    }
}

//Interrupcion PWM
void on_pwm_wrap(){
    pwm_clear_irq(slice_num);
    
    wrap_cont++; 

    if(f_pwm > 0 && f_input > 0 && f_input < 143){
        if (wrap_cont < duty) {
            level = 0;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
        }else if(wrap_cont >= duty && wrap_cont < num_wraps){
            level = WRAP;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
        }else{
            level = 0;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            wrap_cont = 0;
        }   
    }    
}