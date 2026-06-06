/* Parameter management (persisted) */
#include "params.h"
#include "flash_emu.h"
#include <string.h>

static device_params_t g_params;

void params_init(void)
{
    flash_params_load(&g_params, sizeof(g_params));
    if (g_params.magic != PARAMS_MAGIC) {
        memset(&g_params, 0, sizeof(g_params));
        g_params.magic = PARAMS_MAGIC;
        g_params.device_id = 0x0001;
        g_params.baud_map = 0x13; /* 0x13 => 19200 */
        g_params.ch0_ratio = 1.0f;
        g_params.ch1_ratio = 1.0f;
        g_params.ch0_threshold = 0.0f;
        g_params.ch1_threshold = 0.0f;
        flash_params_store(&g_params, sizeof(g_params));
    }
}

uint16_t get_device_id(void) { return g_params.device_id; }
uint8_t get_baud_map(void) { return g_params.baud_map; }
float get_ch0_ratio(void) { return g_params.ch0_ratio; }
float get_ch1_ratio(void) { return g_params.ch1_ratio; }

void params_save(void)
{
    flash_params_store(&g_params, sizeof(g_params));
}

float get_ch0_scaled(void) {
    float raw = drivers_adc_read_ch0(); /* implement in drivers_adc.c */
    return raw * g_params.ch0_ratio;
}
float get_ch1_scaled(void) {
    float raw = drivers_adc_read_ch1();
    return raw * g_params.ch1_ratio;
}
