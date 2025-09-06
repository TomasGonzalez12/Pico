#ifndef FUNCIONES_H
#define FUNCIONES_H

typedef enum estados{
    led_off,
    led_on
}estados_t;

extern uint16_t level;
extern uint slice_num;

extern volatile bool f_puls;
void puls_callback(uint gpio, uint32_t event_mask); 

void init_config(void);

#endif