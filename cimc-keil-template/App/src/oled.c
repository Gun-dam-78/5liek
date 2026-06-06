#include "oled.h"
#include "gd32f4xx_hal.h"
#include <string.h>

/* Simple placeholder OLED driver: user must bind I2C peripheral and SSD1306 implementation */
static char line1[32];
static char line2[32];

void oled_init(void)
{
    /* init i2c and display - left as user implementation */
    strcpy(line1, " ");
    strcpy(line2, " ");
}

void oled_display_line1(const char *s)
{
    strncpy(line1, s, sizeof(line1)-1);
    line1[sizeof(line1)-1] = '\0';
    /* call ssd1306 draw string functions here */
}

void oled_display_line2(const char *s)
{
    strncpy(line2, s, sizeof(line2)-1);
    line2[sizeof(line2)-1] = '\0';
}
