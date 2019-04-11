#ifndef _SERVER_SENSOR
#define _SERVER_SENSOR
#include <stdio.h>

typedef struct {
	FILE *fd;
} sensor_t;

void sensor_create(sensor_t *s, const char *file);

int sensor_get_num(sensor_t *s);

void sensor_destroy(sensor_t *s);

#endif
