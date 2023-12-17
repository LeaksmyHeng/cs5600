/*
 * server.c -- TCP Socket Server
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "commonfunction.h"

// mutex to protect shared resources
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

// struct to pass multiple parameter to thread
struct thread_args{
    int client_sock;
    struct sockaddr_in client_addr;
    struct msg client_message;
};


void *handle_client(void *arg) {
    struct thread_args *thread_arg = (struct thread_args*)arg;
    int client_sock = thread_arg->client_sock;
    struct sockaddr_in client_addr = thread_arg->client_addr;
    struct msg client_message = thread_arg->client_message;

    char server_message[MAX_MESSAGE_LENGTH];

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));

    printf("Received msg from client at IP %s and port %i: \n"
           "command: %d\nremote_file_path: %s\nlocal_file_path: %s\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port),
           client_message.command,
           client_message.remote_file_path,
           client_message.local_file_path
    );

    // Simulate processing (e.g., writing to a file)
    pthread_mutex_lock(&file_mutex);
    // handle each command
    int command = client_message.command;
    if (command == 0) {
        executing_write_command(client_message.local_file_path, client_message.remote_file_path);
    }
    else if (command == 1) {
        executing_get_command(client_message.remote_file_path, client_message.local_file_path);
    }
    else if (command == 2) {
        executing_remove_command(client_message.remote_file_path);
    }
    else if (command == 3) {
        executing_ls_command(client_message.remote_file_path);
    }

    // perform file operations
    pthread_mutex_unlock(&file_mutex);

    // Respond to the client
    strcpy(server_message, "This is the server's response message.");
    if (send(client_sock, server_message, strlen(server_message), 0) < 0) {
        printf("Server cannot send message.\n");
    }

    close(client_sock);
    free(arg);
    return NULL;
}


int main()
{
    int socket_desc, client_sock;
    socklen_t client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[MAX_MESSAGE_LENGTH];
    struct msg client_message;
    pthread_t thread_id;
    struct thread_args *thread_arg;

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(&client_message, 0, sizeof(client_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return FAILURE;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUMBER);
    server_addr.sin_addr.s_addr = inet_addr(MY_IP_ADDRESS);

    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return FAILURE;
    }
    printf("Done with binding\n");

    // Listen for clients:
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        return FAILURE;
    }
    printf("\nListening for incoming connections.....\n");

      // Server keeps running continuously until receive the command to close
      while (1) {
        // Accept an incoming connection:
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);

        if (client_sock < 0){
            printf("Can't accept\n");
            return FAILURE;
        }
        printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive client's message:
        if (recv(client_sock, &client_message, sizeof(client_message), 0) < 0){
            printf("Couldn't receive\n");
            return FAILURE;
        }

        int command = client_message.command;
        printf("Msg from client: \ncommand: %d\nlocal_file: '%s'\nremote_file: '%s'\n",
               command, client_message.remote_file_path, client_message.local_file_path);

          // allocate memory for thread arguments
          thread_arg = (struct thread_args *)malloc(sizeof(struct thread_args));
          if (thread_arg == NULL)  {
              printf("Failed while allocating thread arguments.\n");
              return FAILURE;
          }
          // populate thread arguments
          thread_arg->client_message = client_message;
          thread_arg->client_sock = client_sock;
          thread_arg->client_addr = client_addr;

          // create a new thread
          if (pthread_create(&thread_id, NULL, handle_client, (void*)thread_arg) < 0) {
              printf("Error while creating thread.\n");
              return FAILURE;
          }
          // detach thread
          pthread_detach(thread_id);
      }
        // this is not gonna hit cause we are in a while true
        close(client_sock);
        close(socket_desc);
        return SUCCESS;
}
