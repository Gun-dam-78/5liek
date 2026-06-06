#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdint.h>

int ascii_hex_to_bytes(const char *hex, uint8_t *out, int outlen);
void bytes_to_ascii_hex(const uint8_t *in, int len, char *out);
int build_frame(uint16_t device_id, uint8_t frame_type, uint16_t cmd, const uint8_t *content, uint8_t content_len, uint8_t *out, int out_max);
void send_frame_ascii_hex(uint16_t device_id, uint8_t frame_type, uint16_t cmd, const uint8_t *content, uint8_t content_len);
void protocol_handle_ascii_line(const char *line);

#endif
