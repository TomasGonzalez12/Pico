#include "pico.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include "picoOled.h"
#include "image.h"

t_OledParams g_myOled;

// Initialize the oled structure with the information
//  required to define the display and then configure it
void configOled(t_OledParams *oled){

    oled->i2c         = i2c0;
    oled->SDA_PIN     = 8;
    oled->SCL_PIN     = 9;

    oled->ctlrType    = CTRL_SH1106;
    oled->i2c_address = 0x3C;
    oled->height      = H_64;
    oled->width       = W_132;

// this will configure the OLED module and then
//  clear the screen.
    oledI2cConfig(oled);
}

int main(){

    char msg[] = "Hello, world!\n\n";
    char msg2[] = "Segundo texto 1234567\n";
    char msg3[] = "Tercer texto!\n";
    char msg4[] = "Cuarto texto!\n";
    char msg5[] = "Quinto texto!\n";

    //Initialise I/O
        stdio_init_all(); 
    
    // configure the OLED module
        configOled(&g_myOled);

    // set for TTY mode and print a string
    oledSetTTYMode(&g_myOled, true);
    oledSet_invert(&g_myOled, false);
    oledWriprueba_oledr(&g_myOled, (const uint8_t *)msg, strlen(msg));
    oledDraw_line(&g_myOled, 0, 9, 127, 9, WHITE);
    oledDisplay(&g_myOled);
    oledWriprueba_oledr(&g_myOled, (const uint8_t *)msg2, strlen(msg2));
    oledWriprueba_oledr(&g_myOled, (const uint8_t *)msg3, strlen(msg3));
    oledWriprueba_oledr(&g_myOled, (const uint8_t *)msg4, strlen(msg4));
    oledWriprueba_oledr(&g_myOled, (const uint8_t *)msg5, strlen(msg5));
    //oledDisplay(&g_myOled);
    /*
    // prueba_oled line drawing
    oledDraw_line(&g_myOled, 0, 0, 127, 63, WHITE);
    oledDisplay(&g_myOled);

    // load and display an image.
    oledBmp_show_image(&g_myOled, image_data, image_size);
    // this function must be called after loading the image into
    //  the Pi's buffer memory
    oledDisplay(&g_myOled);
    */
    // spin here until the heat death of the cosmos, we are reset,
    //  or power is removed.
        while (true){}

}