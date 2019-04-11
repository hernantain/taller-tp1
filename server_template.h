#ifndef _SERVER_TEMPLATE
#define _SERVER_TEMPLATE

typedef struct {
	FILE *f;
} template_t;

void template_create(template_t *template, const char *file_name);

char* template_get_content(template_t *template);

void template_destroy(template_t *t);

#endif