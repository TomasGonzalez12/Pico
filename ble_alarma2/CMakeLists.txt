cmake_minimum_required(VERSION 3.13)

# === CONFIGURACIÓN BÁSICA ===
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === CONFIGURACIÓN PICO ===
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Configurar SDK path explícitamente (EVITA BUCLE)
set(PICO_SDK_PATH "C:/Users/tomas/.pico-sdk/sdk/2.2.0" CACHE PATH "Pico SDK path" FORCE)
message(STATUS "Using Pico SDK: ${PICO_SDK_PATH}")

# Importar SDK - IMPORTANTE: version simple
if(NOT DEFINED PICO_SDK_PATH)
    set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
    if(NOT PICO_SDK_PATH)
        set(PICO_SDK_PATH "C:/Users/tomas/.pico-sdk/sdk/2.2.0")
    endif()
endif()

# Verificar que existe el archivo de importación
if(EXISTS "${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
    include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/pico_sdk_import.cmake")
    include(pico_sdk_import.cmake)
else()
    message(FATAL_ERROR "No se encontró pico_sdk_import.cmake")
endif()

project(ble_alarma C CXX ASM)

# Inicializar SDK
pico_sdk_init()

# === CONFIGURACIÓN PARA EVITAR BUCLE ===
# Deshabilitar regeneración automática
set(CMAKE_SUPPRESS_REGENERATION ON)

# === SUBDIRECTORIOS ===
add_subdirectory(oled)

# === EJECUTABLE PRINCIPAL ===
add_executable(ble_alarma
    server.c
)

# === INCLUDE DIRECTORIES ===
target_include_directories(ble_alarma PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    # Incluir directorio oled si es necesario
    ${CMAKE_CURRENT_LIST_DIR}/oled
)

# === LINK LIBRARIES ===
target_link_libraries(ble_alarma
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    hardware_gpio
    oled  # Biblioteca del subdirectorio oled
)

# === GENERAR .h DESDE .gatt ===
pico_btstack_make_gatt_header(ble_alarma PRIVATE 
    "${CMAKE_CURRENT_LIST_DIR}/alarma_control.gatt"
)

# === HABILITAR STDIO ===
pico_enable_stdio_usb(ble_alarma 1)
pico_enable_stdio_uart(ble_alarma 0)  # Deshabilitar UART si no usas

# === SALIDAS ADICIONALES ===
pico_add_extra_outputs(ble_alarma)

# === FINAL: PREVENIR RE-EJECUCIÓN ===
# Esta línea al final evita que CMake se re-ejecute
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY CMAKE_CONFIGURE_DEPENDS "")