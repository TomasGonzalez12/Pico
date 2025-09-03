#ifndef FUNCIONES_H
#define FUNCIONES_H

//Antirebote del pulsador
void puls_callback(uint gpio, uint32_t event_mask);        //Antirebote y apagado de bomba

// Prototipo de las funciones
void init_hardware();                                  //Inicializaciones
void bomba_off();                                      //Apaga/prende la bomba y el led verde
void bomba_on();                                                                                                             
void led_amarillo(uint32_t tanque);                   //Evalua si prender los led dependiendo el nivel de los recipientes
void led_rojo(uint32_t cisterna);                     

#endif
