#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "common_sockets.h"


void socket_create(socket_t *skt, const char *host, const char *port) {
	struct addrinfo hints, *results, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;

	int status = getaddrinfo(host, port,&hints,&results);

	if (status != 0){
		perror("getaddrinfo");
		exit(1);
	}

	for (rp = results; rp != NULL; rp = rp->ai_next) {
		skt->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (skt->fd == -1) {
			printf("Error: %s\n", strerror(errno));
			continue;
		} 

		if (connect(skt->fd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;

		close(skt->fd); 
	}
	freeaddrinfo(results);
}


void socket_turnoff_channel(socket_t *skt, int mode) {
	shutdown(skt->fd, mode);
}


void create_acceptor_socket(socket_t *skt, const char *port) {
	struct addrinfo hints, *results;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(NULL, port, &hints, &results);
	if (status != 0){
		perror("getaddrinfo");
		exit(1);
	}


	if ((skt->fd = socket(results->ai_family,results->ai_socktype,0)) < 0){
		perror("creando socket");
		printf("%sn",strerror(errno));
		freeaddrinfo(results);
		exit(1);
	}

	if ( bind(skt->fd, results->ai_addr, results->ai_addrlen) < 0 ) {
		perror("bind");
		printf("%sn",strerror(errno));
		freeaddrinfo(results);
		exit(1);
	}

	freeaddrinfo(results);

	if ( listen(skt->fd, 10) == -1 ) {
		perror("listen");
		printf("%s\n", strerror(errno));
		exit(1);
	}
}

void socket_send(socket_t *sock, char *buf, int size) {
	int sent = 0;
	int s = 0;

	while (sent < size) {
		s = send(sock->fd, &buf[sent], size-sent, MSG_NOSIGNAL);
		if (s <= 0) {
			return;
		} else {
			sent += s;
		}
	}

	return;
}

void socket_receive(socket_t *sock, char *buf, int size) {
	int received = 0;
	int s = 0;
	
	while (received < size) {
		s = recv(sock->fd, &buf[received], size-received, MSG_NOSIGNAL);
		if (s <= 0) { 
			return;
		} else {
			received += s;
		}
	}

	return;
} 

socket_t socket_accept(socket_t *skt) {
	socket_t new_skt;
	new_skt.fd = accept(skt->fd, NULL, NULL);
	return new_skt;
}

void socket_close(socket_t *skt) {
	close(skt->fd);
}

