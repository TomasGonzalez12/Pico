#include <stdio.h>
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "picoOled.h"

#include "alarma_control.h"

// Definición de pines GPIO
#define MODO_TOTAL_PIN 16
#define MODO_PARCIAL_PIN 18
#define BTN_PANICO_PIN 20
#define BT_SIRENA_PIN 21
#define SALIDA_AUX_PIN 12

#define APP_AD_FLAGS 0x06

//Esto es para la pantalla oled
#define SDA_PIN_OLED 8
#define SCL_PIN_OLED 9
t_OledParams ble_Oled;

void configOled(t_OledParams *oled){

    oled->i2c         = i2c0;
    oled->SDA_PIN     = SDA_PIN_OLED;
    oled->SCL_PIN     = SCL_PIN_OLED;

    oled->ctlrType    = CTRL_SH1106;
    oled->i2c_address = 0x3C;
    oled->height      = H_64;
    oled->width       = W_132;

    oledI2cConfig(oled);
}

void oled_printf(const char *format, ...) {
    char buffer[64];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    oledWriprueba_oledr(&ble_Oled, (const uint8_t *)buffer, strlen(buffer));
    oledDisplay(&ble_Oled);
}

//Datos y config del bluetooth
static uint8_t adv_data[] = {
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    0x0C, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'P','i','c','o',' ','A','l','a','r','m','a',
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0xE0, 0xFF,
};
static const uint8_t adv_data_len = sizeof(adv_data);

static btstack_packet_callback_registration_t hci_event_callback_registration;

// Estado de todas las salidas (1 byte, cada bit representa un estado)
static uint8_t output_states = 0;

void apagar_todos_los_leds(void) {
    output_states = 0;
    
    gpio_put(MODO_TOTAL_PIN, 0);
    gpio_put(MODO_PARCIAL_PIN, 0);
    gpio_put(BTN_PANICO_PIN, 0);
    gpio_put(BT_SIRENA_PIN, 0);
    gpio_put(SALIDA_AUX_PIN, 0);
}

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    UNUSED(channel);
    UNUSED(size);

    if (packet_type != HCI_EVENT_PACKET) return;

    uint8_t event_type = hci_event_packet_get_type(packet);
    bd_addr_t local_addr;

    switch (event_type) {
        case BTSTACK_EVENT_STATE:
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
            
            gap_local_bd_addr(local_addr);
            printf("BTstack up and running on %s\n", bd_addr_to_str(local_addr));
            
            // Setup advertisements
            uint16_t adv_int_min = 800;
            uint16_t adv_int_max = 800;
            uint8_t adv_type = 0;
            bd_addr_t null_addr;
            memset(null_addr, 0, 6);
            gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
            gap_advertisements_set_data(adv_data_len, adv_data);
            gap_advertisements_enable(1);
            printf("Advertising started\n");
            break;
            
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            printf("Disconnected\n");
            oled_printf("Desconectado\n\n");
            apagar_todos_los_leds();
            break;
            
        case ATT_EVENT_CONNECTED:
            printf("Client connected\n");
            oled_printf("Cliente conectado\n\n");
            break;
            
        case ATT_EVENT_MTU_EXCHANGE_COMPLETE:
            printf("ATT MTU = %u\n", att_event_mtu_exchange_complete_get_MTU(packet));
            break;
            
        default:
            break;
    }
}

static uint16_t att_read_callback(hci_con_handle_t connection_handle, uint16_t att_handle, 
                                   uint16_t offset, uint8_t *buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);
    
    if (att_handle == ATT_CHARACTERISTIC_FFE1_01_VALUE_HANDLE) {
        printf("Output states read: 0x%02X\n", output_states);
        return att_read_callback_handle_blob(&output_states, 1, offset, buffer, buffer_size);
    }
    return 0;
}

