/*
 * client.c -- TCP Socket Client
 *
 * adapted from:
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "commonfunction.h"


int main(int argc, char *argv[]) {
    int socket_desc;
    struct sockaddr_in server_addr;

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0) {
        perror("Unable to create socket");
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUMBER);
    server_addr.sin_addr.s_addr = inet_addr(MY_IP_ADDRESS);

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Unable to connect");
        return -1;
    }

    printf("Connected with server successfully\n");

    // handle command line arguments
    if (argc < 3) {
        printf("There should at least be 3 argument. Example: rfs WRITE local-file-path\n");
        return FAILURE;
    }
    else if (argc > 4) {
        printf("At most 4 arguments only. Example: rfs WRITE local-file-path remote-file-path\n");
        return FAILURE;
    }
    // copy the string over from the argument
    char local_file_path[MAX_PATH_LENGTH], remote_file_path[MAX_PATH_LENGTH];
    // check if we support the command or not
    // so the server only handle those that are supported
    int issupportedcommand = isincommandarray(argv[1]);
    if (issupportedcommand == -1) {
        printf("Not supported command\n");
        return FAILURE;
    }

    // proceed to copy data from local to server if supportedcommand = 0
    // this is question1
    struct msg msg;
    msg.command = issupportedcommand;
    if (issupportedcommand == 0) {
        printf("Sending 'WRITE' command...\n");
        copystring(local_file_path, argv[2]);
        if (argc == 4) {
            copystring(remote_file_path, argv[3]);
        }
        else {
            copystring(remote_file_path, argv[2]);
        }
        copystring(msg.remote_file_path, remote_file_path);
        copystring(msg.local_file_path, local_file_path);
    }
    else if (issupportedcommand == 1) {
        printf("Sending 'GET' command...\n");
        copystring(remote_file_path, argv[2]);
        if (argc == 4) {
            copystring(local_file_path, argv[3]);
        }
        else {
            copystring(local_file_path, argv[2]);
        }
        copystring(msg.remote_file_path, remote_file_path);
        copystring(msg.local_file_path, local_file_path);
    }
    else if (issupportedcommand == 2) {
        printf("Executing 'REMOVE' command...\n");
        copystring(remote_file_path, argv[2]);
        copystring(msg.remote_file_path, remote_file_path);
        copystring(msg.local_file_path, remote_file_path);
        //int removefile = executing_remove_command(remote_file_path);
    }
    else if (issupportedcommand == 3) {
        printf("Executing 'LS' command...\n");
        copystring(remote_file_path, argv[2]);
        copystring(msg.remote_file_path, remote_file_path);
        copystring(msg.local_file_path, remote_file_path);
        //int result = executing_ls_command(remote_file_path);
    }

    printf("Sending information to server side:\n"
           "command: %d\nremote file: %s\nlocal file: %s\n",
           msg.command, msg.remote_file_path, msg.local_file_path);

    // Send the struct to the server
    if (send(socket_desc, &msg, sizeof(msg), 0) < 0) {
        perror("Unable to send struct");
        close(socket_desc);
        return -1;
    }

    // Close the socket:
    close(socket_desc);

    return 0;
}
