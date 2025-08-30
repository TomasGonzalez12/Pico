#ifndef DEFINICIONES_H
#define DEFINICIONES_H 

#define LED_PIN 16

#define F_PWM 100 // Hz
#define DUTY 0.5f

#define F_COUNTER 150000000 // 150MHz
#define WRAP 65535 // 16 bits resolucion (0 - 65535)
#define DIV ((float)F_COUNTER / (F_PWM * (WRAP + 1))) //22.9 (0 - 255)

#define F_LED 2 // Hz
#define num_wraps ((float)F_PWM / F_LED)  // Número de wraps por ciclo
#define t_off (uint8_t)roundf((num_wraps / 2.0f))      // 50% duty
#define t_on (uint8_t)num_wraps                      

#endif