#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "server_user_agent_list.h"
#include "common.h"


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
