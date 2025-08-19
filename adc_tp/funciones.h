#ifndef FUNCIONES_H
#define FUNCIONES_H

//Antirebote del pulsador
void puls_callback(uint gpio, uint32_t event_mask);        //Antirebote
void puls_bomba_callback(uint gpio, uint32_t event_mask);  //Apaga la bomba en caso de emergencia

// Prototipo de las funciones
void init_hardware();                                  //Inicializaciones
void bomba_off();                                      //Apaga/prende la bomba y el led verde
void bomba_on();                                       
void init_adc(uint32_t* tanque, uint32_t* cisterna);   //Primer medicion de adcs, sin promediar                                   
void valor_adc_tanque(uint32_t* tanque);               //Lectura de los valores entregados por adc tanque
void valor_adc_cisterna(uint32_t* cistern);            
uint32_t get_cont_tanque();                            //Getters para usar la variable static contador en main
uint32_t get_cont_cis();                          
void led_amarillo(uint32_t* tanque);                   //Evalua si prender los led dependiendo el nivel de los recipientes
void led_rojo(uint32_t* cisterna);                     

#endif
