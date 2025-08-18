#ifndef FUNCIONES_H
#define FUNCIONES_H

//Antirebote del pulsador
extern volatile uint32_t demora;
void puls_callback(uint gpio, uint32_t event_mask);

//variables para promediar el adc
extern uint32_t tiempo;
extern uint32_t acum_tanque, cont_tanque, prom_tanque;
extern uint32_t acum_cis, cont_cis, prom_cis;

// Prototipo de las funciones
void init_hardware();                                  //Inicializaciones
void puls_bomba();                                     //Apaga la bomba en caso de emergencia
void bomba_off();                                      //Apaga la bomba y el led verde
void bomba_on();                                       //Prende la bomba y el led verde
void valor_adc_tanque(uint32_t* tanque);               //Lectura de los valores entregados por adc tanque
void valor_adc_cisterna(uint32_t* cistern);            //Lectura de los valores entregados por adc cisterna
void led_amarillo(uint32_t* tanque);                   //Evalua si prender el led amarillo dependiendo el nivel del tanque
void led_rojo(uint32_t* cisterna);                     //Evalua si prender el led rojo dependiendo del nivel de la cisterna

#endif
