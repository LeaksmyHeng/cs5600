#include <stdio.h>

int string_comparison(char* string_one, char* string_two) {
	/*
	*comparing 2 string to see if it is the same
	*/
	int flag = 0; // initialized the flag. 0 mean the strings are the same
	
	while (*string_one != '\0') {
		if (*string_one != *string_two) {
			flag = 1;
		}
		*string_one ++;
		*string_two ++;
	}
	if (*string_one == '\0' && *string_two != '\0') {
		return -1;
	}
	else {
		if (flag == 0) {return 0;}
		else {return -1;};
	}
}


/**
int  main() {
	char * test1 = "hello";
	char * test2 = "hello";
	char * test3 = "beam";
	char * test4 = "my  name is leaksmy";
	char * test5 = "hell";
	char * test6 = "124";
	char * test7 = ".,12";
	
	printf("Expected 0, got %d\n", string_comparison(test1, test2));
	printf("Expected -1, got %d\n", string_comparison(test1, test3));
	printf("Expected -1, got %d\n", string_comparison(test1, test4));
	printf("Expected -1, got %d\n", string_comparison(test1, test5));
	printf("Expected -1, got %d\n", string_comparison(test1, test6));
	printf("Expected -1, got %d\n", string_comparison(test1, test7));
	printf("Expected -1, got %d\n", string_comparison(test5, test2));
}
*/
