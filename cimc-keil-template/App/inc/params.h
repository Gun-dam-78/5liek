#ifndef PARAMS_H
#define PARAMS_H
#include <stdint.h>

#define PARAMS_MAGIC 0xA55AA55A

typedef struct {
    uint32_t magic;
    uint16_t device_id;
    uint8_t baud_map;
    float ch0_ratio;
    float ch1_ratio;
    float ch0_threshold;
    float ch1_threshold;
} device_params_t;

void params_init(void);
uint16_t get_device_id(void);
uint8_t get_baud_map(void);
float get_ch0_ratio(void);
float get_ch1_ratio(void);
void params_save(void);

float get_ch0_scaled(void);
float get_ch1_scaled(void);

#endif
