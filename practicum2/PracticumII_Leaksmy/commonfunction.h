#define SUCCESS 0
#define FAILURE 1

// defining config data in client.c
#define MAX_MESSAGE_LENGTH 1024 
#define MAX_PATH_LENGTH 1000
#define PORT_NUMBER 2000
#define MY_IP_ADDRESS "127.0.0.1"

struct msg {
    int command;
    char local_file_path[MAX_PATH_LENGTH];
    char remote_file_path[MAX_PATH_LENGTH];
};

void copystring(char*, char*);
int isincommandarray(char*);
int directoryorfileexist(const char*);
char *concatenatepath(const char*, const char*);
int executing_write_command(char*, char*);
int executing_get_command(char*, char*);
int executing_ls_command(char*);
int executing_remove_command(char*);

// use in server.c
int directoryexist(const char*);
