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


//Variables
volatile flags_t flag = {0};

led_t led = {
    .f_pwm = 1,//Hz
    .f_input = 1, //HZ
    .duty = 0,
    .num_wraps = 0,  // Número de wraps por ciclo
    .t_off = 0, // 50% duty
    .t_on = 0
};

uint slice_num = 0;
uint32_t level = 0;
float div  = 1000;


//Configuraciones generales
void init_config(){
    stdio_init_all();

    //Boton
    gpio_init(PULS_PIN);
    gpio_set_dir(PULS_PIN, GPIO_IN);
    gpio_pull_up(PULS_PIN);
    gpio_set_input_hysteresis_enabled(LED_PIN, true);

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
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pow_wrap_cont);

    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, div);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    pwm_set_enabled(slice_num, false);
}

//Leer valor de f_input ingresado por el usuario y validarlo
void ingresar_frec_input(){
    char buffer[20];

    flag.entrada_valida = false;

    while (!flag.entrada_valida) {
        printf("---Ingrese un valor de frecuencia (1 - 36600)[Hz]---\n");

        // Leer línea de entrada
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Validar si es un entero limpio (sin caracteres extra)
            char extra;
            if (sscanf(buffer, "%" SCNu32 " %c", &led.f_input, &extra) == 1) {
                if (led.f_input >= 1 && led.f_input <= 143){
                    printf("El valor ingresado es %"PRIu32"\n", led.f_input);
                    estado_actual = irq_frec;
                    flag.entrada_valida = 1;
                }else if(led.f_input >= 144 && led.f_input <= 36600){
                    printf("El valor ingresado es %"PRIu32"\n", led.f_input);
                    estado_actual = formula_frec;
                    flag.entrada_valida = 1;       
                }else{
                    printf("Valor inválido.\n");
                }
            }else{
                printf("Error al leer entrada.\n");
            }
        }
    }    
}

//configuración del estado irq_frec
void config_irq_frec(){
    led.f_pwm = 1000; //Valor fijo, para asegurar que el valor de div de entre el rango valido (0 -255), que no haya redondeos que den 0, etc
    div = ((float)F_COUNTER / (led.f_pwm * (WRAP + 1)));
    pwm_set_clkdiv(slice_num, div);
    led.num_wraps = (uint16_t)roundf(((float)led.f_pwm /led.f_input)); // wraps por ciclo
    level = adc_read();
    led.duty = ((level * led.num_wraps)/WRAP); // Duty expresado en numero de wraps
    printf("Duty: %"PRIu32"%%\n", ((led.duty*100)/led.num_wraps));
    level = WRAP; // Asi arranca apagado;
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    flag.config_irq_frec_ok = true;
}

//configuración del estado fomrula_frec
void config_formula_frec(){
    irq_set_enabled(PWM_IRQ_WRAP, false);
    led.f_pwm = led.f_input;
    div = ((float)F_COUNTER / (led.f_pwm * (WRAP + 1)));
    level = adc_read();
    led.duty = ((level * 100)/WRAP);
    printf("Duty: %"PRIu32"%%\n", led.duty);
    pwm_set_clkdiv(slice_num, div);
    pwm_set_wrap(slice_num, WRAP);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    flag.config_formula_frec_ok = true;
}

//Callback boton
volatile uint32_t demora = 0;

void puls_callback(uint gpio, uint32_t event_mask) {
    gpio_acknowledge_irq(gpio, event_mask); 
    if (gpio == PULS_PIN && event_mask == GPIO_IRQ_EDGE_FALL) {
        // Solo activar si pasó el tiempo de rebote
        if (get_systick() >= demora) {
            demora = get_systick() + DELAY_PULS;
            flag.puls_presionado = true;
        }
    }
}

//Interrupcion PWM
volatile uint32_t wrap_cont = 0;

void pow_wrap_cont(){
    pwm_clear_irq(slice_num);
    
    wrap_cont++;
    flag.wrap_irq = true;       
}