#ifndef PROMEDIOS_H
#define PROMEDIOS_H

//Funciones globales
void get_valores();
uint32_t get_nivel_tanq(void);
uint32_t get_nivel_cis(void);
bool get_band_prom(void);                              //True cuando ambas banderas estan listas
void reset_banderas(bool banderas);

#endif