#ifndef MEMORY_SIMULATION_H
#define MEMORY_SIMULATION_H

#define MAX_MSG_LENGTH 200
#define MAX_SENDER_RECEIVER_LENGTH 50

typedef struct message {
	int unique_identifier;
	int timesent;
	char sender[MAX_SENDER_RECEIVER_LENGTH];
	char receiver[MAX_SENDER_RECEIVER_LENGTH];
	char content[MAX_MSG_LENGTH];	// content of the message
	int flag;	//flag indicate whether message is delivered
} message;


// function prototypes
void print_size_of_message(message* message);
struct message create_msg(int, const char*, const char*, const char*);
void print_msg(const message message);
void store_msg_in_disk(const message message);
struct message retrieve_msg_from_disk(int);

#endif
