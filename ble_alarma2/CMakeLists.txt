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

project(ble_alarma C CXX ASM)

pico_sdk_init()

add_subdirectory(oled)

add_executable(ble_alarma
    server.c
)

# IMPORTANTE: El directorio debe incluirse ANTES de linkear las bibliotecas
target_include_directories(ble_alarma PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(ble_alarma
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    hardware_gpio
    oled
)

# Generate .h from .gatt
pico_btstack_make_gatt_header(ble_alarma PRIVATE "${CMAKE_CURRENT_LIST_DIR}/alarma_control.gatt")

# Enable USB output
pico_enable_stdio_usb(ble_alarma 1)
pico_enable_stdio_uart(ble_alarma 1)

pico_add_extra_outputs(ble_alarma)
