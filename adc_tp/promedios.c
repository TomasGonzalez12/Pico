/*
Funciones para sacar promedio de los valores del adc
*/

#include "hardware.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"
#include "promedios.h"

//Variables locales
static uint32_t tiempo_tanq = 0, cont_tanq = 0, acum_tanq = 0, prom_tanq = 0;
static uint32_t tiempo_cis = 0, cont_cis = 0, acum_cis = 0, prom_cis = 0;
static bool band_prom_tanq = false, band_prom_cis = false;
static uint32_t nivel_tanq = 0, nivel_cis = 0;

void valor_adc_tanque(uint32_t* tanque) {
    if (get_systick() >= tiempo_tanq) {
        tiempo_tanq = get_systick() + DELAY_MS;
        adc_select_input(0);
        acum_tanq += adc_read();
        cont_tanq++;
        band_prom_tanq = false;
        if (cont_tanq == MUESTRAS) {
            *tanque = acum_tanq / cont_tanq;
            acum_tanq = 0;
            cont_tanq = 0;
            band_prom_tanq = true;
        }
    }
}

void valor_adc_cisterna(uint32_t* cisterna) {
    if (get_systick() >= tiempo_cis) {
        tiempo_cis = get_systick() + DELAY_MS;
        adc_select_input(1);
        acum_cis += adc_read();
        cont_cis++;
        band_prom_cis = false;
        if (cont_cis == MUESTRAS) {
            *cisterna = acum_cis / cont_cis;
            acum_cis = 0;
            cont_cis = 0;
            band_prom_cis = true;
        }
    }
}

void get_valores(){
    valor_adc_cisterna(&nivel_cis);
    valor_adc_tanque(&nivel_tanq);
}

bool get_band_prom(){
    if(!band_prom_cis && !band_prom_tanq){
        return false;
    }

    if(band_prom_cis && band_prom_tanq){
        return true;
    }
}

void reset_banderas(bool banderas){
    if(banderas){
        band_prom_cis = false;
        band_prom_tanq = false;
    }
    if(!banderas){
        band_prom_cis = true;
        band_prom_tanq = true; 
    }
}

uint32_t get_nivel_tanq(){
    return nivel_tanq;
}

uint32_t get_nivel_cis(){
    return nivel_cis;
}