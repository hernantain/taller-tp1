#ifndef _SERVER_USER_AGENT_LIST
#define _SERVER_USER_AGENT_LIST

//TDA USER_AGENT_LIST
//ES UNA LISTA ENLAZADA QUE CONTIENE EN CADA NODO EL User-Agent,
//LA CANTIDAD DE VISITAS QUE REALIZO Y UN PUNTERO AL PROXIMO NODO.
//LOS User-Agent SON GUARDADOS EN ORDEN DE VISITA
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

//CREA LA LISTA
void list_create(list_t *self);

//AGREGA UN NUEVO NODO A LA LISTA O INCREMENTA LA CANTIDAD DE VISITAS
//DE UN USER EXISTENTE EN LA LISTA
int list_add_element(list_t *self, char *element);

//HELPER FUNCTION PARA DETERMINAR SI UN USER ES UNA VISITA NUEVA O NO
bool new_visit(list_t *self, char *element);

//DESTRUYE LA LISTA
void list_destroy(list_t *self);

#endif
