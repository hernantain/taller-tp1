#include <arpa/inet.h>
#include <stdlib.h>
#include "server_sensor.h"


void sensor_create(sensor_t *s, const char *file) {
	s->fd = fopen(file, "rb");
	if (s->fd == NULL)
		exit(1);
}

int sensor_get_num(sensor_t *s) {
	unsigned short int bin_num;
	unsigned int r = fread(&bin_num, sizeof(bin_num), 1, s->fd);
	if (r == 0) {
		return -1;
	} else {
		return htons(bin_num);
	}
}

void sensor_destroy(sensor_t *s) {
	fclose(s->fd);
}
