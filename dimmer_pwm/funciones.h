#ifndef FUNCIONES_H
#define FUNCIONES_H

typedef enum estados{
    parp_off,
    parp_on
}estados_t;

extern uint16_t level;
extern uint slice_num;

extern volatile uint8_t boton_presionado;
void puls_callback(uint gpio, uint32_t event_mask); 

void init_config(void);

#endif