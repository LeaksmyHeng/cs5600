#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int *dataArray;		// pointer to integer array
	size_t size;		// size of the dataArray
} dynBlock;


dynBlock* allocDynBlock(size_t size) {
	/**
	* Write a function that  allocates a single instance of a dynBlock from the heap and returns a pointer to that allocated object.
	*/
	
	dynBlock* singleBlock = (dynBlock*)malloc(sizeof(dynBlock));	// reserve memory for a single instance of dynBlock
	if (singleBlock == NULL)	// check if memory is allocated correctly
	{
		printf("Memory allocated in allocDynBlock failed.\n");
		exit(-1);
	}
	
	//singleBlock is a pointer of dynBlock which is a structure. So with singleBlock, we use -> to access dataArray.
	// we create a dynamic memory size to store the array
	singleBlock->dataArray = (int*)malloc(size*sizeof(int));
	if (singleBlock->dataArray == NULL) {
		printf("Memory allocated for dataArray in dynBlock failed.\n");
		exit(-1);
	}
	singleBlock->size = size;
	return singleBlock;
}

void storeMem2Blk(size_t size, int *dataArray, dynBlock* singleBlock) {
	/**
	* Create a function that stores an array of integers in a dynamic block (a dynBlock object) previously allocated.
	*/
	
	if (singleBlock->size < size) // check size of the array and the size input
	{
		printf("number of size is greater than the number of array size allocated in dynBlock\n");
		exit(-1);
	}
	for (int i=0; i<size; i++) {
		singleBlock->dataArray[i] = dataArray[i];
	}
}

void freeDynBlock(dynBlock* singleBlock) {
	/**
	free memory
	*/
	free(singleBlock->dataArray);
	free(singleBlock);
}

void readIntFromLine(char *input) {
	/**
	* read line in a file and grab the number. Then store each number in the array in dynBlock.
	*/
	int count = 0;
	int tempDataToStore[50];
	
	// need to scan each line
	const char *token;
	token = strtok(input, " ");	// Use strtok to split the string by space
	while (token != NULL) {		// Loop through the tokens
	    //printf("%d -> %s\n", count, token);
	    int x;
	    sscanf(token, "%d", &x);
	    tempDataToStore[count] = x;
	    count++;
	    token = strtok(NULL, " ");
	}
	
	//copy to new array now that we know the total number of size
	int dataToStore[count];
	size_t blockSize=count;
	for (int i=0; i<count; i++) {
		dataToStore[i] = tempDataToStore[i];
	}
	
	dynBlock* block = allocDynBlock(blockSize);			// allocate dynBlock
	storeMem2Blk(blockSize, dataToStore, block);		// store data in dynBlock
	
	// Access and print the data
    printf("Data stored in dynBlock:\n");
    for (size_t i = 0; i < block->size; i++) {
        printf("%d\n", block->dataArray[i]);
    }
    freeDynBlock(block);	//free the allocated memory afterward
}

void main() {
	char const* const fileName = "blocks.data";
	FILE* file = fopen(fileName, "r"); 
	
	if (file == NULL || !file) {
		printf("blocks.data file has no content!\n");
		exit(-1);
	}
	char input[500];
	int counter = 0;
	int num;
	// Create an array of dynBlocks
	while (fgets(input, sizeof(input), file)) {
		readIntFromLine(input);
		counter+=1; 
	}
	fclose(file);
}
