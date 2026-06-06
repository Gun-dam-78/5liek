#include "drivers_adc.h"
#include "gd32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;

void drivers_adc_init(void)
{
    /* Initialize ADC peripheral in system_init() or here if needed */
}

float drivers_adc_read_ch0(void)
{
    /* Blocking single conversion on channel 0 - user must ensure channel configured */
    uint32_t val = 0;
    if (HAL_ADC_Start(&hadc1) == HAL_OK) {
        if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
            val = HAL_ADC_GetValue(&hadc1);
        }
        HAL_ADC_Stop(&hadc1);
    }
    /* Convert ADC value (12-bit) to voltage (assuming Vref=3.3) */
    return (float)val * 3.3f / 4095.0f;
}

float drivers_adc_read_ch1(void)
{
    /* Channel1 conversion - may need to switch channel in ADC sampling sequence */
    return drivers_adc_read_ch0(); /* placeholder: map to same ADC */
}
