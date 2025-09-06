#ifndef HARDWARE_H
#define HARDWARE_H

#define REBOTE_PULS 20

#define LEDR_PIN 11
#define LEDA_PIN 12
#define LEDV_PIN 13

#define PULS_PIN 18
#define CTRL_BOMBA 16
#define FLOT_CIS_PIN 27 // ADC1 Cisterna
#define FLOT_TANQ_PIN 26 //ADC0 Tanque
#define MUESTRAS 10
#define DELAY_MS 20

// Tanque
#define nivel_max_tanq 1740  //aca no va el dato en mV, sino el de la cuenta (mv*4096)/3.3
#define nivel_min_tanq 465
#define cap_tanq (nivel_max_tanq - nivel_min_tanq)
#define nivel_ledA_on (nivel_min_tanq + cap_tanq/4)
#define nivel_ledA_off (nivel_min_tanq + cap_tanq * 3/4)

// Cisterna
#define nivel_max_cis 2040
#define nivel_min_cis 707
#define cap_cis (nivel_max_cis - nivel_min_cis)
#define nivel_ledR_on (nivel_min_cis + cap_cis/4)
#define nivel_ledR_off (nivel_min_cis + cap_cis * 3/4)
#define nivel_bomba_on (nivel_min_cis + cap_cis/2)


#endif 
 