#include "params.h"
#include "protocol.h"
#include "drivers_uart.h"
#include "oled.h"

int main(void)
{
    system_init(); /* clock, systick, etc. implement in platform layer */
    drivers_uart_init(19200); /* default required by test */
    oled_init();
    params_init();

    oled_display_line1(TEAM_ID_STRING); /* set your team id */
    oled_display_line2("IDLE");

    char linebuf[512];
    while (1) {
        /* simple: read ASCII hex lines terminated by newline from UART */
        if (uart_readline(linebuf, sizeof(linebuf))) {
            protocol_handle_ascii_line(linebuf);
        }
        /* other background tasks: timers, auto-report state handling in driver/timers */
        background_tasks();
    }
    return 0;
}
