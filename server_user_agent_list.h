#ifndef _SERVER_USER_AGENT_LIST
#define _SERVER_USER_AGENT_LIST


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

#endif