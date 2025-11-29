cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Config VSCode (no tocar)
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

set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Importar SDK
include(pico_sdk_import.cmake)
project(prueba_oled C CXX ASM)

pico_sdk_init()

# ------------------------
#   AGREGAR LA LIBRERÍA OLED
# ------------------------
add_subdirectory(oled)

# Crear ejecutable
add_executable(prueba_oled
    main.c
    image.h
)

pico_set_program_name(prueba_oled "prueba_oled")
pico_set_program_version(prueba_oled "0.1")

# Salida por USB/UART
pico_enable_stdio_uart(prueba_oled 0)
pico_enable_stdio_usb(prueba_oled 0)

# Librerías del proyecto
target_link_libraries(prueba_oled
    pico_stdlib
    hardware_i2c
    oled             # <--- nuestra librería
)

# Includes del proyecto
target_include_directories(prueba_oled PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(prueba_oled)

