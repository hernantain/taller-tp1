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
#include "common.h"
#include "server_utils.h"

#define MAX_HTTP_REQUEST_LEN 512

int main(int argc, char *argv[]) {
	struct addrinfo hints, *results;
	unsigned int sockfd, newsockfd = 0;
	const char *port, *sensor_file, *template_file;
	char *user_agent;
	int num;
	FILE *tf, *f;
	list_t visitors;
	request_answer_t answer;

	if (argc != 4){
		fprintf(stderr,
			"Uso:\n./server <port> <sensor-filename> <template-filename>\n");
		exit(1);
	}

	port = argv[1];
	sensor_file = argv[2];
	template_file = argv[3];

	f = fopen(sensor_file, "rb");
	if (f == NULL)
		exit(1);

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(NULL, port, &hints, &results);
	if (status != 0){
		perror("getaddrinfo");
		exit(1);
	}

	if ((sockfd = socket(results->ai_family,results->ai_socktype,0)) < 0){
		perror("creando socket");
		printf("%sn",strerror(errno));
		freeaddrinfo(results);
		exit(1);
	}

	if ( bind(sockfd, results->ai_addr, results->ai_addrlen) < 0 ) {
		perror("bind");
		printf("%sn",strerror(errno));
		freeaddrinfo(results);
		exit(1);
	}

	freeaddrinfo(results);

	if ( listen(sockfd, 10) == -1 ) {
		perror("listen");
		printf("%s\n", strerror(errno));
		exit(1);
	}
	
	list_create(&visitors);
	bool running = true; 
	num = read_sensor_data(f);
	while(running){
		newsockfd = accept(sockfd, NULL, NULL);   
		if (newsockfd == -1) {
			printf("Accept Error: %s\n", strerror(errno));
		} else {
			char http_req[MAX_HTTP_REQUEST_LEN];
			memset(http_req, 0, MAX_HTTP_REQUEST_LEN);
			receive_message(newsockfd, http_req, MAX_HTTP_REQUEST_LEN); 
			shutdown(newsockfd, SHUT_RD);
			
			request_answer_create(&answer);
			answer.header = process_request(http_req);

			if (valid_request(answer.header)) {
				user_agent = get_user(http_req);
				list_add_element(&visitors,user_agent);

				tf = fopen(template_file, "rt");
				if (tf == NULL) 
					exit(1);
				
				answer.body = get_file_content(tf);
				replace_template_with_temperature(answer.body,num);

				send_message(newsockfd, answer.header, (int) strlen(answer.header));
				send_message(newsockfd, answer.body, (int) strlen(answer.body));
				num = read_sensor_data(f);

				free(answer.body);
				fclose(tf);
				if (num == -1)
					running = false;

			} else {
				send_message(newsockfd, answer.header, (int) strlen(answer.header));
			}	
		}
		shutdown(newsockfd, SHUT_RDWR);
		close(newsockfd);
	}

	shutdown(newsockfd, SHUT_RDWR);
	close(newsockfd);
	close(sockfd);
	fclose(f);
	fprintf(stdout,"# Estadisticas de visitantes\n\n");
	
	node_t *visit = visitors.first;
	while (visit != NULL) {
		fprintf(stdout, "* %s: %i\n", visit->data, visit->times);
		visit = visit->next_node;		
	}

	list_destroy(&visitors);
	exit(0);
}
