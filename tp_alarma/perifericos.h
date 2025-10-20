#ifndef PERIFERICOS_H
#define PERIFERICOS_H

typedef struct{
    uint8_t id;
    bool f_pres; // bandera para usar en el callback de pulsador, 
}boton_t; 
// inicializados en funciones.c junto a la funcion callback

typedef struct{
    uint8_t id;
    uint8_t estado;
}sensores_t;

typedef struct{
    uint8_t  estado;
    uint32_t tiempo_act; //esto se podria poner como input por la pagina de html, si no, sacar y usar un define en definiciones y chau
}buzz_t;

typedef struct{
    uint8_t id;
    uint16_t carga;
}bateria_t;

#endif
