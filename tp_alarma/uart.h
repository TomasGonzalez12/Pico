#ifndef UART_H
#define UART_H

extern volatile uint32_t actual_time_puls1, actual_time_puls2, actual_time_led;
/* En este array voy a armar un mensaje para enviar */
extern char mensaje_tx[24];

#define MSG_ENCENDER    ("ON")
#define MSG_APAGAR      ("OFF")

#define LEN_MAX_PAYLOAD (4)

typedef struct {
    bool rx_ok;
    uint8_t stx;
    int8_t id;
    uint8_t payload[LEN_MAX_PAYLOAD];
    uint8_t codigo;
    uint8_t etx;
    uint32_t chksum;
}mensaje_rx_t;


/*
    Los posibles mensajes para encender o apagar 2 leds:
    "1 OFF\r\n"
    "1 ON\r\n"
    "2 OFF\r\n"
    "2 ON\r\n"
*/

/* Estados */
enum {
    CODIGO,
    ESPACIO,
    PAYLOAD,
    ENTER,
};

#endif