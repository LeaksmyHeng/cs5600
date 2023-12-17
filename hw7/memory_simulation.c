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
#include <time.h>

// set up a data structure for message
typedef struct message {
	int unique_identifier;
	int timesent;
	char* sender;
	char* receiver;
	char* content;	// content of the message
	int flag;		//flag indicate whether message is delivered
} message;


struct message* create_msg(int unique_identifier, const char* sender, const char* receiver, const char* content){
	int datetime_sending;
	int flag = 0;		// 0 indicate message not delivered and 1 message delivered
	datetime_sending = (int)time(NULL);
	
	// populate the message strucut
	message* new_message = (message*)malloc(sizeof(message));
	if (new_message == NULL) {
		printf("Error while allocating the memory for new_message in create_msg\n");
		return NULL;
	}
	new_message->unique_identifier = unique_identifier;
	new_message->timesent = datetime_sending;
	new_message->sender = strdup(sender);
	new_message->receiver = strdup(receiver);
	new_message->content = strdup(content);
	new_message->flag = flag;
	return new_message;
}

void free_msg(message* message) {
	/**
	Free memory allocated in message.
	*/
	free(message->sender);
	free(message->receiver);
	free(message->content);
	free(message);
}

void store_msg(const message* message) {
	/**
	Functions to store message in disk. Based on some research on the internet,
	one way to store data in disk is through file I/O. 
	https://towardsdatascience.com/how-your-data-is-stored-on-disk-and-memory-8842891da52
	Therefore, I'll read the message
	then stored it as a txt file with its unique_id as its name.
	*/
	int unique_identifier = message->unique_identifier;
	char filename[20];	// assuming file name has max of 20 character
	snprintf(filename, sizeof(filename), "%d.txt", unique_identifier);
	
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		printf("Unable to open the %s to write\n", filename);
		return;
	}
	
	fprintf(file, "unique_id is: %d\n", message->unique_identifier);
	fprintf(file, "timesend is: %d\n", message->timesent);
	fprintf(file, "sender is: %s\n", message->sender);
	fprintf(file, "receiver is: %s\n", message->receiver);
	fprintf(file, "content is: %s\n", message->content);
	fprintf(file, "flag is: %d\n", message->flag);
	
	fclose(file);
}

struct message* retrieve_msg(int unique_identifier) {
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
		printf("Unable to open the %s to read\n", filename);
		exit(1);
	}
	
	// read data from file and populate the message structure
	char line[5000];
	int id, timesent, flag;
	char sender[100], receiver[100], content[1000];
	// initalized message
	message* new_message = (message*)malloc(sizeof(message));
	if (new_message == NULL) {
		printf("Error while allocating the memory for new_message in create_msg\n");
		return NULL;
	}
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
	new_message->unique_identifier = id;
	new_message->timesent = timesent;
	new_message->sender = ptr_sender;
	new_message->receiver = ptr_receiver;
	new_message->content = ptr_content;
	new_message->flag = 0;
	fclose(file);
	return new_message;
}

void print_msg(const message* message) {
	printf("unique_id is: %d\n", message->unique_identifier);
	printf("timesend is: %d\n", message->timesent);
	printf("sender is: %s\n", message->sender);
	printf("receiver is: %s\n", message->receiver);
	printf("content is: %s\n", message->content);
	printf("flag is: %d\n", message->flag);
	printf("\n");
}

void main() {
	// test to see if create_msg, create the message successfully
	message* message1 = create_msg(1, "leaksmy_heng", "mey_mey", "hello this is a test only");
	message* message2 = create_msg(2, "leaksmy_heng@gmail.com", "me@gmail.com", "hello this is a test only Best,Leaksmy");
	message* message3 = create_msg(3, "leaksmy_heng@gmail.com,leaksmy@gmail.com", 
										"mey_mey@gmail.com,meh@yahoo.com",
										"Hello, are you doing okay? Test123 Best,Leaksmy Heng");
	
	// store the message
	store_msg(message1);
	store_msg(message2);
	store_msg(message3);
	
	// retrive the message
	message* retreive_message1 = retrieve_msg(1);
	print_msg(retreive_message1);
	message* retreive_message2 = retrieve_msg(2);
	print_msg(retreive_message2);
	message* retreive_message3 = retrieve_msg(3);
	print_msg(retreive_message3);
	
	// free message after storing
	free_msg(message1);
	free_msg(message2);
	free_msg(message3);
	free_msg(retreive_message1);
	free_msg(retreive_message2);
	free_msg(retreive_message3);
}
