cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(picoEEPROM C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Creates ROM object file from ROM binary
add_custom_command(
    OUTPUT "${PROJECT_BINARY_DIR}/rom.obj"
    COMMAND bash "${PROJECT_SOURCE_DIR}/scripts/generate_rom_header.sh" "rom.bin" "${PROJECT_BINARY_DIR}/rom.obj"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(rom_header ALL DEPENDS "${PROJECT_BINARY_DIR}/rom.obj")

# Adds EEPROM Emulator to the project
add_library(eeprom STATIC
    ${PROJECT_SOURCE_DIR}/src/eeprom/eeprom.cpp
)

target_include_directories(eeprom PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(eeprom PRIVATE
    ${PROJECT_BINARY_DIR}/rom.obj
)


# Adds I/O wrapper to the project
add_library(io STATIC
    ${PROJECT_SOURCE_DIR}/src/io/io.cpp
)

target_include_directories(io PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(io PUBLIC
    pico_stdlib
    hardware_pio
    hardware_gpio
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wno-format
    -Wno-unused-function
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    io 
    eeprom
    pico_stdlib
    hardware_clocks
    hardware_vreg
)

pico_generate_pio_header(io ${CMAKE_CURRENT_LIST_DIR}/pio/bus.pio)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})