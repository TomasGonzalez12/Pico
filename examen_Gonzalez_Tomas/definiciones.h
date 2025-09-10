#ifndef DEFINICIONES_H
#define DEFINICIONES_H

//BCD
#define A_PIN 16
#define B_PIN 18
#define C_PIN 20
#define D_PIN 21

#define AON gpio_put(A_PIN, 1)
#define BON gpio_put(B_PIN, 1)
#define CON gpio_put(C_PIN, 1)
#define DON gpio_put(D_PIN, 1)

#define AOFF gpio_put(A_PIN, 0)
#define BOFF gpio_put(B_PIN, 0)
#define COFF gpio_put(C_PIN, 0)
#define DOFF gpio_put(D_PIN, 0)

//BOTONES
#define RESET_PIN 14
#define PULS1_PIN 15
#define DELAY_MS  100


//BUZZER
#define BUZZER 12
#define F_BUZZ 1000
#define F_COUNTER 150000000UL//150MHz
#define WRAP 65535 //16 bits resolucion (0 - 65535)
#define LEVEL WRAP/2
#define DIV ((float) F_COUNTER/(F_BUZZ * (WRAP + 1))) // (0 - 255)
#define SLEEP 200

#endif