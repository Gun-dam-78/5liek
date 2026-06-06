#include "drivers_dac.h"
#include "gd32f4xx_hal.h"

extern DAC_HandleTypeDef hdac;

void drivers_dac_init(void)
{
    /* DAC init done in system layer */
}

void drivers_dac_set(uint16_t value)
{
    /* value 0..4095 for 12-bit DAC */
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}
