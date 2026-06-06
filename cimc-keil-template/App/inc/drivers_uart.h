# Drivers UART (RS485) header
#ifndef DRIVERS_UART_H
#define DRIVERS_UART_H
#include <stdint.h>
#include <stdbool.h>

void drivers_uart_init(uint32_t baud);
void uart_send_string(const char *s);
int uart_readline(char *buf, int maxlen);

/* RS485 control helpers (user may map DE pin in config.h) */
void rs485_tx_enable(void);
void rs485_rx_enable(void);

/* hooks used by protocol */
bool uart_has_prepare_transfer(void);
int receive_firmware_slices(void);

#endif
