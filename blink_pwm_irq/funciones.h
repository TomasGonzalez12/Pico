#ifndef FUNCIONES_H
#define FUNCIONES_H

//Estados
typedef enum{
    nulo,
    irq_frec,
    formula_frec
}estados_t;

extern estados_t estado_actual;

//Encabezados de funciones
void init_config(void);
void puls_callback(uint gpio, uint32_t event_mask);
void ingresar_frec_input(void);
void selc_frec_trabajo(void);
void on_pwm_wrap(void);

#endif