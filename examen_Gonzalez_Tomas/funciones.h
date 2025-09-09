#ifndef FUNCIONES_H
#define FUNCIONES_H

//Variables globales
extern volatile bool f_puls1, f_puls2;
extern uint32_t cont;
extern uint32_t dato;
extern uint slice_num;

//Funciones
void init_config(void);
void puls_callback(uint gpio, uint32_t event_mask);
void display_digito(uint32_t* dato);

#endif