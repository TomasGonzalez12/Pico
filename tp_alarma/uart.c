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

//Variables globales
mensaje_rx_t mensaje_rx;
volatile uint32_t actual_time_puls1, actual_time_puls2, actual_time_led;
char mensaje_tx[24];

void fx_uart() {
    
    if ((actual_time_puls1) && (actual_time_puls1 < to_ms_since_boot(get_absolute_time()))) {
        actual_time_puls1 = 0;
        if (!gpio_get(PULS1_PIN)) {
            gpio_put(LED1_PIN, !(gpio_get(LED1_PIN)));
            /* Envía un mensaje fijo */
            uart_puts(UART_ID, "Se presiono el pulsador 1\r\n");
            /* Compone una respuesta y envía */
            sprintf(mensaje_tx, "El LED1 esta %s\r\n", (gpio_get(LED1_PIN) ? "ON" : "OFF"));
            uart_puts(UART_ID, mensaje_tx);
        }
    }
    if ((actual_time_puls2) && (actual_time_puls2 < to_ms_since_boot(get_absolute_time()))) {
        actual_time_puls2 = 0;
        if (!gpio_get(PULS2_PIN)) {
            gpio_put(LED2_PIN, !(gpio_get(LED2_PIN)));
            /* Envía un mensaje fijo */
            uart_puts(UART_ID, "Se presiono el pulsador 2\r\n");
            /* Compone una respuesta y envía */
            sprintf(mensaje_tx, "El LED2 está %s\r\n", (gpio_get(LED2_PIN) ? "ON" : "OFF"));
            uart_puts(UART_ID, mensaje_tx);
        }
    }
    if (mensaje_rx.rx_ok == true) {
        mensaje_rx.rx_ok = false;
        /* Analiza mensaje OK y ejecuta */
        if (strcmp((const char*)(MSG_ENCENDER), (const char*)(mensaje_rx.payload)) == 0) {
            /* PayloaON" */
            if (mensaje_rx.codigo == '1') {
                /* LED 1 */
                gpio_put(LED1_PIN, 1);
            } else if (mensaje_rx.codigo == '2') {
                /* LED 2 */
                gpio_put(LED2_PIN, 1);
            }
        } else if (strcmp((const char*)(MSG_APAGAR), (const char*)(mensaje_rx.payload)) == 0) {
            /* Payload es "OFF" */
            if (mensaje_rx.codigo == '1') {
                /* LED 1 */
                gpio_put(LED1_PIN, 0);
            } else if (mensaje_rx.codigo == '2') {
                /* LED 2 */
                gpio_put(LED2_PIN, 0);
            }
        }
    }
    
}

void isr_rx_uart1(void) {
    while (uart_is_readable(UART_ID)) {
        analiza_rx(uart_getc(UART_ID), &mensaje_rx);
    }
}

void analiza_rx(char rx_byte, mensaje_rx_t* destino) {
    /* Aquí escribo la máquina de estado del receptor de UART */
    /* Si recibe un mensaje pone en la estructura destino rx_ok en TRUE */
    /* Mientras recibe o no es correcto queda FALSE en rx_ok */
    static uint8_t state = CODIGO;
    static uint8_t cont_payload = 0;

    if (destino->rx_ok == false) {
        /* Solo analizo RX si no hay mensajes pendientes */
        switch (state) {
        case CODIGO:
            if ((rx_byte == '1') || (rx_byte == '2')) {
                destino->codigo = rx_byte;
                cont_payload = 0;
                state = ESPACIO;
            }
            break;
        case ESPACIO:
            if (rx_byte == ' ')
                state = PAYLOAD;
            else {
                destino->codigo = 0;
                destino->rx_ok = false;
                state = CODIGO;
            }
            break;
        case PAYLOAD:
            if ((rx_byte != '\r') && (rx_byte != '\n')) {
                if (cont_payload >= (LEN_MAX_PAYLOAD - 1)) {
                    state = CODIGO;
                    destino->rx_ok = false;
                } else {
                    destino->payload[cont_payload] = rx_byte;
                    cont_payload++;
                }
            } else
                state = ENTER;
            break;
        case ENTER:
            if ((rx_byte == '\r') || (rx_byte == '\n')) {
                destino->payload[cont_payload] = 0; //NULL finaliza string en payload
                destino->rx_ok = true;
            }
            state = CODIGO;
            break;

        default:
            break;
        }
    }
}



