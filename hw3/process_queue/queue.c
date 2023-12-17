#include <stdio.h>
#include <stdlib.h>

// Define a struct process_t
typedef struct process_t {
	int identifier;
	char *name;
	long cumulative_runtime;
	int priority;
} process_t;


// Define a struct node
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


void initialize_queue(queue_t *queue) {
	/**
	Initialize the queue by having it point the front and back to NULL and the size to 0.
	*/
    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;
}


int is_empty_queue(const queue_t *queue) {
	/**
	Function whether to check whether the queue is empty or not. If empty, return 1 otherwise return 0.
	*/
	int queue_size = queue->size;
	if (queue_size == 0) {
		return 1;
	}
	return 0;
}


void add2q(queue_t *queue, void *element) {
	/**
	Function to add element into queue.
	*/
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL) {
        printf("Error while allocating the memory.\n");
        exit(1);
    }
    node->data = element;
    node->next = NULL;
	
	// if queue is empty, initialize the front and back to the node
    if (is_empty_queue(queue)) {
        queue->front = node;
        queue->back = node;
        node->prev = NULL;
    } 
    // modify the node and queue accordingly when there is already node in the queue
    else {
        node->prev = queue->back;
        queue->back->next = node;
        queue->back = node;
    }

    queue->size++;	// increment the queue size
}


int qsize(queue_t* queue) {
	return queue->size;
}


void* popQ(queue_t* queue){
	/**
	Function to removes and returns the data within the element at the front of the queue.
	*/
	// check if the queue is empty
	if (is_empty_queue(queue)) {
		printf("queue is already empty so can't pop anything\n");
		return NULL;
	}
	// if queue is not empty then store the front data as data
	void *data = queue->front->data;
	
	// Update the front pointer to the next element
    node_t *temp = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        // If the front becomes NULL, the queue is now empty, so reset the rear pointer.
        queue->back = NULL;
    } else {
        // Update the previous pointer of the new front node.
        queue->front->prev = NULL;
    }

    free(temp);		// free the node
    queue->size--;	// decrement the size
    return data;
}

process_t* rmProcess(queue_t* queue) {
	/**
	Function that removes and returns the process with the highest priority in the queue.
	*/
	// check if the queue is empty
	if (is_empty_queue(queue)) {
		printf("queue is already empty so can't pop anything\n");
		return NULL;
	}
	node_t* current_node = queue->front;
	node_t* node_to_be_remove = NULL;		// initialize this to null as of now
	process_t* highest_priorit_process = NULL;		// initialize this to null as of now
	int highest_priority = 0;				// 0 is the lowest priority
	
	// find the process with the highest_priority
	while (current_node != NULL) {
		// check if the current node data is null or not
		if (current_node->data != NULL) {
			process_t* process = (process_t*)current_node->data;
			
			// if we are able to find the higher priority
			if (process->priority > highest_priority) {
                highest_priority = process->priority;	// reassign higher priority to the process priority
                node_to_be_remove = current_node;		// reassign node_to_be_remove the the current_node
                highest_priorit_process = process;		// reassign higher priority process to the process
            }
		}
		current_node = current_node->next;
	}
	
	// after finding it, we remove it now
	if (node_to_be_remove != NULL) {
		if (node_to_be_remove == queue->front) {
			queue->front = node_to_be_remove->next;
            if (queue->front == NULL) {
                queue->back = NULL;
            } 
            else {
                queue->front->prev = NULL;
            }
        } 
        else if (node_to_be_remove == queue->back) {
            queue->back = node_to_be_remove->prev;
            queue->back->next = NULL;
        } 
        else {
            node_to_be_remove->prev->next = node_to_be_remove->next;
            node_to_be_remove->next->prev = node_to_be_remove->prev;
        }

        free(node_to_be_remove);
        queue->size--;
    }
	//printf("Identifier: %d\n", highest_priorit_process->identifier);
    //printf("Name: %s\n", highest_priorit_process->name);
    //printf("Cumulative Runtime: %ld\n", highest_priorit_process->cumulative_runtime);
    //printf("Priority: %d\n", highest_priorit_process->priority);
    return highest_priorit_process;
}


void free_queue(queue_t *queue) {
	/**
	Function to free the queue because we allocate that in the stack.
	*/
    while (!is_empty_queue(queue)) {
        node_t *temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
        queue->size--;
    }
    queue->back = NULL; // Reset the back pointer
}

