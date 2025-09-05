function(make_firmware board)
    pico_sdk_init()
    add_executable(${board}
        sdvx.c button.c cli.c usb_descriptors.c
    )
    target_compile_definitions(${board} PUBLIC BOARD_sdvx_PICO
                               PICO_XOSC_STARTUP_DELAY_MULTIPLIER=6)

    pico_enable_stdio_usb(${board} 1)
    pico_enable_stdio_uart(${board} 0)

    target_compile_options(${board} PRIVATE -Wall -Werror -Wfatal-errors -O3)
    target_include_directories(${board} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

    target_link_libraries(${board} PRIVATE
            pico_multicore pico_stdlib hardware_pio hardware_pwm hardware_flash
            hardware_adc hardware_i2c hardware_watchdog
            tinyusb_device tinyusb_board)

    pico_add_extra_outputs(${board})
endfunction()

make_firmware(sdvx)