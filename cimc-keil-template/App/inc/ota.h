# OTA and Bootloader helpers
#ifndef OTA_H
#define OTA_H

#include <stdint.h>

void request_bootloader_on_next_reset(void);
void system_reboot(void);

/* Bootloader side helpers */
int uart_has_prepare_transfer(void);
int receive_firmware_slices(void);
int verify_firmware_magic(void);

#endif
