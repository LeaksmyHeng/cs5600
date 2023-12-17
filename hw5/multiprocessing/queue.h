#ifndef QUEUE_H
#define QUEUE_H

// Define a struct process_t
typedef struct process_t {
	int identifier;
	char *name;
	long cumulative_runtime;
	int priority;
} process_t;

// Define a struct node_t
typedef struct node_t {
    void *data;
    struct node_t *next;
    struct node_t *prev;
} node_t;

// Define the queue_t struct
typedef struct {
    node_t *front;
    node_t *back;
    int size;
} queue_t;

#endif

void initialize_queue(queue_t*);

int is_empty_queue(const queue_t*);

void add2q(queue_t*, void*);

int qsize(queue_t* );

void* popQ(queue_t*);

void free_queue(queue_t *);

process_t* rmProcess(queue_t*);
