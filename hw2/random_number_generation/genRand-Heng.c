#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genRandHelper-Heng.h"

int main(int argc, char* argv[]) {
	// check if command line arguments are more than 2 or less than 2
	if (argc < 3) {
		printf("Only need 2 or 3 arguments!\n");
		return -1;
	}
	if (argc > 4) {
		printf("Only need 2 or 3 arguments!\n");
		return -1;
	}
	
	// assuming the 1st arg is positive number and 2nd is filename
	char *number = argv[1];
	char *filename = argv[2];
	// check if the first arg is actually a number
	int digit = checkIfDigit(number);
	if (digit == -1) {
		printf("The first arg need to a positive number!\n");
		return -1;
	}
	
	int append = 0;		// 0 is overwrite
	// if there is the 3 third agument to append the file, then open file as append otherwise open as overwrite
	if (argc == 4 && strcmp(argv[3], "-a") == 0) {
		append = 1;
	}
	
	FILE *file;
	if (append == 1) {
		printf("open file and append content to it.\n");
		file = fopen(filename, "a");
	}
	else {
		printf("open file and overwrite content to it.\n");
		file = fopen(filename, "w");
	}
	if (file == NULL) {
		printf("Error while opening file\n");
		return -1;
	}
	for (int i=0; i<digit; i++) {
		int randGen = genRand(1, 100);	// generate random number and write to file
		printf("write %d\n", randGen);
		fprintf(file, "%d\n", randGen);
	}
	fclose(file);
	return 0;
}
