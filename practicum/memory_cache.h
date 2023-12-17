#ifndef MEMORY_CACHE_H
#define MEMORY_CACHE_H

#include "stdbool.h"
#include <stdio.h>

#define MAX_LISTS 16

typedef struct node {
	message data;
	struct node* next;
	struct node* prev;
} node;


typedef struct {
	node* head;
	node* tail;
	size_t count;
} linkedlist;


typedef struct {
	linkedlist lists[MAX_LISTS];
} cache;

node* createnode(message data);
void removerandomnode(linkedlist* list);
void removeleastrecentlynode(linkedlist* list);
void insertnode(linkedlist* list, message data, bool);
struct message searchcache(linkedlist* list, int);
void printlinkedlist(linkedlist* list);
void store_msg(linkedlist* list, message data, bool);
struct message retrieve_msg(linkedlist* list, int);

#endif
