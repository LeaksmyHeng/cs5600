/**
Name: Leaksmy Heng
Assignment: Create Cipher
Class: CS5600 Fall 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polybius.h"

int main(int argc, char* argv[]) {
	/**
	main function to run encoding table. All the test case will be in dumper.sh
	*/
	if (argc < 3) {
		printf("Need 3 arguments! Not %d\n", argc);
        return 1;
    }

    char* option = argv[1];
    char* text = argv[2];
    
    if (argc > 3) {
    	for (int i=2; i<argc; i++) {
    		char* text = argv[i];
    		int numOfLetter = 25;
    		if (strcmp(option, "-e") == 0) {
    			struct pb_node* table = populate_pb_table(numOfLetter);
    			char *pbEncodeResult = pbEncode(text, table);
    			free(pbEncodeResult);
    			free(table);
    			} 
    		else if (strcmp(option, "-d") == 0) {
    			struct pb_node* table = populate_pb_table(numOfLetter);
				char *pbDecodeResult = pbDecode(text, table);
				free(pbDecodeResult);
				free(table);
    			} 
    		else {
        		printf("Invalid option. Use -e for encryption or -d for decryption.\n");
        		return 1;
        		}
    	}
    }
    return 0;
}

