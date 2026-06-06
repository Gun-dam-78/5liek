#include "drivers_uart.h"
#include "project/inc/config.h"
#include "gd32f4xx_hal.h"
#include <string.h>
#include <stdbool.h>

/* This implementation uses HAL UART polling to receive framed ASCII hex lines and raw binary slices.
   Adjust UART instance and GPIO pins per your board. */

extern UART_HandleTypeDef huart1; /* defined in system layer */

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
    /* Assuming huart1 has been configured in system_init/MSP functions with desired baud.
       Here we reconfigure baud if needed. */
    huart1.Init.BaudRate = baud;
    HAL_UART_Init(&huart1);
}

void uart_send_string(const char *s)
{
    rs485_tx_enable();
    HAL_Delay(1);
    HAL_UART_Transmit(&huart1, (uint8_t*)s, (uint16_t)strlen(s), 500);
    HAL_Delay(1);
    rs485_rx_enable();
}

int uart_readline(char *buf, int maxlen)
{
    uint8_t ch;
    while (1) {
        if (HAL_UART_Receive(&huart1, &ch, 1, 500) == HAL_OK) {
            if (ch == '\r') continue;
            if (ch == '\n') {
                linebuf[linepos] = '\0';
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

/* For Bootloader: detect prepare-transfer command (frame 0502). We parse ASCII-hex framed line if available. */
bool _check_ascii_frame_for_cmd(const char *line, const char *cmd_hex)
{
    /* Look for command hex string cmd_hex (4 chars) after start header.
       A valid frame ASCII looks like: A5B6<DevID(4)><Type(2)><Cmd(4)><Len(2)><Ver(2)>...<CRC(4)><B6A5>
       We'll search for cmd_hex anywhere as simple heuristic but prefer position-based check if the line length matches.
    */
    if (!line) return false;
    if (strstr(line, cmd_hex) != NULL) {
        return true;
    }
    return false;
}

bool uart_has_prepare_transfer(void)
{
    char buf[512];
    if (uart_readline(buf, sizeof(buf))) {
        /* check if this ascii frame contains 0502 */
        if (_check_ascii_frame_for_cmd(buf, "0502")) return true;
    }
    return false;
}

/* Receive raw firmware slices (non-framed binary). This function will be used in Bootloader context.
   It expects sender to transmit raw binary slices of 256 bytes each, with small gaps (~5ms) between slices.
   The function will write sequentially to the provided flash address up to max_len bytes and return total bytes written.
*/
int receive_firmware_slices_to(uint32_t flash_addr, uint32_t max_len)
{
    const uint32_t slice_size = 256;
    uint8_t buffer[256];
    uint32_t written = 0;
    HAL_StatusTypeDef res;

    /* erase staging area before writing - caller should have erased region */

    while (written < max_len) {
        /* try to receive one slice with a timeout window (e.g., 200ms to allow slice arrival) */
        res = HAL_UART_Receive(&huart1, buffer, slice_size, 200);
        if (res == HAL_OK) {
            /* write to flash - ensure 32-bit aligned programming */
            uint32_t addr = flash_addr + written;
            /* program in 4-byte words */
            HAL_FLASH_Unlock();
            for (uint32_t i = 0; i < slice_size; i += 4) {
                uint32_t word = 0xFFFFFFFF;
                for (uint32_t j = 0; j < 4; j++) {
                    uint32_t idx = i + j;
                    if (idx < slice_size) word = (word & ~(0xFFu << (8*j))) | (((uint32_t)buffer[idx]) << (8*j));
                }
                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i, word) != HAL_OK) {
                    HAL_FLASH_Lock();
                    return -1;
                }
            }
            HAL_FLASH_Lock();
            written += slice_size;
            /* small delay to match sender pacing */
            HAL_Delay(5);
        } else {
            /* timeout - assume transmission finished */
            break;
        }
    }
    return (int)written;
}

/* Wrapper for compatibility with earlier prototype */
int receive_firmware_slices(void)
{
    /* staging area: 128K at 0x08051000 */
    const uint32_t staging_addr = 0x08051000U;
    const uint32_t staging_size = 0x20000U; /* 128K */
    return receive_firmware_slices_to(staging_addr, staging_size);
}
