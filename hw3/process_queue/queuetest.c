#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main() {
    // initialize a queue_t
    queue_t queue;
    initialize_queue(&queue);

    // Test adding element to queue by checking its qsize and print out to see if it is correct
    printf("Test adding element\n");
    int intValue = 1998;
    double doubleValue = 1998.2546;
    float floatValue = 3.14;
    char *stringValue = "Leaksmy Heng";
    
    int queue_size0 = qsize(&queue);
	printf("Expected qsize of 0 getting %d\n", queue_size0);
    add2q(&queue, &intValue);
    int queue_size1 = qsize(&queue);
    printf("Expected qsize of 1 getting %d\n", queue_size1);
    add2q(&queue, &doubleValue);
    int queue_size2 = qsize(&queue);
	printf("Expected qsize of 2 getting %d\n", queue_size2);
    add2q(&queue, &floatValue);
    int queue_size3 = qsize(&queue);
    printf("Expected qsize of 3 getting %d\n", queue_size3);
    add2q(&queue, stringValue);
    int queue_size4 = qsize(&queue);
    printf("Expected qsize of 4 getting %d\n", queue_size4);
	printf("-------------------Print queue after adding element into it-------------------\n");
    node_t *current = queue.front;
    while (current != NULL) {
        void *element = current->data;

        if (element == NULL) {
            printf("NULL\n");
        } else if (element == &intValue) {
            printf("int value: %d\n", *(int *)element);
        } else if (element == &doubleValue) {
            printf("double value: %lf\n", *(double *)element);
        } else if (element == stringValue) {
            printf("string value: %s\n", (char *)element);
        } else if (element == &floatValue) {
            printf("float value: %f\n", *(float *)element);
        }

        current = current->next; // Move to the next element in the queue
    }
	
	// check popq
	printf("\n\nTesting popq\n");
	void *element = popQ(&queue);
	int queuesize_pop1 = qsize(&queue);
	printf("Expected qsize of 3 getting %d (cause of poping intvalue %d)\n", queuesize_pop1, *(int *)element);
	void *element2 = popQ(&queue);
	int queuesize_pop2 = qsize(&queue);
	printf("Expected qsize of 2 getting %d (cause of poping doublevalue %lf)\n", queuesize_pop2, *(double *)element2);
	printf("----------Remaining Queue--------------\n");
	node_t *currentnew = queue.front;
    while (currentnew != NULL) {
        void *element = currentnew->data;

        if (element == NULL) {
            printf("NULL\n");
        } else if (element == &intValue) {
            printf("int value: %d\n", *(int *)element);
        } else if (element == &doubleValue) {
            printf("double value: %lf\n", *(double *)element);
        } else if (element == stringValue) {
            printf("string value: %s\n", (char *)element);
        } else if (element == &floatValue) {
            printf("float value: %f\n", *(float *)element);
        }

        currentnew = currentnew->next; // Move to the next element in the queue
    }
    
    // adding some process to the new queue to test
    printf("\n\ntest rmProcess\n");
    process_t *process1 = malloc(sizeof(process_t));
	process1->identifier = 1; // Assign a unique identifier
	process1->name = "Process 1"; // Assign a name
	process1->cumulative_runtime = 1000; // Assign cumulative runtime
	process1->priority = 3; // Assign a priority
	
	process_t *process2 = malloc(sizeof(process_t));
	process2->identifier = 2; // Assign a unique identifier
	process2->name = "Process 2"; // Assign a name
	process2->cumulative_runtime = 2000; // Assign cumulative runtime
	process2->priority = 4; // Assign a priority
	
	process_t *process3 = malloc(sizeof(process_t));
	process3->identifier = 3; // Assign a unique identifier
	process3->name = "Process 3"; // Assign a name
	process3->cumulative_runtime = 3000; // Assign cumulative runtime
	process3->priority = 5; // Assign a priority

	// Enqueue the process into the queue
	queue_t process_queue;
    initialize_queue(&process_queue);
	add2q(&process_queue, process1);
	add2q(&process_queue, process2);
	add2q(&process_queue, process3);
	printf("Expected qsize of 3 getting %d\n", qsize(&process_queue));
	process_t* p = rmProcess(&process_queue);
	if (p != NULL) {
		printf("Removed process with highest priority:\n");
		printf("Identifier: %d\n", p->identifier);
		printf("Name: %s\n", p->name);
		printf("Cumulative Runtime: %ld\n", p->cumulative_runtime);
		printf("Priority: %d\n", p->priority);
		free(p); // Free the memory of the removed process
		}
	
	free_queue(&queue);
	
	// free the other process
	free(process2);
	free(process1);
	free_queue(&process_queue);
    return 0;
}
