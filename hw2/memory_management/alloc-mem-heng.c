#include <stdlib.h>
#include <stdio.h>

/**
* Reading from:
* https://craftofcoding.wordpress.com/2015/12/07/memory-in-c-the-stack-the-heap-and-static/
*/


/**
Static memory refers to the variable that are stored outside of a function or the variable that are stored inside of the function but declare static.
Generally static memory is allocated at the beginning of the program.
Static memory has a global scope, it could be accessed from any part of the program.
Static memory are freed or deallocated automatically when the program exits.
Static memory is tracked by OS and managed by the program's runtime environment. 
The location of the static memory is commonly at the beginning of RAM area.
*/
char staticMemory[5*1024*1024];

void allocatingStackMemory() {
	/**
	Stack is a blocked of memory that is used to stored the local variables.
	As the example below, stackMemory is stored in a stack.
	Stack memory is managed using a stack data structure (LIFO-last in first out).
	This means that when there is new variable created, it will be pushed onto the stack.
	The last variable that are stored is the first to be freed.
	When the function finishes executed or exit, the memory is freed up automatically. 
	The variables allocated on the stack are typically not accessible outside the function in which they are defined.
	*/
	char stackMemory[1*1024*1024];
	printf("Allocating 1MB from the stack.\n");
}

void allocatingHeapMemory() {
	/**
	Heap is the memory that can be used dynamically. 
	However, you have to explicitly allocate it and track that.
	You also have to deallocate the heap memory (free); otherwise,there will be a memory leak issue.
	This means the memory is not returned to the OS even after the program exists.
	This means that the memory allocates in heap has a longer lifetime than stack or static.
	It persists until explicitly freed or deallocated.
	The variables that are stored in heap can have a broader scope and can be accessd across functions if pointers to memory are passed or shared.
	*/
	char *heapMemory = (char *)malloc(10*1024*1024);
	if (heapMemory == NULL) {
		printf("Allocating 10MB from the heap failed!\n");
		exit(-1);
	}
	printf("Allocating 10MB from the heap.\n");
}

int main() {
	allocatingHeapMemory();
	printf("Allocating 5MB from the static segment.\n");
	allocatingStackMemory();
}
