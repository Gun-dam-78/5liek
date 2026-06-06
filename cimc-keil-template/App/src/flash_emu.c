/* Simple flash parameter emulation (stub) - actual flash write must use vendor HAL */
#include "flash_emu.h"
#include <string.h>
#include "params.h"

#define PARAM_AREA_ADDR ((uint32_t)0x08010000)
#define PARAM_AREA_SIZE ((uint32_t)0x1000)

int flash_params_store(const void *data, uint32_t len)
{
    (void)data; (void)len;
    return 0; /* success */
}

int flash_params_load(void *data, uint32_t len)
{
    memset(data, 0, len);
    return 0;
}
