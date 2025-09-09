#include "pico/stdlib.h"
#include "definiciones.h"
#include "numeros.h"

void put_cero(){
    AOFF;
    BOFF;
    COFF;
    DOFF;
}

void put_uno(){
    AON;
    BOFF;
    COFF;
    DOFF;
}
void put_dos(){
    AOFF;
    BON;
    COFF;
    DOFF;
}
void put_tres(){
    AON;
    BON;
    COFF;
    DOFF;
}
void put_cuatro(){
    AOFF;
    BOFF;
    CON;
    DOFF;
}
void put_cinco(){
    AON;
    BOFF;
    CON;
    DOFF;
}
void put_seis(){
    AOFF;
    BON;
    CON;
    DOFF;
}
void put_siete(){
    AON;
    BON;
    CON;
    DOFF;
}
void put_ocho(){
    AOFF;
    BOFF;
    COFF;
    DON;
}
void put_nueve(){
    AON;
    BOFF;
    COFF;
    DON;
}