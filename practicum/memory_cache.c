#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "memory_simulation.h"

#define MAX_LISTS 16

// define node for a doubly linked list
typedef struct node {
	message data;
	struct node* next;
	struct node* prev;
} node;


// define a doubly linked list
typedef struct {
	node* head;
	node* tail;
	size_t count;		// this is to count the number of linked list
} linkedlist;


// define a cache with 16 linked lists
typedef struct {
	linkedlist lists[MAX_LISTS];
} cache;


node* createnode(message data) {
	/**
	Function to create node that will be used to store in the linked list
	*/
	node* newnode = (node*)malloc(sizeof(node));
	if (newnode == NULL) {
		printf("Error while allocating the memory for new node in cache\n");
		exit(0);
	}
	newnode->data = data;
	newnode->next = NULL;
	newnode->prev = NULL;
	return newnode;
}


void removerandomnode(linkedlist* list) {
	/**
	Remove random node from the doubly linked list.
	*/
	
	// if list is empty, there is nothing to remove
	if (list->count == 0) {
		return;
	}
	
	// generate random index
	srand((unsigned int)time(NULL));
	size_t indextoremove = rand() % list->count;
	
	// rearrange the linked list
	node* current = list->head;
	for (size_t i=0; i<indextoremove; i++) {
		current = current->next;
	}
	if (current->prev != NULL) {
		current->prev->next = current->next;
	}
	// remove first node
	else {
		list->head = current->next;
	}
	if (current->next != NULL) {
		current->next->prev = current->prev;
	}
	// remove last node
	else {
		list->tail = current->prev;
	}
	
	// free current
	free(current);
	// decrement element in list
	list->count--;
}


void removeleastrecentlynode(linkedlist* list) {
	/**
	Function to remove the node from the doubly linked list using leasat recently node.
	*/
	
	// if there is no element in the list, just return without interrupt the program
	if (list->count==0) {
		return;
	}
	
	// rearrange the node
	node* temp = list->tail;
	list->tail = temp->prev;
	if (list->tail != NULL) {
		list->tail->next = NULL;
	}
	// remove last node
	else {
		// 
		list->head = NULL;
	}
	free(temp);
	list->count--;
}


void insertnode(linkedlist* list, message data, bool is_randomnode) {
	/**
	Function to insert node into linked list. Insert at the beginning of a linked list.
	We implemented 2 algorithm in here, randomnode and LRU.
	- Random node means if the number of node in the cache hits its maximum, we will
	remove one random node out
	- LRU means if the number of node in the cache hits its maximum, we will
	remove the least recently.
	
	:param list: the linkedlist that we used to store message
	:param data: message or data that will be used to store in the linkedlist
	:param is_randomnode: a flag to indicate whether we are going to implement randomnode or LRU
		- True means randomnode
		- False means LRU
	*/
	
	// check if the number of list node is already full meaning 16 nodes in linkedlist
	// if it is, check if it is implementing randomnode
	if (list->count >= MAX_LISTS) {
		//printf("Linkedlist is too full. Remove one node now\n");
		if (is_randomnode == true) {
			removerandomnode(list);
		}
		// otherwise meaning it is implementing LRU
		else {
			removeleastrecentlynode(list);
		}
	}
	
	// create node
	node* newnode = createnode(data);
	
	// update the access order
	if (list->head == NULL) {
		// in case of the first node
		list->head = newnode;
		list->tail = newnode;
	}
	else {
		newnode->next = list->head;
		list->head->prev = newnode;
		list->head = newnode;
	}
	list->count++;
}

struct message searchcache(linkedlist* list, int unique_identifier) {
	/**
	Function to search for message id in cache aka doubly linked list.
	Return the message with its content if the message is found in cache
	Otherwise return the message with flag, unique_id and time = 0 and sender and receiver=""
	
	:param list: list that is used to store node (aka cache)
	:param unique_identifier: message unique identifier that we are searching for
	*/
	
	// check if list is NULL if it is print the message warning that
	// there is nothing in cache and exit the program
	if (list == NULL) {
		printf("There is nothing populated in cache yet\n");
		exit(1);
	}
	
	node* current = list->head;
	while (current != NULL) {
		// loop through to check for unique identifier in the cache
		// if found, rearrange the linked list by move the access node to the front
		if (current->data.unique_identifier == unique_identifier) {
			if (current->prev != NULL) {
				current->prev->next = current->next;
			}
			else {
				// access node is already the first node
				list->head = current->next;
			}
			
			if (current->next != NULL) {
				current->next->prev = current->prev;
			}
			else {
				// access node is the last node
				list->tail = current->prev;
			}
			current->next = list->head;
			current->prev = NULL;
			if (list->head != NULL) {
				list->head->prev = current;
			}
			list->head = current;
			
			// update tail
			if (current->next == NULL) {
				list->tail = current;
			}
			return current->data;
		}
		current = current->next;
	}
	// if cannot find the data in cache; return the default value here
	// then check in disk! Do not interrupt the program though
	return (message){.unique_identifier=0, .timesent=0, .sender="", .receiver="", .content="", .flag=0};
}


void printlinkedlist(linkedlist* list) {
	// check if list is NULL, print warning message
	// and return. Do not interrupt the program
	if (list == NULL) {
		printf("There is nothing in your cache yet\n");
		return;
	}

	node* current = list->head;
	while (current != NULL) {
		printf("Message id is %d\n", current->data.unique_identifier);
		current = current->next;
	}
	printf("\n");
}


void store_msg(linkedlist* list, message data, bool is_randomnode){
	/**
	This method is used to store message in both disk and cache. In cache implementation, we
	implemented two algorithm: LRU and randomnode. Therefore, I put a flag here, if the is_randomnode:
	 - equal to True: use is_randomnode algorithm in insertnode
	 - equal to False: use LRU algorithm in insert node
	 
	 :param list: cache
	 :param data: message that will be used to store in cache implemented in linkedlist or disk through txt file
	 :param is_randomnode: a flag to indicate whether we want to implement LRU or randomnode in cache
	*/
	
	// does not have to check if list is NULL here cause we are going to populate node into list
	// store message in cache
	insertnode(list, data, is_randomnode);
	// store message in disk
	store_msg_in_disk(data);
}


struct message retrieve_msg(linkedlist* list, int unique_identifier){
	/**
	Retreive the message from cache or disk using unique_identifier.
	*/
	message data = searchcache(list, unique_identifier);
	if ((data.unique_identifier==0) && (data.timesent==0) && (data.flag==0)) {
		printf("Message was not able to find in cache. Therefore, checking in disk now\n");
		
		// I do not have to put another check for if it can't find in both disk and cache
		// because in retrieve_msg_from_disk, if I can't find the file,
		// it will exit the program
		message disk_data = retrieve_msg_from_disk(unique_identifier);
		return disk_data;
	}
	// update the flag in cache to 1 since we are retrieving it
	data.flag = 1;
	return data;
}