static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, 
                              uint16_t transaction_mode, uint16_t offset, 
                              uint8_t *buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);
    UNUSED(transaction_mode);
    UNUSED(offset);
    
    if (att_handle == ATT_CHARACTERISTIC_FFE1_01_VALUE_HANDLE && buffer_size > 0) {
        uint8_t command = buffer[0];
        
        // Comandos individuales (0x81-0x85 para encender, 0x01-0x05 para apagar)
        switch(command) {
            case 0x81: // Encender Modo Total
                output_states |= 0x01;
                printf("Comando: Encender Modo Total\n");
                break;
            case 0x01: // Apagar Modo Total
                output_states &= ~0x01;
                printf("Comando: Apagar Modo Total\n");
                break;
            case 0x82: // Encender Modo Parcial
                output_states |= 0x02;
                printf("Comando: Encender Modo Parcial\n");
                break;
            case 0x02: // Apagar Modo Parcial
                output_states &= ~0x02;
                printf("Comando: Apagar Modo Parcial\n");
                break;
            case 0x84: // Encender Botón Pánico
                output_states |= 0x04;
                printf("Comando: Encender Botón Pánico\n");
                break;
            case 0x04: // Apagar Botón Pánico
                output_states &= ~0x04;
                printf("Comando: Apagar Botón Pánico\n");
                break;
            case 0x88: // Encender Sirena
                output_states |= 0x08;
                printf("Comando: Encender Sirena\n");
                break;
            case 0x08: // Apagar Sirena
                output_states &= ~0x08;
                printf("Comando: Apagar Sirena\n");
                break;
            case 0x90: // Encender Salida Aux
                output_states |= 0x10;
                printf("Comando: Encender Salida Aux\n");
                break;
            case 0x10: // Apagar Salida Aux
                output_states &= ~0x10;
                printf("Comando: Apagar Salida Aux\n");
                break;
            default:
                // Si es <= 0x1F, usa el método original (compatibilidad)
                if (command <= 0x1F) {
                    output_states = command;
                    printf("Comando: Set estado completo 0x%02X\n", command);
                } else {
                    printf("Comando desconocido: 0x%02X\n", command);
                }
                break;
        }
        
        // Aplicar cambios a los GPIOs
        gpio_put(MODO_TOTAL_PIN, (output_states & 0x01) ? 1 : 0);
        gpio_put(MODO_PARCIAL_PIN, (output_states & 0x02) ? 1 : 0);
        gpio_put(BTN_PANICO_PIN, (output_states & 0x04) ? 1 : 0);
        gpio_put(BT_SIRENA_PIN, (output_states & 0x08) ? 1 : 0);
        gpio_put(SALIDA_AUX_PIN, (output_states & 0x10) ? 1 : 0);
        
        //Esto sale por terminal
        printf("Estado actual: 0x%02X\n", output_states);
        printf("  Modo Total (GPIO %d): %s\n", MODO_TOTAL_PIN, (output_states & 0x01) ? "ON" : "OFF");
        printf("  Modo Parcial (GPIO %d): %s\n", MODO_PARCIAL_PIN, (output_states & 0x02) ? "ON" : "OFF");
        printf("  Boton Panico (GPIO %d): %s\n", BTN_PANICO_PIN, (output_states & 0x04) ? "ON" : "OFF");
        printf("  Sirena (GPIO %d): %s\n", BT_SIRENA_PIN, (output_states & 0x08) ? "ON" : "OFF");
        printf("  Salida Aux (GPIO %d): %s\n", SALIDA_AUX_PIN, (output_states & 0x10) ? "ON" : "OFF");
        printf("\n");

       //Esto por oled
        oledClear(&ble_Oled, BLACK);
        oled_printf("MODO    EST  ON   OFF\n\n");
        oledDraw_line(&ble_Oled, 0, 9, 127, 9, WHITE);
        oled_printf("Total   %s  0x81  01\n",   (output_states & 0x01) ? "ON " : "OFF");
        oled_printf("Parcial %s  0x82  02\n",   (output_states & 0x02) ? "ON " : "OFF");
        oled_printf("Panico  %s  0x84  04\n",   (output_states & 0x04) ? "ON " : "OFF");
        oled_printf("Sirena  %s  0x88  08\n",   (output_states & 0x08) ? "ON " : "OFF");
        oled_printf("Aux     %s  0x90  10\n",   (output_states & 0x10) ? "ON " : "OFF");

    }
    return 0;
}

// Función para inicializar todos los GPIOs
static void init_gpios(void) {
    const uint pins[] = {MODO_TOTAL_PIN, MODO_PARCIAL_PIN, BTN_PANICO_PIN, BT_SIRENA_PIN, SALIDA_AUX_PIN};
    
    for (int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_OUT);
        gpio_put(pins[i], 0);
        printf("GPIO %d inicializado como salida\n", pins[i]);
    }
}

int main() {
    stdio_init_all();
    
    configOled(&ble_Oled);
    oledSetTTYMode(&ble_Oled, true); 
    oledSet_invert(&ble_Oled, false);

    // Inicializar todos los GPIOs
    init_gpios();

    printf("\n=== Pico 2W BLE Alarma Control ===\n");
    oled_printf("==== Pico 2W ====\n BLE Alarma Control\n\n");
    
   
    
    // Initialize CYW43
    if (cyw43_arch_init()) {
        printf("Failed to initialize cyw43\n");
        oled_printf("Error al iniciar CYW43\n\n");
        return -1;
    }
    printf("CYW43 initialized\n");
    oled_printf("CYW43 iniciado\n\n");
    
    // Initialize BTstack
    l2cap_init();
    sm_init();
    
    // Setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);
    
    // Register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);
    att_server_register_packet_handler(packet_handler);
    
    // Turn on Bluetooth
    hci_power_control(HCI_POWER_ON);
    printf("BLE stack initialized\n");
    printf("Connect with nRF Connect or LightBlue\n");
    printf("Service UUID: FFE0\n");
    printf("Characteristic UUID: FFE1\n");
    printf("\n=== COMANDOS DISPONIBLES ===\n");
    printf("ENCENDER individualmente:\n");
    printf("  0x81 - Modo Total\n");
    printf("  0x82 - Modo Parcial\n");
    printf("  0x84 - Boton Panico\n");
    printf("  0x88 - Sirena\n");
    printf("  0x90 - Salida Aux\n");
    printf("\nAPAGAR individualmente:\n");
    printf("  0x01 - Modo Total\n");
    printf("  0x02 - Modo Parcial\n");
    printf("  0x04 - Boton Panico\n");
    printf("  0x08 - Sirena\n");
    printf("  0x10 - Salida Aux\n");
    printf("\nCOMANDOS COMBINADOS (compatibilidad):\n");
    printf("  0x00 - Apagar todo\n");
    printf("  0x1F - Encender todo\n");
    printf("  0x03 - Modo Total + Parcial\n");
    printf("  0x09 - Modo Total + Sirena\n");
    printf("  etc...\n\n");

    sleep_ms(2500);

    oledClear(&ble_Oled, BLACK);

    oled_printf("MODO    ON     OFF\n\n");
    oledDraw_line(&ble_Oled, 0, 9, 127, 9, WHITE);
    oled_printf("Total   0x81   0x01\n");
    oled_printf("Parcial 0x82   0x02\n");
    oled_printf("Panico  0x84   0x04\n");
    oled_printf("Sirena  0x88   0x08\n");
    oled_printf("Aux     0x90   0x10\n");

    
    // Main loop
    while (1) {
        async_context_poll(cyw43_arch_async_context());
        async_context_wait_for_work_until(cyw43_arch_async_context(), at_the_end_of_time);
    }
    
    return 0;
}