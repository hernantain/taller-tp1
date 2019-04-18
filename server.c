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
#include "common_sockets.h"
#include "server_user_agent_list.h"
#include "server_sensor.h"
#include "server_template.h"
#include "server_request_manager.h"

#define MAX_HTTP_REQUEST_LEN 512

#define PORT 1
#define SENSOR 2
#define TEMPLATE 3
#define ALL_ARGS 4

int main(int argc, char *argv[]) {
	if (argc != ALL_ARGS){
		fprintf(stderr,
			"Uso:\n./server <port> <sensor-filename> <template-filename>\n");
		exit(1);
	}

	request_manager_t req;
	template_t template;

	sensor_t sensor;
	sensor_create(&sensor, argv[SENSOR]);

	socket_t acceptor_skt, exchange_skt;
	create_acceptor_socket(&acceptor_skt, argv[PORT]);

	list_t visitors;
	list_create(&visitors);
	
	bool running = true; 
	int num = sensor_get_num(&sensor);

	while(running){
		exchange_skt = socket_accept(&acceptor_skt);
		if (exchange_skt.fd == -1) {
			printf("Accept Error: %s\n", strerror(errno));
		} else {
			socket_receive(&exchange_skt, req.orig_request, MAX_HTTP_REQUEST_LEN); 
			socket_turnoff_channel(&exchange_skt, SHUT_RD);
			
			request_create(&req);
			process_request(&req);

			if (valid_request(&req)) {
				request_load_user(&req);
				list_add_element(&visitors,req.user);

				template_create(&template, argv[TEMPLATE]);

				req.body = template_get_content(&template);
				replace_template_with_temperature(req.body,num);

				socket_send(&exchange_skt, req.header, (int) strlen(req.header));
				socket_send(&exchange_skt, req.body, (int) strlen(req.body));
				num = sensor_get_num(&sensor);

				free(req.body);
				template_destroy(&template);
				if (num == -1)
					running = false;

			} else {
				socket_send(&exchange_skt, req.header, (int) strlen(req.header));
			}	
		}
		socket_turnoff_channel(&exchange_skt, SHUT_RDWR);
		socket_close(&exchange_skt);
	}

	socket_turnoff_channel(&exchange_skt, SHUT_RDWR);
	socket_close(&acceptor_skt);
	sensor_destroy(&sensor);
	
	fprintf(stdout,"# Estadisticas de visitantes\n\n");
	
	node_t *visit = visitors.first;
	while (visit != NULL) {
		fprintf(stdout, "* %s: %i\n", visit->data, visit->times);
		visit = visit->next_node;		
	}

	list_destroy(&visitors);
	exit(0);
}
