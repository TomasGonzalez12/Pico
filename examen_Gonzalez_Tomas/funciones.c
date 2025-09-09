#include <stdio.h>
#include "pico/stdlib.h"
#include "systick.h"
#include "definiciones.h"
#include "funciones.h"
#include "numeros.h"
#include "hardware/pwm.h"

//Configuraciones generales
void init_config(){
    stdio_init_all();
    init_systick();

    //Pines BCD
    uint32_t bcd_mask =((1 << A_PIN) | (1 << B_PIN) | (1 << C_PIN) | (1 << D_PIN));
    gpio_init_mask(bcd_mask);
    gpio_set_dir_out_masked(bcd_mask);
    gpio_clr_mask(bcd_mask);

    //Pulsadores
    uint32_t puls_mask = ((1 << PULS1_PIN) | (1 << PULS2_PIN));
    gpio_init_mask(puls_mask);
    gpio_set_dir_in_masked(puls_mask);
    gpio_pull_up(PULS1_PIN);
    gpio_pull_up(PULS2_PIN);
    gpio_set_input_hysteresis_enabled(PULS1_PIN, true);
    gpio_set_input_hysteresis_enabled(PULS2_PIN, true);

    gpio_set_irq_enabled_with_callback(PULS1_PIN,  GPIO_IRQ_EDGE_FALL, true, puls_callback);
    gpio_set_irq_enabled(PULS2_PIN, GPIO_IRQ_EDGE_FALL, true);

    //PWM
    gpio_set_function(BUZZER, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, DIV);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, LEVEL);
    pwm_set_enabled(slice_num, false);
    
}

//Aumentar el valor del display
void display_digito(uint32_t* dato){
    switch (*dato)
    {
        case 0:
            put_cero();
        break;

        case 1:
            put_uno();
        break;

        case 2:
            put_dos();
        break;

        case 3:
            put_tres();
        break;

        case 4:
            put_cuatro();
        break;

        case 5:
            put_cinco();
        break;

        case 6:
            put_seis();
        break;

        case 7:
            put_siete();
        break;

        case 8:
            put_ocho();
        break;

        case 9:
            put_nueve();
        break;
    }   
}

//Callback
volatile uint32_t ahora, ventana;
volatile bool f_puls1 = false, f_puls2 = false;

void puls_callback(uint gpio, uint32_t event_mask){
    ahora= get_systick();
    if(ahora <= ventana) return;
    ventana = ahora + DELAY_MS;

    if(gpio == PULS1_PIN && event_mask == GPIO_IRQ_EDGE_FALL){
        f_puls1 = true;
    }

     if(gpio == PULS2_PIN && event_mask == GPIO_IRQ_EDGE_FALL){
        f_puls2 = true;
    }
}

