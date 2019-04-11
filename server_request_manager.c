#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "server_utils.h"
#include "common.h"
#include "server_request_manager.h"

#define USER_LENGTH 4
#define BAD_REQUEST "HTTP/1.1 400 Bad request\n\n"
#define NOT_FOUND "HTTP/1.1 404 Not found\n\n"
#define REQUEST_OK "HTTP/1.1 200 OK\n\n"

void request_create(request_manager_t *self) {
	self->header = NULL;
	self->body = NULL;
	self->user = NULL;
}


int get_line_length(char *line) {
	int i = 0;
	while (!compare(&line[i], "\n", 1))
		i++;

	return i;
}

void request_load_user(request_manager_t *request) {
	char *user = NULL;
	char *user_line = strchr(request->orig_request,'\n');
	int len;
	//char *user_agent = NULL;
	while (!compare(user_line+1,"User",USER_LENGTH))
		user_line = strchr(user_line+1,'\n');

	user = strchr(user_line,':');
	len = get_line_length(user+2);
	request->user = malloc(sizeof(char) * (len+1));
	memset(request->user, 0, len+1);
	strncpy(request->user,user+2,len);
}

bool valid_request(request_manager_t *request) {
	return compare(request->header+9,"200",3);
}

void process_request(request_manager_t *answer) {
	char *request = answer->orig_request;
	if (!compare(request,"GET",3)) {
		answer->header = BAD_REQUEST;
		return;
	}

	char *res = strchr(request, '/');
	if (!compare(res,"/sensor",7)) {
		answer->header = NOT_FOUND;
		return;
	}

	answer->header = REQUEST_OK;
}
