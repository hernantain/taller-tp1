#ifndef _SERVER_REQUEST_MANAGER
#define _SERVER_REQUEST_MANAGER


typedef struct {
	char orig_request[512];
	char *header;
	char *body;
	char *user;
} request_manager_t;


void request_create(request_manager_t *self);

int get_line_length(char *line);

void request_load_user(request_manager_t *request);

bool valid_request(request_manager_t *request);

void process_request(request_manager_t *answer);

bool valid_request(request_manager_t *request);

#endif