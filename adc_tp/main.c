#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "systick.h"
#include "hardware.h"


int main()
{
    stdio_init_all();
    init_systick();

    gpio_init(LED);

    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED, 0);

    adc_init();
    adc_gpio_init(ADC_0_GPIO);
    adc_gpio_init(ADC_1_GPIO);
    adc_select_input(ADC_CHANNEL_0);

    
}
