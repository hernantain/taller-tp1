#ifndef _COMMON_SOCKETS
#define _COMMON_SOCKETS


//TDA SOCKET: USADO PARA ENCAPSULAR Y MODULARIZAR EL USO DE LOS SOCKETS
//SOLAMENTE CONTIENE SU FILE DESCRIPTOR, NECESARIO PARA REALIZAR TODAS LAS 
//OPERACIONES DE LOS SOCKETS
typedef struct {
	int fd;
} socket_t;

//CREA UN SOCKET PARA EL CLIENT
void socket_create(socket_t *skt, const char *host, const char *port);

//CIERRA EL CANAL DE LECTURA/ESCRITURA
void socket_turnoff_channel(socket_t *skt, int mode);

//CREA UN SOCKET ACEPTADOR DEL LADO DEL SERVER
void create_acceptor_socket(socket_t *skt, const char *port);

//FUNCION USADA PARA ENVIAR INFORMACION A TRAVES DEL SOCKET
void socket_send(socket_t *sock, char *buf, int size);

//FUNCION USADA PARA RECIBIR INFORMACION A TRAVES DEL SOCKET
void socket_receive(socket_t *sock, char *buf, int size);

//FUNCION USADA PARA EL SOCKET ACEPTADOR. 
//CUANDO ACEPTA UN NUEVO CLIENTE DEVUELVE UN NUEVO TDA SOCKET
//QUE ES EL QUE REALMENTE ESTABLECE LA CONEXION
socket_t socket_accept(socket_t *skt);

//CIERRA EL SOCKET
void socket_close(socket_t *skt);

#endif
