#include <stdio.h>
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "led_control.h"

#define LED_PIN 16
#define APP_AD_FLAGS 0x06

static uint8_t adv_data[] = {
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    0x0E, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'P','i','c','o',' ','L','E','D',' ','C','t','r','l',
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0xE0, 0xFF,
};
static const uint8_t adv_data_len = sizeof(adv_data);

static btstack_packet_callback_registration_t hci_event_callback_registration;
static uint8_t led_state = 0;

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
            break;
            
        case ATT_EVENT_CONNECTED:
            printf("Client connected\n");
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
        printf("LED state read: %d\n", led_state);
        return att_read_callback_handle_blob(&led_state, 1, offset, buffer, buffer_size);
    }
    return 0;
}

static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, 
                              uint16_t transaction_mode, uint16_t offset, 
                              uint8_t *buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);
    UNUSED(transaction_mode);
    UNUSED(offset);
    
    if (att_handle == ATT_CHARACTERISTIC_FFE1_01_VALUE_HANDLE) {
        if (buffer_size > 0) {
            led_state = buffer[0];
            gpio_put(LED_PIN, led_state ? 1 : 0);
            printf("LED %s (value: 0x%02X)\n", led_state ? "ON" : "OFF", led_state);
        }
    }
    return 0;
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    printf("\n=== Pico 2W BLE LED Control ===\n");
    
    // Initialize GPIO for LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
    printf("LED initialized on GPIO %d\n", LED_PIN);
    
    // Initialize CYW43
    if (cyw43_arch_init()) {
        printf("Failed to initialize cyw43\n");
        return -1;
    }
    printf("CYW43 initialized\n");
    
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
    printf("Write 0x01 to turn LED ON, 0x00 to turn LED OFF\n\n");
    
    // Main loop
    while (1) {
        async_context_poll(cyw43_arch_async_context());
        async_context_wait_for_work_until(cyw43_arch_async_context(), at_the_end_of_time);
    }
    
    return 0;
}