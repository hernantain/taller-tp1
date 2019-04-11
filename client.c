#define _POSIX_C_SOURCE 200809L

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
#include "common.h"

#define MAX_HTTP_RESPONSE 1024

#define HOST 1
#define PORT 2
#define FILENAME 3


int main(int argc, char* argv[]) {
	size_t nread;
	FILE *f = 0;
	bool connected = true;

	if ((argc < 3) | (argc > 4)) {
		fprintf(stderr,"Uso:\n./client <host> <port> [<filename>]\n");
		exit(1);
	} else if (argc == 4) {
		const char *filename = argv[FILENAME];
		f = fopen(filename, "rt");
		if (f == NULL)
			exit(1);
	} 

	socket_t socket;
	socket_create(&socket, argv[HOST], argv[PORT]);

	char *line = NULL;
	size_t len = 0;
	while (connected) {
		if (argc == 3) {
			while ((nread = getline(&line, &len, stdin)) != -1) {
				socket_send(&socket, line, (int) strlen(line));
			}
		} else {
			while ((nread = getline(&line, &len, f)) != -1) 
				socket_send(&socket, line, (int) strlen(line) );
		}

		socket_turnoff_channel(&socket, SHUT_WR);
		char response[MAX_HTTP_RESPONSE];
		memset(response, 0, MAX_HTTP_RESPONSE);
		socket_receive(&socket, response, MAX_HTTP_RESPONSE);
		printf("%s", response);
		socket_turnoff_channel(&socket,SHUT_RD);
		connected = false;
	}

	free(line);
	
	if (argc == 4) 
		fclose(f);

	return 0;
}
