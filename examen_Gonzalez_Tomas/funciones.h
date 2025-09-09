#ifndef FUNCIONES_H
#define FUNCIONES_H

//Variables globales
extern uint32_t bcd_mask;
extern volatile bool f_puls1, f_reset;
extern uint32_t cont;
extern uint32_t ventanaP1;
extern bool f_blink;
extern uint slice_num;

//Funciones
void init_config(void);
void puls_callback(uint gpio, uint32_t event_mask);
void display_digito(uint32_t* dato);

#endif