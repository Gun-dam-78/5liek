#include "system.h"
#include "gd32f4xx_hal.h"

UART_HandleTypeDef huart1;
ADC_HandleTypeDef hadc1;
DAC_HandleTypeDef hdac;

void system_init(void)
{
    HAL_Init();
    /* SystemClock_Config should be implemented to set up PLL and clocks */
    /* MX_GPIO_Init(); MX_USART1_UART_Init(); MX_ADC1_Init(); MX_DAC_Init(); MX_I2C1_Init(); */
}

void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
