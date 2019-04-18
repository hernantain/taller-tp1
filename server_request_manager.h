#ifndef _SERVER_REQUEST_MANAGER
#define _SERVER_REQUEST_MANAGER

#define MAX_LEN 512


//TDA RQUEST_MANAGER
//SE ENCARGA DE GUARDAR LA INFORMACION DEL REQUEST
//GUARDA EL REQUEST ORIGINA DEL CLIENTE Y EXTRAE DE EL LA RESPUESTA
//HEADER; BODY Y USER PARA LUEGO ENVIAR ESTA INFORMACION
typedef struct {
	char orig_request[MAX_LEN];
	char *header;
	char *body;
	char *user;
} request_manager_t;

//CREA EL REQUEST MANAGER
void request_create(request_manager_t *self);

//HELPER FUNCTION QUE DEVUELVE LA LONGITUD DE LA LINEA
int get_line_length(char *line);

//EXTRAE EL User-agent DEL REQUEST
void request_load_user(request_manager_t *request);

//DEVUELVE TRUE SI EL REQUEST ES VALIDO.
//FALSO EN CASO CONTRARIO
bool valid_request(request_manager_t *request);

//PROCESA EL REQUEST Y CARGA EL HEADER CON EL STATUS DEL MISMO
//SEA VALIDO O NO. 
void process_request(request_manager_t *answer);

#endif
