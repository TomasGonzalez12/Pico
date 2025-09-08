#ifndef FUNCIONES_H
#define FUNCIONES_H

//Variables
typedef struct{
    uint8_t ledR_on          :1;
    uint8_t ledA_on          :1;
    uint8_t ledV_on          :1;
    uint8_t reservado        :5;
}flags_s;

extern flags_s flag;

extern volatile bool puls_presionado;

typedef enum{
    rojo_on,
    amarillo_on,
    verde_on,
    boton_pres
}estados_e;

extern estados_e estado_actual;
extern uint32_t lapso;

//Funciones globales
void init_config(void);
void reset_led_flags(void);
void puls_callback(uint gpio, uint32_t event_mask);

#endif 