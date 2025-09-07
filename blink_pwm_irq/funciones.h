#ifndef FUNCIONES_H
#define FUNCIONES_H

//Estados
typedef enum{
    nulo,
    irq_frec,
    formula_frec
}estados_t;

extern estados_t estado_actual;

//Banderas
typedef struct{
    uint8_t input                   : 1;
    uint8_t entrada_valida          : 1;
    uint8_t puls_presionado         : 1;
    uint8_t wrap_irq                : 1;
    uint8_t config_irq_frec_ok      : 1;
    uint8_t config_formula_frec_ok  : 1;
    uint8_t reservado               : 2;
}flags_t;

extern volatile flags_t flag;

//Variables globales
extern uint slice_num;
extern uint32_t level;
extern float div;

extern volatile uint32_t ahora, ventana;

typedef struct{
    uint32_t f_pwm;
    uint32_t f_input;
    uint32_t duty ;
    uint16_t num_wraps;
    uint8_t t_off;
    uint8_t t_on;
}led_t;

extern led_t led;

extern volatile uint32_t wrap_cont;

//Encabezados de funciones
void init_config(void);
void puls_callback(uint gpio, uint32_t event_mask);
void reset_var_puls();
void ingresar_frec_input(void);
void config_irq_frec(void);
void config_formula_frec(void);
void pow_wrap_cont(void);

#endif