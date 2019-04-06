#ifndef _SERVER_UTILS
#define _SERVER_UTILS

typedef struct {
	char *header;
	char *body;
} request_answer_t;

typedef struct {
	char *data;
	void *next_node;
	int times;
} node_t;

typedef struct {
	node_t *first;
	node_t *last;
	size_t size;
} list_t;

void list_create(list_t *self);

int list_add_element(list_t *self, char *element);

bool new_visit(list_t *self, char *element);

void list_destroy(list_t *self);

int read_sensor_data(FILE *file);

char* process_request(char *request);

void request_answer_create(request_answer_t *self);

int get_line_length(char *line);

char* get_user(char *request);

float get_temperature(int file_num);

void replace_template_with_temperature(char *template, int file_num);

bool valid_request(const char* request);

#endif
