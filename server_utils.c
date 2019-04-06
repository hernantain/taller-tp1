#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "server_utils.h"
#include "common.h"

#define USER_LENGTH 4
#define BAD_REQUEST "HTTP/1.1 400 Bad request\n\n"
#define NOT_FOUND "HTTP/1.1 404 Not found\n\n"
#define REQUEST_OK "HTTP/1.1 200 OK\n\n"
#define MAX_TEMP_STRING_LEN 12
#define REPLACE_TAG_LEN 9


void list_create(list_t *self) {
	self->first = NULL;
	self->last = NULL;
	self->size = 0;
}

bool new_visit(list_t *self, char *element) {
	node_t *node = self->first;
	while (node != NULL) {
		if (compare(node->data, element, strlen(element))){
			node->times++;
			free(element);
			return false;
		}
		node = node->next_node;
	}
	return true;
}


int list_add_element(list_t *self, char *element) {
	if (self->size == 0) {
		node_t *node = malloc(sizeof(node_t));
		if (node == NULL)
			return 1;

		node->data = element;
		node->next_node = NULL;
		node->times = 1;
		self->first = node;
		self->last = node;
		self->size++;
		return 0;
	}

	if (new_visit(self,element)) {
		node_t *node = malloc(sizeof(node_t));
		if (node == NULL)
			return 1;

		node->data = element;
		node->next_node = NULL;
		node->times = 1;
		self->last->next_node = node;
		self->last = node;
		self->size++;
	}

	return 0;
}


void list_destroy(list_t *self) {
	if (self->size == 0)
		return;
	node_t *node = self->first;
	node_t *nxt_node;
	while (node != NULL) {
		free(node->data);
		nxt_node = node->next_node;
		free(node);
		node = nxt_node;
	}
}


int read_sensor_data(FILE *file) {
	unsigned short int bin_num;
	unsigned int s = fread(&bin_num, sizeof(bin_num), 1, file);
	if (s == 0) {
		return -1;
	} else {
		return htons(bin_num);
	}
}


void request_answer_create(request_answer_t *self) {
	self->header = NULL;
	self->body = NULL;
}


int get_line_length(char *line) {
	int i = 0;
	while (!compare(&line[i], "\n", 1))
		i++;

	return i;
}


char* get_user(char* request){
	char *user = NULL;
	char *user_line = strchr(request,'\n');
	int len;
	char *user_agent = NULL;
	while (!compare(user_line+1,"User",USER_LENGTH))
		user_line = strchr(user_line+1,'\n');

	user = strchr(user_line,':');
	len = get_line_length(user+2);
	user_agent = malloc(sizeof(char) * (len+1));
	memset(user_agent, 0, len+1);
	strncpy(user_agent,user+2,len);
	return user_agent;
}


bool valid_request(const char *request) {
	return compare(request+9,"200",3);
}


char* process_request(char *request) {
	if (!compare(request,"GET",3))
		return BAD_REQUEST;

	char *res = strchr(request, '/');
	if (!compare(res,"/sensor",7))
		return NOT_FOUND;

	return REQUEST_OK;
}


float get_temperature(int file_num) {
	return (file_num-2000)/100.0;
}


void replace_template_with_temperature(char *template, int file_num) {
	float temp = get_temperature(file_num);
	char string_temp[MAX_TEMP_STRING_LEN];
	memset(string_temp,0,MAX_TEMP_STRING_LEN);
	snprintf(string_temp,MAX_TEMP_STRING_LEN, "%.2f", temp);
	char *ret = strchr(template,'}');	
	while (true) {
		if (strchr(ret+1,'}') == NULL)
			break;
		ret = strchr(ret+1,'}');
	}

	for (int i = 0; i < strlen(template); ++i) {
		if (compare(&template[i],"{{",2)){
			memmove(&template[i], string_temp, strlen(string_temp));
			memmove(&template[i+strlen(string_temp)], ret+1, strlen(ret+1));
			memmove(&template[i+strlen(string_temp)+strlen(ret+1)], "\0", 
											REPLACE_TAG_LEN - strlen(string_temp));
		}
	}
}
