#ifndef HARDWARE_H
#define HARDWARE_H

#define REBOTE_PULS 30

#define LEDR_PIN 11
#define LEDA_PIN 12
#define LEDV_PIN 13

#define PULS_PIN 18
#define CTRL_BOMBA 16
#define FLOT_BAJO_PIN 27 // ADC1 Cisterna
#define FLOT_ALTO_PIN 26 //ADC0 Tanque
#define MUESTRAS 10
#define delay_ms 60

// Tanque
#define nivel_max_sup 1740
#define nivel_min_sup 465
#define cap_sup (nivel_max_sup - nivel_min_sup)
#define nivel_medio_sup (nivel_min_sup + cap_sup/2)
#define level_ledA_on (nivel_min_sup + cap_sup/4)
#define level_ledA_off (nivel_min_sup + cap_sup * 3/4)

// Cisterna
#define nivel_max_inf 2040
#define nivel_min_inf 707
#define cap_inf (nivel_max_inf - nivel_min_inf)
#define level_ledR_on (nivel_min_inf + cap_inf/4)
#define level_ledR_off (nivel_min_inf + cap_inf * 3/4)
#define level_bomba_on (nivel_min_inf + cap_inf/2)


#endif 
 