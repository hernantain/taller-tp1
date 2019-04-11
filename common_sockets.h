#ifndef _COMMON_SOCKETS
#define _COMMON_SOCKETS

typedef struct {
	int fd;
} socket_t;

void socket_create(socket_t *skt, const char *host, const char *port);

void socket_turnoff_channel(socket_t *skt, int mode);

void create_acceptor_socket(socket_t *skt, const char *port);

void socket_send(socket_t *sock, char *buf, int size);

void socket_receive(socket_t *sock, char *buf, int size);

socket_t socket_accept(socket_t *skt);

void socket_close(socket_t *skt);

#endif
