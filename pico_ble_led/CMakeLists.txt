# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Import SDK
include(pico_sdk_import.cmake)

project(picow_ble_led C CXX ASM)

pico_sdk_init()

add_executable(picow_ble_led
    server.c
)

# IMPORTANTE: El directorio debe incluirse ANTES de linkear las bibliotecas
target_include_directories(picow_ble_led PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(picow_ble_led
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    hardware_gpio
)

# Generate .h from .gatt
pico_btstack_make_gatt_header(picow_ble_led PRIVATE "${CMAKE_CURRENT_LIST_DIR}/led_control.gatt")

# Enable USB output
pico_enable_stdio_usb(picow_ble_led 1)
pico_enable_stdio_uart(picow_ble_led 0)

pico_add_extra_outputs(picow_ble_led)
