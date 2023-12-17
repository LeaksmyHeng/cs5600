/*
* HengL.CS5600.LearnC.c / 
* Program in C
* Leaksmy Heng / CS5600 / Northeastern University
* Fall 2023 / Oct 30, 2023
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// defining length of content, sender and receiver
#define MAX_MSG_LENGTH 200
#define MAX_SENDER_RECEIVER_LENGTH 50

// set up a data structure for message
typedef struct message {
	int unique_identifier;
	int timesent;
	char sender[MAX_SENDER_RECEIVER_LENGTH];
	char receiver[MAX_SENDER_RECEIVER_LENGTH];
	char content[MAX_MSG_LENGTH];	// content of the message
	int flag;		//flag indicate whether message is delivered
} message;


void print_size_of_message(message* message) {
	size_t struct_size = sizeof(message);
	// printf("message id: %d -> size of message is %zu bytes\n", message->unique_identifier, struct_size);
	return;
}



struct message create_msg(int unique_identifier, const char* sender, const char* receiver, const char* content) {
	/**
	Create a struct message.
	
	:param unique_identifier: unique identifier of a message
	:param sender: the message's sender
	:param receiver: the message's receiver
	:param content: the message's content
	
	if the length of the sender, receiver and content are longer than what is specified,
	we will only copy over that specified length.
	*/
	int datetime_sending;
	int flag = 0;		// 0 indicate message not delivered and 1 message delivered
	datetime_sending = (int)time(NULL);
	
	// put a check for the length of sender, receiver and conent
	int sender_length = strlen(sender);
	int receiver_length = strlen(receiver);
	int content_length = strlen(content);
	if (sender_length > MAX_SENDER_RECEIVER_LENGTH) {
		printf("Id %d has sender's length more than %d.\n", unique_identifier, MAX_SENDER_RECEIVER_LENGTH);
	}
	if (receiver_length > MAX_SENDER_RECEIVER_LENGTH) {
		printf("Id %d has receiver's length more than %d.\n", unique_identifier, MAX_SENDER_RECEIVER_LENGTH);
	}
	if (content_length > MAX_MSG_LENGTH) {
		printf("Id %d has content's length more than %d.\n", unique_identifier, MAX_MSG_LENGTH);
	}
	
	// initalize struct message
	struct message new_message;
	
	// copy input values to struct
	new_message.unique_identifier = unique_identifier;
	new_message.timesent = datetime_sending;
	strncpy(new_message.sender, sender, sizeof(new_message.sender)-1);
	strncpy(new_message.receiver, receiver, sizeof(new_message.receiver)-1);
	strncpy(new_message.content, content, sizeof(new_message.content)-1);
	new_message.flag = flag;
	
	// check to ensure that the message has the same size before and after retrieving
	// printf("print message size when creating message. ");
	// print_size_of_message(&new_message);
	
	return new_message;
}


void print_msg(const message message) {
	printf("unique_id is: %d\n", message.unique_identifier);
	printf("timesend is: %d\n", message.timesent);
	printf("sender is: %s\n", message.sender);
	printf("receiver is: %s\n", message.receiver);
	printf("content is: %s\n", message.content);
	printf("flag is: %d\n", message.flag);
	printf("\n");
}


void store_msg_in_disk(const message message) {
	/**
	Functions to store message in disk. Based on some research on the internet,
	one way to store data in disk is through file I/O. 
	https://towardsdatascience.com/how-your-data-is-stored-on-disk-and-memory-8842891da52
	Therefore, I'll read the message
	then stored it as a txt file with its unique_id as its name.
	*/
	
	int unique_identifier = message.unique_identifier;
	char filename[20];	// assuming file name has max of 20 character
	snprintf(filename, sizeof(filename), "%d.txt", unique_identifier);
	
	// check if unique_identifier or filename is already exist, if it is send message out
	FILE *fileopen;
	
	if (fileopen = fopen(filename, "r")) {
		// if the unique_identifier exisit, do not store it and move forward.
		// Do not stop the program
		fclose(fileopen);
		printf("unique_identifier: %d already exists\n\n", unique_identifier);
		return;
	}
	else {
		// store message in Disk
		FILE *file = fopen(filename, "w");
		if (file == NULL) {
			printf("Unable to open the %s to write\n", filename);
			return;
		}
		fprintf(file, "unique_id is: %d\n", message.unique_identifier);
		fprintf(file, "timesend is: %d\n", message.timesent);
		fprintf(file, "sender is: %s\n", message.sender);
		fprintf(file, "receiver is: %s\n", message.receiver);
		fprintf(file, "content is: %s\n", message.content);
		fprintf(file, "flag is: %d\n", message.flag);
		
		fclose(file);
	}
}


struct message retrieve_msg_from_disk(int unique_identifier) {
	/**
	This function will take message_identifier or the message unique identifier.
	Look through the directory and see if there is any text file that has that name.
		If it has -> return the message (message* message)
		if not -> return NULL.
	*/
	char filename[20];
	snprintf(filename, sizeof(filename), "%d.txt", unique_identifier);
	
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Cannot find %s in disk\n", filename);
		exit(1);
	}
	// read data from file and populate the message structure
	char line[1000];
	int id, timesent, flag;
	char sender[MAX_SENDER_RECEIVER_LENGTH], receiver[MAX_SENDER_RECEIVER_LENGTH], content[MAX_MSG_LENGTH];
	// initalized message
	while (fgets(line, sizeof(line), file) != NULL) {
		if (strstr(line, "unique_id is: ")) {
			sscanf(line, "unique_id is: %d", &id);
		}
		else if (strstr(line, "timesend is: ")) {
			sscanf(line, "timesend is: %d", &timesent);
		}
		else if (strstr(line, "sender is: ")) {
			sscanf(line, "sender is: %[^\n]", sender);
		}
		else if (strstr(line, "receiver is: ")) {
			sscanf(line, "receiver is: %[^\n]", receiver);
		}
		else if (strstr(line, "content is: ")) {
			sscanf(line, "content is: %[^\n]", content);
		}
		else if (strstr(line, "flag is: ")) {
			sscanf(line, "flag is: %d", &flag);
		}
	}
	
	// strdup this so that we can avoid memory leak & core dump error
	char* ptr_sender = strdup(sender);
	char* ptr_receiver = strdup(receiver);
	char* ptr_content = strdup(content);
	
	struct message new_message;
	new_message.unique_identifier = unique_identifier;
	new_message.timesent = timesent;
	strncpy(new_message.sender, ptr_sender, sizeof(new_message.sender)-1);
	strncpy(new_message.receiver, ptr_receiver, sizeof(new_message.receiver)-1);
	strncpy(new_message.content, ptr_content, sizeof(new_message.content)-1);
	new_message.flag = 1;	// turn the flag to 1 which mean retrieving
	fclose(file);
	
	// check to ensure that the message has the same size before and after retrieving
	printf("print message size when retrieving message.\n");
	print_size_of_message(&new_message);
	return new_message;
}

