/* Protocol framing/parsing (ASCII hex on serial) - skeleton */
#include <string.h>
#include <stdint.h>
#include "protocol.h"
#include "crc16.h"
#include "drivers_uart.h"
#include "params.h"

static uint8_t ascii_hex_nibble(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    return 0xFF;
}

int ascii_hex_to_bytes(const char *hex, uint8_t *out, int outlen)
{
    int hexlen = strlen(hex);
    if (hexlen % 2) return -1;
    int bytes = hexlen / 2;
    if (bytes > outlen) return -1;
    for (int i = 0; i < bytes; i++) {
        uint8_t hi = ascii_hex_nibble(hex[2*i]);
        uint8_t lo = ascii_hex_nibble(hex[2*i+1]);
        if (hi == 0xFF || lo == 0xFF) return -1;
        out[i] = (hi << 4) | lo;
    }
    return bytes;
}

void bytes_to_ascii_hex(const uint8_t *in, int len, char *out)
{
    const char *tbl = "0123456789ABCDEF";
    for (int i = 0; i < len; i++) {
        out[2*i]   = tbl[(in[i] >> 4) & 0xF];
        out[2*i+1] = tbl[in[i] & 0xF];
    }
    out[len*2] = 0;
}

int build_frame(uint16_t device_id, uint8_t frame_type, uint16_t cmd, const uint8_t *content, uint8_t content_len, uint8_t *out, int out_max)
{
    int required = 2+2+1+2+1+1 + content_len + 2 + 2;
    if (required > out_max) return -1;
    int p = 0;
    out[p++] = 0xA5; out[p++] = 0xB6;
    out[p++] = (device_id >> 8) & 0xFF; out[p++] = (device_id) & 0xFF;
    out[p++] = frame_type;
    out[p++] = (cmd >> 8) & 0xFF; out[p++] = cmd & 0xFF;
    out[p++] = content_len;
    out[p++] = 0x02; /* protocol version */
    if (content_len && content) {
        memcpy(&out[p], content, content_len);
        p += content_len;
    }
    uint16_t crc = crc16_modbus(out, p);
    out[p++] = (crc >> 8) & 0xFF;
    out[p++] = crc & 0xFF;
    out[p++] = 0xB6; out[p++] = 0xA5;
    return p; /* binary length */
}

void send_frame_ascii_hex(uint16_t device_id, uint8_t frame_type, uint16_t cmd, const uint8_t *content, uint8_t content_len)
{
    uint8_t bin[256];
    int blen = build_frame(device_id, frame_type, cmd, content, content_len, bin, sizeof(bin));
    if (blen <= 0) return;
    char ascii[512];
    bytes_to_ascii_hex(bin, blen, ascii);
    uart_send_string(ascii); /* sends raw ASCII bytes */
}

void protocol_handle_ascii_line(const char *line)
{
    uint8_t buf[256];
    int blen = ascii_hex_to_bytes(line, buf, sizeof(buf));
    if (blen <= 0) return;
    if (blen < 14) return;
    if (!(buf[0]==0xA5 && buf[1]==0xB6 && buf[blen-2]==0xB6 && buf[blen-1]==0xA5)) return;
    uint16_t rx_crc = ((uint16_t)buf[blen-4] << 8) | buf[blen-3];
    if (crc16_modbus(buf, blen-4) != rx_crc) {
        send_frame_ascii_hex(get_device_id(), 0xFF, 0xEEEE, NULL, 0);
        return;
    }
    uint16_t dev_id = ((uint16_t)buf[2] << 8) | buf[3];
    uint8_t ftype = buf[4];
    uint16_t cmd   = ((uint16_t)buf[5] << 8) | buf[6];
    uint8_t clen   = buf[7];
    uint8_t version= buf[8];
    uint8_t *content = &buf[9];
    if (dev_id != 0xFFFF && dev_id != get_device_id()) {
        return;
    }
    if (ftype == 0x01) {
        switch (cmd) {
            case 0x0101:
                send_frame_ascii_hex(get_device_id(), 0x02, 0x0101, (uint8_t*)"\xFF", 1);
                system_reboot();
                break;
            case 0x0104: {
                uint8_t ver[4] = {2,0,1,0};
                send_frame_ascii_hex(get_device_id(), 0x02, 0x0104, ver, 4);
                break;
            }
            case 0x0201: {
                float v = get_ch0_scaled();
                uint8_t fb[4];
                union {float f; uint8_t b[4];} u;
                u.f = v;
                fb[0]=u.b[3]; fb[1]=u.b[2]; fb[2]=u.b[1]; fb[3]=u.b[0];
                send_frame_ascii_hex(get_device_id(), 0x02, 0x0201, fb, 4);
                break;
            }
            case 0x0302: {
                start_auto_report();
                send_frame_ascii_hex(get_device_id(), 0x02, 0x0302, (uint8_t*)"\xFF",1);
                break;
            }
            case 0x0303: {
                stop_auto_report();
                send_frame_ascii_hex(get_device_id(), 0x02, 0x0303, (uint8_t*)"\xFF",1);
                break;
            }
            case 0x0501: {
                send_frame_ascii_hex(get_device_id(), 0x02, 0x0501, (uint8_t*)"\xFF",1);
                request_bootloader_on_next_reset();
                system_reboot();
                break;
            }
            default:
                send_frame_ascii_hex(get_device_id(), 0xFF, 0xEEEE, NULL, 0);
                break;
        }
    }
}
