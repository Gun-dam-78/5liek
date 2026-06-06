#include "ota.h"
#include "drivers_flash.h"
#include "drivers_uart.h"
#include "project/inc/config.h"
#include <string.h>

/* Partition addresses (as in spec) */
#define APP_START_ADDR        0x08011000U
#define APP_BACKUP_ADDR       0x08031000U
#define STAGING_ADDR          0x08051000U
#define PARAM_AREA_ADDR       0x08010000U

/* Magic word expected in firmware bin (big-endian 0x5AA5C33C) */
#define FIRMWARE_MAGIC        0x5AA5C33CUL

static volatile int g_request_bootloader = 0;

void request_bootloader_on_next_reset(void)
{
    g_request_bootloader = 1;
    /* In practice store flag in backup register or parameter area */
}

void system_reboot(void)
{
    /* Use NVIC_SystemReset from CMSIS */
    NVIC_SystemReset();
}

/* Bootloader helpers - called in bootloader context */
int uart_has_prepare_transfer(void)
{
    /* In bootloader, detect a 0x0502 command framed; simplified placeholder: return 0; */
    return 0;
}

/* Receive firmware slices into STAGING_ADDR; each slice 256 bytes, sender provides exactly file size
   This implementation is placeholder and returns 0 (not received). Real implementation must
   read raw bytes from UART (non-framed) and write sequentially to flash staging area. */
int receive_firmware_slices(void)
{
    /* Example algorithm (to implement on hardware):
       - Erase staging area (size 128K)
       - For each slice: read 256 bytes over UART with expected inter-slice delay ~5ms
       - Program into flash sequentially
       - After all slices received, return number of bytes written (>0)
    */
    return 0;
}

int verify_firmware_magic(void)
{
    uint32_t magic = 0;
    drivers_flash_read(STAGING_ADDR, (uint8_t*)&magic, 4);
    /* magic as stored little-endian on flash - ensure ordering as required by your toolchain
       Compare to FIRMWARE_MAGIC interpreted in little-endian */
    if (magic == FIRMWARE_MAGIC) return 1;
    return 0;
}
