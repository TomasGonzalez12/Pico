#ifndef FUNCIONES_H
#define FUNCIONES_H

//Variables globales
extern volatile bool puls_presionado;

typedef struct{
    uint8_t ledR_on   :1;
    uint8_t ledA_on   :1;
    uint8_t ledV_on   :1;
    uint8_t reservado :5;
}flags_s;

extern flags_s flag;

//Funciones globales
void init_config(void);
void reset_led_flags(void);
void puls_callback(uint gpio, uint32_t event_mask);

#endif 