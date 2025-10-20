#ifndef FUNCIONES_H
#define FUNCIONES_H

extern void init_config(void);

//UART
/* Estos son los callbacks de GPIO y RX UART */
extern void puls_callback(uint, uint32_t);
extern void isr_rx_uart1(void);
/* Esta función es la máquina de estado para analizar mensaje recibido */
extern void analiza_rx(char, mensaje_rx_t*);
extern void fx_uart(void);

#endif