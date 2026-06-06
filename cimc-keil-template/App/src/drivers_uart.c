#include "drivers_uart.h"
#include "project/inc/config.h"
#include "gd32f4xx_hal.h"
#include <string.h>

/* Simple blocking UART implementation using HAL. Adjust UART instance and pins per board. */

#define APP_USART huart1 /* expected to be defined in system layer */
extern UART_HandleTypeDef APP_USART;

/* RS485 DE pin - user must map in config.h or board file */
#ifndef RS485_DE_PORT
#define RS485_DE_PORT GPIOA
#define RS485_DE_PIN  GPIO_PIN_8
#endif

static char linebuf[512];
static int linepos = 0;

void rs485_tx_enable(void)
{
    HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_SET);
}
void rs485_rx_enable(void)
{
    HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET);
}

void drivers_uart_init(uint32_t baud)
{
    /* Configure huart1 elsewhere in system_init(); here ensure RX interrupt enabled */
    (void)baud;
}

void uart_send_string(const char *s)
{
    rs485_tx_enable();
    HAL_Delay(1);
    HAL_UART_Transmit(&APP_USART, (uint8_t*)s, strlen(s), 200);
    rs485_rx_enable();
}

int uart_readline(char *buf, int maxlen)
{
    /* blocking read: read bytes until '\n' or timeout - user may replace with IRQ-based version */
    uint8_t ch;
    while (1) {
        if (HAL_UART_Receive(&APP_USART, &ch, 1, 200) == HAL_OK) {
            if (ch == '\r') continue;
            if (ch == '\n') {
                linebuf[linepos]='\0';
                int n = linepos;
                if (n >= maxlen) n = maxlen-1;
                memcpy(buf, linebuf, n+1);
                linepos = 0;
                return 1;
            }
            if (linepos < (int)sizeof(linebuf)-1) linebuf[linepos++] = (char)ch;
        } else {
            /* timeout */
            return 0;
        }
    }
}

bool uart_has_prepare_transfer(void)
{
    /* Placeholder: user should detect 0x0502 command frame or raw trigger; returning false here */
    return false;
}

int receive_firmware_slices(void)
{
    /* Placeholder for receiving binary slices over UART (non-framed). Implement per contest: each slice 256 bytes, ~5ms between slices. */
    return 0;
}
