#ifndef DEFINICIONES_H
#define DEFINICIONES_H

//Leds
#define LEDR_PIN 16
#define LEDA_PIN 18 
#define LEDV_PIN 20
#define LED_MASK (1 << LEDR_PIN) | (1 << LEDA_PIN) | (1 << LEDV_PIN)

#define LED_RON gpio_put(LEDR_PIN, 1)
#define LED_AON gpio_put(LEDA_PIN, 1)
#define LED_VON gpio_put(LEDV_PIN, 1)
#define LED_ROFF gpio_put(LEDR_PIN, 0)
#define LED_AOFF gpio_put(LEDA_PIN, 0)
#define LED_VOFF gpio_put(LEDV_PIN, 0)

#define SLEEP 250

//Botón
#define PULS_PIN 15
#define DELAY_MS 20

#endif