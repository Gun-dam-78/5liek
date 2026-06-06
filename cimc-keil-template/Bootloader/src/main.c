/* Bootloader skeleton */
#include <stdio.h>
#include <stdint.h>
#include "drivers_uart.h"
#include "protocol.h"
#include "flash_emu.h"
#include "oled.h"

#define APP_START_ADDRESS 0x08011000U

static volatile int upgrade_mode = 0;

void jump_to_app(void)
{
    typedef void (*pfunc)(void);
    uint32_t sp = *((uint32_t*)APP_START_ADDRESS);
    uint32_t reset = *((uint32_t*)(APP_START_ADDRESS + 4));
    (void)sp; (void)reset;
    while (1) { }
}

int main(void)
{
    system_init();
    drivers_uart_init(115200);
    oled_init();
    oled_display_line1(TEAM_ID_STRING);
    oled_display_line2("Bootloader");

    if (!upgrade_mode) {
        delay_ms(5000);
        jump_to_app();
    } else {
        for (int t=10; t>0; t--) {
            char buf[64];
            snprintf(buf,sizeof(buf),"wait for start Application(%ds)...", t);
            uart_send_string(buf);
            delay_ms(1000);
            if (uart_has_prepare_transfer()) {
                if (receive_firmware_slices()) {
                    if (verify_firmware_magic()) {
                        send_frame_ascii_hex(0x0000, 0x02, 0x0502, (uint8_t*)"\xFF",1);
                    } else {
                        send_frame_ascii_hex(0x0000, 0xFF, 0x0502, NULL,0);
                    }
                }
            }
        }
        jump_to_app();
    }
    return 0;
}
