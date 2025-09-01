#include <stdio.h>
#include <math.h>
#include <inttypes.h> 
#include <string.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "hardware/pwm.h"
#include "definiciones.h"
#include "funciones.h"


//Variables globales a funciones.c
uint32_t level = 0;
uint slice_num = 0;

volatile uint8_t entrada_valida = 0; //flag para validar entrada de ingresar_fled()

uint32_t f_pwm = 0;//Hz
uint32_t f_led = 0; //HZ
float div  = 100;
estados_t estado_actual = nulo;
float num_wraps = 0;  // Número de wraps por ciclo
uint8_t t_off = 0; // 50% duty
uint8_t t_on = 0; 
volatile uint32_t wrap_cont = 0;

volatile uint32_t demora = 0;
volatile uint8_t flab_puls = 0;

//Configuraciones generales
void init_config(){
    stdio_init_all();

    //Boton
    gpio_init(PULS_PIN);
    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN);

    gpio_set_irq_enabled_with_callback(PULS_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);

    // Configuración de PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LED_PIN);

    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);

    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, div);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    pwm_set_enabled(slice_num, false);
}

//Leer valor de f_led ingresado por el usuario y validarlo ¿
void ingresar_fled(){
    char buffer[5];
    uint8_t entrada;

    entrada_valida = 0;

    while (entrada_valida == 0) {
        printf("---Ingrese un valor de frecuencia (1 - 8)[Hz]---\n");

        // Leer línea de entrada
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Validar si es un entero limpio (sin caracteres extra)
            char extra;
            if (sscanf(buffer, "%" SCNu8 " %c", &entrada, &extra) == 1) {
                if (entrada >= 1 && entrada <= 8){
                    f_led = entrada;
                    printf("El valor ingresado es %"PRIu8"\n", f_led);
                    estado_actual = irq_frec;
                    entrada_valida = 1;
                }else if(entrada >= 9){
                    f_led = entrada;
                    printf("El valor ingresado es %"PRIu8"\n", f_led);
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

//Seleccion de estado segun f_led
void selc_frec_trabajo(){
    
    switch (estado_actual)
    {
        case irq_frec:
            f_pwm = 100;
            div = ((float)F_COUNTER / (f_pwm * (WRAP + 1)));
            pwm_set_clkdiv(slice_num, div);
            num_wraps = ((float)f_pwm /f_led);  // wraps por ciclo
            t_off  = (num_wraps / 2.0f);   // 50% duty
            t_on = num_wraps;
            irq_set_enabled(PWM_IRQ_WRAP, true);
        break;
        
        case formula_frec:
            irq_set_enabled(PWM_IRQ_WRAP, false);
            f_pwm = f_led;
            div = ((float)F_COUNTER / (f_pwm * (WRAP + 1)));
            level = WRAP/2;
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
        ingresar_fled();
        selc_frec_trabajo();
        sleep_ms(100);
        pwm_set_enabled(slice_num, true);
    }
}

void accion_puls(){
    if(demora <= get_systick()){
        if(gpio_get(PULS_PIN) == 0){
            pwm_set_enabled(slice_num, false);
            pwm_clear_irq(slice_num);
            ingresar_fled();
            selc_frec_trabajo();
            sleep_ms(100);
            pwm_set_enabled(slice_num, true);
        }
    }
}

//Interrupcion PWM
void on_pwm_wrap(){
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(slice_num);
    
    wrap_cont++; 

    if(f_pwm > 0 && f_led > 0 && f_led < 9){
        if (wrap_cont < t_off) {
            if (level == 1) {
                level = 0;  
                pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            }
        }
        else if (wrap_cont >= t_off && wrap_cont < t_on) {
            if (level == 0) {
                level = WRAP; 
                pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
            }
        }
        else {
            wrap_cont = 0; 
            level = 0;
            pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
        }
    }    
}