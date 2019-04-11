#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "server_utils.h"
#include "common.h"
#include "server_user_agent_list.h"

#define MAX_TEMP_STRING_LEN 12
#define REPLACE_TAG_LEN 9

float get_temperature(int file_num) {
	return (file_num-2000)/100.0;
}


void replace_template_with_temperature(char *template, int file_num) {
	float temp = get_temperature(file_num);
	char string_temp[MAX_TEMP_STRING_LEN];
	memset(string_temp,0,MAX_TEMP_STRING_LEN);
	snprintf(string_temp,MAX_TEMP_STRING_LEN, "%.2f", temp);
	char *ret = strchr(template,'}');	
	while (true) {
		if (strchr(ret+1,'}') == NULL)
			break;
		ret = strchr(ret+1,'}');
	}

	for (int i = 0; i < strlen(template); ++i) {
		if (compare(&template[i],"{{",2)){
			memmove(&template[i], string_temp, strlen(string_temp));
			memmove(&template[i+strlen(string_temp)], ret+1, strlen(ret+1));
			memmove(&template[i+strlen(string_temp)+strlen(ret+1)], "\0", 
											REPLACE_TAG_LEN - strlen(string_temp));
		}
	}
}
