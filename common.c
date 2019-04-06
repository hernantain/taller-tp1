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


void send_message(int sock, char *buf, int size) {
	int sent = 0;
	int s = 0;

	while (sent < size) {
		s = send(sock, &buf[sent], size-sent, MSG_NOSIGNAL);
		if (s <= 0) {
			return;
		} else {
			sent += s;
		}
	}

	return;
}

void receive_message(int sock, char *buf, int size) {
	int received = 0;
	int s = 0;
	
	while (received < size) {
		s = recv(sock, &buf[received], size-received, MSG_NOSIGNAL);
		if (s <= 0) { 
			return;
		} else {
			received += s;
		}
	}

	return;
} 

bool compare(const char *s1, const char *s2, int size) {
	return (strncmp(s1,s2,size) == 0);
}

char* get_file_content(FILE* f){
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	rewind(f);  

	char *content = malloc(fsize + 1);
	if (content == NULL) {
		fclose(f);
		exit(1);
	}

	unsigned int s = fread(content, 1, fsize, f);
	if (s == 0) {
		fclose(f);
		free(content);
		exit(1);
	}

	content[s] = '\0';
	return content;
}
