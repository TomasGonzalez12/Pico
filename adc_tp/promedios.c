/*
Funciones para sacar promedio de los valores del adc
*/

#include "hardware.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"
#include "promedios.h"

typedef struct{
    uint32_t tiempo;
    uint32_t cont;
    uint32_t acum;
    uint32_t prom;
    uint32_t nivel;
    uint8_t  canal_adc;
    bool     band_prom;
}deposito_t;

static deposito_t tanque = {.canal_adc = 0};
static deposito_t cisterna = {.canal_adc = 1};

void valor_adc_prom(deposito_t* d) {
    if (get_systick() >= d->tiempo) {
        d->tiempo = get_systick() + DELAY_MS;
        adc_select_input(d->canal_adc);
        d->acum += adc_read();
        d->cont++;
        d->band_prom = false;
        if (d->cont == MUESTRAS) {
            d->nivel = d->acum / d->cont;
            d->acum = 0;
            d->cont = 0;
            d->band_prom = true;
        }
    }
}

void get_valores(){
    valor_adc_prom(&cisterna);
    valor_adc_prom(&tanque);
}

uint32_t get_nivel_tanq(){
    return tanque.nivel;
}

uint32_t get_nivel_cis(){
    return cisterna.nivel;
}

bool get_band_prom(){
    if(!cisterna.band_prom && !tanque.band_prom){
        return false;
    }

    if(cisterna.band_prom && tanque.band_prom){
        return true;
    }
}

void reset_banderas(bool banderas){
    if(banderas){
        cisterna.band_prom = false;
        tanque.band_prom = false;
    }
    if(!banderas){
        cisterna.band_prom = true;
        tanque.band_prom = true; 
    }
}
