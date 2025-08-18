#ifndef HARDWARE_H
#define HARDWARE_H

#define LEDR_PIN 11
#define LEDA_PIN 12
#define LEDV_PIN 13

#define FLOT_BAJO_PIN 27
#define FLOT_ALTO_PIN 26
#define PULS_PIN 18
#define CTRL_BOMBA 16

#define nivel_max_sup x
#define nivel_min_sup y
#define cap_sup (nivel_max_sup - nivel_min_sup)
#define level_ledA_on (nivel_min_sup + cap_sup/4)
#define level_ledA_off (nivel_min_sup + cap_sup * 3/4)


#endif 
