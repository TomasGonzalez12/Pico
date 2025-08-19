#ifndef FUNCIONES_H
#define FUNCIONES_H

//Variables
extern uint32_t band_prom_tanq, band_prom_cis;

//Antirebote del pulsador
void puls_callback(uint gpio, uint32_t event_mask);        //Antirebote y apagado de bomba

// Prototipo de las funciones
void init_hardware();                                  //Inicializaciones
void bomba_off();                                      //Apaga/prende la bomba y el led verde
void bomba_on();                                                                        
void valor_adc_tanque(uint32_t* tanque);               //Lectura de los valores entregados por adc tanque
void valor_adc_cisterna(uint32_t* cistern);                                     
void led_amarillo(uint32_t* tanque);                   //Evalua si prender los led dependiendo el nivel de los recipientes
void led_rojo(uint32_t* cisterna);                     

#endif
