#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#define LED_PIN 16
#define POTE 26 //ADC0
#define PULS_PIN 15
#define DELAY_PULS 30

#define F_PWM 1 //KHz
#define DUTY 0.5f
#define F_COUNTER 150000000UL//150MHz
#define WRAP 4095 //12 bits resolucion (0 - 4095)
#define DIV ((float) F_COUNTER/(F_PWM * (WRAP + 1))) //2.29 (0 - 255)


#endif