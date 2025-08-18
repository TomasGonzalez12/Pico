#ifndef FUNCIONES_H
#define FUNCIONES_H

//Antirebote del pulsador
extern volatile uint32_t demora;
void puls_callback(uint gpio, uint32_t event_mask);

// Prototipo de las funciones
void init_hardware();                                  //Inicializaciones
void puls_bomba();                                     //Apaga la bomba en caso de emergencia
void bomba_off();                                      //Apaga la bomba y el led verde
void bomba_on();                                       //Prende la bomba y el led verde
void init_adc(uint32_t* tanque, uint32_t* cisterna);   //Primer medicion de adcs, sin promediar                                   
void valor_adc_tanque(uint32_t* tanque);               //Lectura de los valores entregados por adc tanque
void valor_adc_cisterna(uint32_t* cistern);            //Lectura de los valores entregados por adc cisterna
void led_amarillo(uint32_t* tanque);                   //Evalua si prender el led amarillo dependiendo el nivel del tanque
void led_rojo(uint32_t* cisterna);                     //Evalua si prender el led rojo dependiendo del nivel de la cisterna

#endif
