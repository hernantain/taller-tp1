#ifndef _SERVER_SENSOR
#define _SERVER_SENSOR
#include <stdio.h>

//TDA SENSOR
//CONTIENE UN PUNERO AL ARCHIVO BINARIO CON LA INFORMACION LEIDA DEL SENSOR
typedef struct {
	FILE *fd;
} sensor_t;

//CREA EL TDA SENSOR
void sensor_create(sensor_t *s, const char *file);

//LEE UN NUMERO DEL SENSOR A LA VEZ Y DEVUELVE EL NUMERO
//CALCULADO EN EL ENDIANNES CORRECTO
int sensor_get_num(sensor_t *s);

//DESTRUYE EL TDA SENSOR
void sensor_destroy(sensor_t *s);

#endif
