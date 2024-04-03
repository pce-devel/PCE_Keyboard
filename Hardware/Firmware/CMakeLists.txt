cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pcekeybd C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

set(FAMILY rp2040)

add_executable(pcekeybd)

pico_add_extra_outputs(pcekeybd)


# Example source
target_sources(pcekeybd PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/hid_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c
        )

# Example include
target_include_directories(pcekeybd PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(pcekeybd PRIVATE
	pico_stdlib
	pico_multicore
	tinyusb_host
	tinyusb_board
	)

