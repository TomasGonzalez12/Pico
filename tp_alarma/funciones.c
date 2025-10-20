#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "uart.h"
#include "definiciones.h"
#include "funciones.h"
#include "perifericos.h"


void init_config(){
    stdio_init_all();

    //LEDS
    gpio_set_function(LED1_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, 0);
    gpio_set_function(LED2_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_put(LED2_PIN, 0);

    //PULSADORES
    gpio_set_function(PULS1_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(PULS1_PIN, GPIO_IN);
    gpio_pull_up(PULS1_PIN);
    gpio_set_input_hysteresis_enabled(PULS1_PIN, true);
    gpio_set_irq_enabled_with_callback(PULS1_PIN, GPIO_IRQ_EDGE_FALL, true, puls_callback);

    gpio_set_function(PULS2_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(PULS2_PIN, GPIO_IN);
    gpio_pull_up(PULS2_PIN);
    gpio_set_input_hysteresis_enabled(PULS2_PIN, true);
    gpio_set_irq_enabled(PULS2_PIN, GPIO_IRQ_EDGE_FALL, true);

    //UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    irq_set_exclusive_handler(UART1_IRQ, isr_rx_uart1);
    irq_set_enabled(UART1_IRQ, true);

    uart_set_irq_enables(UART_ID, true, false);

    //PWM
    uint slice_num;
    gpio_set_function(BUZZ_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(BUZZ_PIN);
    pwm_set_wrap(slice_num, WRAP);
    pwm_set_clkdiv(slice_num, DIV);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, LEVEL);
    pwm_set_enabled(slice_num, true);

    //ADC
    adc_init();
    adc_gpio_init(BAT_5V);
    adc_gpio_init(BAT_12V);
}

/* Callback de atención de interrupción */
boton_t btn_total   = { .id = PULS1_PIN, .f_pres = false }; //diria de cambiar el nombre a PULS_TOTAL
boton_t btn_parcial = { .id = PULS2_PIN, .f_pres = false }; //PULS_PARCIAL
//boton_t btn_panico  = { .id = PULS_PANICO, .f_pres = false };
//boton_t btn_aux     = { .id = PULS_AUX, .f_pres = false };

void puls_callback(uint gpio, uint32_t event_mask) {
    if ((gpio == PULS1_PIN) && (event_mask == GPIO_IRQ_EDGE_FALL)) {
        actual_time_puls1 = to_ms_since_boot(get_absolute_time()) + DEMORA;
    }
    if ((gpio == PULS2_PIN) && (event_mask == GPIO_IRQ_EDGE_FALL)) {
        actual_time_puls2 = to_ms_since_boot(get_absolute_time()) + DEMORA;
    }
    /*
     if ((gpio == PULS_PANICO) && (event_mask == GPIO_IRQ_EDGE_FALL)) {
        actual_time_puls2 = to_ms_since_boot(get_absolute_time()) + DEMORA;
    }
     if ((gpio == PULS_AUX) && (event_mask == GPIO_IRQ_EDGE_FALL)) {
        actual_time_puls2 = to_ms_since_boot(get_absolute_time()) + DEMORA;
    }    
    */
}

