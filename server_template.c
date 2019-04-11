#include <stdio.h>
#include <stdlib.h>
#include "server_template.h"

void template_create(template_t *template, const char *file_name) {
	template->f = fopen(file_name, "rt");
	if (template->f == NULL) 
		exit(1);
}

char* template_get_content(template_t *template){
	fseek(template->f, 0, SEEK_END);
	long fsize = ftell(template->f);
	rewind(template->f);  

	char *content = malloc(fsize + 1);
	if (content == NULL) {
		fclose(template->f);
		exit(1);
	}

	unsigned int s = fread(content, 1, fsize, template->f);
	if (s == 0) {
		fclose(template->f);
		free(content);
		exit(1);
	}

	content[s] = '\0';
	return content;
}

void template_destroy(template_t *t) {
	fclose(t->f);
}
