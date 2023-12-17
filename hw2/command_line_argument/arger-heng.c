#include <stdio.h>
#include "helper-arger-heng.h"

int convert_string_to_all_upper(int index, int argc, char* argv[]) {
	/**
	* converting string to all upper case. String is actually the argument parse from
	* the main function. 
	* param index: the index in which you want to start convert to all upper.
	* param argc: the total number of arguments.
	* param argv: string to be converted to all upper.
	*/
	char buffer[75];		// assuming we take no more than 75 char as arguments
	char *result =buffer;
	int counter = 0;
	for (int i = index; i < argc; i++) {
    	size_t j = 0;
    	char * string = argv[i];
        while (string[j] != '\0') {
        	int ascii_char = string[j];
        	if (ascii_char >= 97 && ascii_char <= 122) {
        		int int_upper = ascii_char - 32;
        		char big_char = (char) int_upper;
        		*result++ = big_char;
        	}
        	else {
        		char big_char = string[j];
        		*result++ = big_char;
        		}
        	counter += 1;
    		j++;
        }
    	*result++ = ' ';	
    	counter += 1;
    }
    *--result = '\0';
    printf("%s\n",buffer);
    
    return 0;
}

int convert_string_to_all_lower(int index, int argc, char* argv[]) {
	/**
	* converting string to all lower case. String is actually the argument parse from
	* the main function. 
	* param index: the index in which you want to start convert to all upper.
	* param argc: the total number of arguments.
	* param argv: string to be converted to all upper.
	*/
	char buffer[64];
	char *result =buffer;
	int counter = 0;
	for (int i = index; i < argc; i++) {
    	size_t j = 0;
    	char * string = argv[i];
        while (string[j] != '\0') {
        	int ascii_char = string[j];
        	if (ascii_char >= 65 && ascii_char <= 90) {
        		int int_lower = ascii_char + 32;
        		char small_char = (char) int_lower;
        		*result++ = small_char;
        	}
        	else {
        		char small_char = string[j];
        		*result++ = small_char;
        		}
        	counter += 1;
    		j++;
        }
    	*result++ = ' ';	
    	counter += 1;
    }
    *--result = '\0';
    printf("%s\n",buffer);
    
    return 0;
}

int convert_string_to_cap(int index, int argc, char* argv[]) {
	/**
	* converting string to cap. String is actually the argument parse from
	* the main function. 
	* param index: the index in which you want to start convert to all upper.
	* param argc: the total number of arguments.
	* param argv: string to be converted to all upper.
	*/
	char buffer[75];		// assuming we take no more than 75 char as arguments
	char *result =buffer;
	int counter = 0;
	for (int i = index; i < argc; i++) {
    	size_t j = 0;
    	char * string = argv[i];
    	int while_loop_counter = 0;
        while (string[j] != '\0') {
        	int ascii_char = string[j];
        	if (ascii_char >= 97 && ascii_char <= 122 && while_loop_counter == 0) {
        		int int_upper = ascii_char - 32;
        		char big_char = (char) int_upper;
        		*result++ = big_char;
        	}
        	else {
        		// if big letter, convert to small letter
        		if (ascii_char >= 65 && ascii_char <= 90 && while_loop_counter != 0) {
        			int letter = ascii_char + 32;
        			char small_letter = (char) letter;
        			*result++ = small_letter;
        		}
        		else {
        		char big_char = string[j];
        		*result++ = big_char;
        		}
        	}
        	while_loop_counter += 1;
        	counter += 1;
    		j++;
        }
    	*result++ = ' ';	
    	counter += 1;
    }
    *--result = '\0';
    printf("%s\n",buffer);
    
    return 0;
}

// defining main with arguments
int main(int argc, char* argv[]) {

	// if there is no argument parsing in (meaning argument is less than 3), exit the code with -1
	if (argc < 3) { 
		printf("Expected at least 3 arguments no less!\n");
		return -1;
		}
	
	char* first_arg = argv[0];		// grab 0th argument which should be arger-heng
	char* first_arg_correct = "./arger-heng";
	char* sec_arg = argv[1];		// grab the second argument used in if else
	char* upper = "-u";
	char* lower = "-l";
	char* cap = "-cap";
	int starting_index = 2;
	
	int compare_zero_arg = string_comparison(first_arg, first_arg_correct);
	if (compare_zero_arg != 0) {
		printf("The zero argument has to be ./arger-heng and not %s!\n", first_arg);
		return -1;
	}
	
	int string_comp_upper = string_comparison(sec_arg, upper);
	if (string_comp_upper == 0) {
		//printf("converting string to upper case\n");
		convert_string_to_all_upper(starting_index, argc, argv);
		return 0;
	}
	
	int string_comp_lower = string_comparison(sec_arg, lower);
	if (string_comp_lower == 0) {
		//printf("converting string to lower\n");
		convert_string_to_all_lower(starting_index, argc, argv);
		return 0;
	}
	
	int string_comp_cap = string_comparison(sec_arg, cap);
	if (string_comp_cap == 0) {
		//printf("converting string to cap\n");
		convert_string_to_cap(starting_index, argc, argv);
		return 0;
	}
	
	// if second argument is not upper, lower, or cap; return -1
	printf("The argument parsing in is neither -u, -l or -cap.\n");
	return -1;
}
