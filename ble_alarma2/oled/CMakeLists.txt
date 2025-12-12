add_library(oled
    picoOled.c
)

target_include_directories(oled PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(oled
    hardware_i2c
    pico_stdlib
)
