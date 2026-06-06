/* CRC-16-Modbus (polynomial 0x8005 reflected) implementation */
#include "crc16.h"

uint16_t crc16_modbus(const uint8_t *buf, uint32_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint32_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) crc = (crc >> 1) ^ 0xA001;
            else crc = (crc >> 1);
        }
    }
    return crc;
}
