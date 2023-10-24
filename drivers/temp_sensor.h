#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H
#include <stdint.h>
#include <stdbool.h>
#include "platform.h"

#define DATA PC_8 // set DATA GPIO

void temp_init(void);

bool temp_data(float *data);

#endif

