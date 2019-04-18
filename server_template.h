#ifndef _SERVER_TEMPLATE
#define _SERVER_TEMPLATE

//TDA TEMPLATE
//CONTIENE EL PUNTERO AL ARCHIVO DEL TEMPLATE HTML PARA LA RESPUESTA
typedef struct {
	FILE *f;
} template_t;

//CREA EL TDA TEMPLATE
void template_create(template_t *template, const char *file_name);

//CARGA TODO EL CONTENIDO DEL TEMPLATE HTML PARA LUEGO ENVIAR LA RESPUESTA
char* template_get_content(template_t *template);

//DESTRUYE EL TDA TEMPLATE
void template_destroy(template_t *t);

#endif
