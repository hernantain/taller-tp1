#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE

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
#include "common.h"

#define MAX_HTTP_RESPONSE 1024


int main(int argc, char* argv[]) {
	const char *filename;
	const char *host, *port;
	char *line = NULL;
	size_t len = 0, nread;
	FILE *f = 0;
	int sockfd;
	struct addrinfo hints, *results, *rp;
	bool connected = true;

	if ((argc < 3) | (argc > 4)) {
		fprintf(stderr,"Uso:\n./client <host> <port> [<filename>]\n");
		exit(1);
	} else if (argc == 4) {
		filename = argv[3];
		f = fopen(filename, "rt");
		if (f == NULL)
			exit(1);
	} 

	host = argv[1];
	port = argv[2];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;

	int status = getaddrinfo(host, port,&hints,&results);

	if (status != 0){
		perror("getaddrinfo");
		exit(1);
	}

	sockfd = -1;
	for (rp = results; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1) {
			printf("Error: %s\n", strerror(errno));
			continue;
		} 

		if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;

		close(sockfd); 
	}

	/********************************************SOCKET DEF *********/
	while (connected) {
		if (argc == 3) {
			while ((nread = getline(&line, &len, stdin)) != -1) {
				send_message(sockfd, line, (int) strlen(line));
			}
		} else {
			while ((nread = getline(&line, &len, f)) != -1) 
				send_message(sockfd, line, (int) strlen(line) );
		}

		shutdown(sockfd, SHUT_WR);
		char response[MAX_HTTP_RESPONSE];
		memset(response, 0, MAX_HTTP_RESPONSE);
		receive_message(sockfd, response, MAX_HTTP_RESPONSE);
		printf("%s", response);
		shutdown(sockfd,SHUT_RD);
		connected = false;
	}

	freeaddrinfo(results);
	free(line);
	
	if (argc == 4) 
		fclose(f);

	return 0;
}
