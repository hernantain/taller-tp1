
void send_message(int sock, char *buf, int size);

void receive_message(int sock, char *buf, int size);

char* get_file_content(FILE* f);

bool compare(const char *s1, const char *s2, int size);
