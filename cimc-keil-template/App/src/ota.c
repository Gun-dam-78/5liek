#include "ota.h"
#include "drivers_flash.h"
#include "drivers_uart.h"
#include "project/inc/config.h"
#include <string.h>
#include "crc16.h"

#define STAGING_ADDR 0x08051000U
#define STAGING_SIZE 0x20000U /* 128K */
#define FIRMWARE_MAGIC 0x5AA5C33CUL

/* Wait for prepare transfer (uart_has_prepare_transfer) then receive slices into staging area and verify magic */
int receive_firmware_slices(void)
{
    /* Erase staging area */
    if (drivers_flash_erase_staging() != 0) return -1;
    /* Receive slices */
    int written = receive_firmware_slices_to(STAGING_ADDR, STAGING_SIZE);
    if (written <= 0) return -1;
    /* Verify magic: first 4 bytes == FIRMWARE_MAGIC (in little-endian stored) */
    uint32_t magic = 0;
    drivers_flash_read(STAGING_ADDR, (uint8_t*)&magic, 4);
    if (magic == FIRMWARE_MAGIC) return written;
    /* try endian swap if needed */
    uint32_t swapped = __builtin_bswap32(magic);
    if (swapped == FIRMWARE_MAGIC) return written;
    return -1;
}

int verify_firmware_magic(void)
{
    uint32_t magic = 0;
    drivers_flash_read(STAGING_ADDR, (uint8_t*)&magic, 4);
    if (magic == FIRMWARE_MAGIC) return 1;
    if (__builtin_bswap32(magic) == FIRMWARE_MAGIC) return 1;
    return 0;
}
