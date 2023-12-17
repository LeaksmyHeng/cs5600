/**
Name: Leaksmy Heng
Assignment: Create Cipher
Class: CS5600 Fall 2023
*/

# include <stdio.h>
# include <stdlib.h>

typedef struct pb_node {
	char letter;
	char row;
	char col;
} pb_node;

struct pb_node* populate_pb_table(int numElement) {
	/**
	Populate PB table. In this assignment, we will only encode english letter.
	All the number and special character are ignore.
	Letter i and j are in the same location of the 2D array.
	*/
	struct pb_node* pb_table = (struct pb_node*)malloc(sizeof(struct pb_node) * numElement);
	if (pb_table == NULL) {
		printf("Error while allocate memory for pb_table");
		exit(1);
	}
	int rows = 5;	// max row in pb table = 5
	int cols = 5;	// max col in pb table = 5
	int counter = 65;
	
	for (int row = 1; row<=rows; row++) {
		for (int col = 1; col<= cols; col++ ) {
			// letter i and j are usually combined hence share the same row ^ col num.
			// Since row already the same, decrement col to be the same
			if (counter == 74) {
				col = col - 1;
			}
			char letter = (char) counter;
			pb_table[counter-65].letter = letter;
			pb_table[counter-65].row = (char)row;
			pb_table[counter-65].col = (char)col;
			counter = counter + 1;
		} 
	}
	
	return pb_table;
}

const struct pb_node* isCharInStructArray(const struct pb_node* table, char target) {
	/**
	Check if the target character is in the table or not. If it is, return the location of that character, else return NULL.
	*/
	int numOfLetter = 26;
	for (size_t i =0; i<numOfLetter; i++) {
		const char currletter = table[i].letter;
		if (currletter == target) {
			return table + i;
		}
	}
	return NULL;
}

char* pbEncode(const char *plaintext, struct pb_node* table) {
	/**
	Function to encode the text. Each character in the text will be encoded based on the table.
	If the character is not letter (example space or ,.?/ and so on), convert it into space.
	Example: "Hello 123!Smy" => "2315313134    433254"
	*/
	// loop through each character in const char * and get the size of the plaintext
	int size = 0;
	for (int i=0; plaintext[i] != '\0'; i++) {
		int char_dec = plaintext[i];
		if ((char_dec >= 97 && char_dec <= 122) || (char_dec >= 65 && char_dec <= 90)) {
			size = size + 2;
		}
		else {size = size + 1;}
	}
	
	//initialized character array  
	char* char_array = (char*)malloc(size * sizeof(char));
	if (char_array == NULL) {
		printf("Memory allocation failed");
		exit(1);
	}
	int counter = 0;
	for (int i=0; plaintext[i] != '\0'; i++) {
		char character = plaintext[i];
		int char_dec = plaintext[i];
		
		// check if character is in lowercase, if so, convert to upper
		if (char_dec >= 97 && char_dec <= 122) {
			char_dec = char_dec - 32;
			character = (char) char_dec;
		}
		
		const struct pb_node* result = isCharInStructArray(table, character);
		if (result!=NULL) {
			//printf("%d, %d\n", result->row, result->col);
			char_array[counter] = result->row + '0';
			counter = counter + 1;
			char_array[counter] = result->col + '0';
			counter = counter + 1;
		}
		else {
			char_array[counter] = character;
			counter = counter + 1;
			}
		}
	char_array[size+1]='\0';
	printf("%s", char_array);
	return char_array;
}

const struct pb_node* isRowColInStructArray(const struct pb_node* table, char row, char col) {
	int numOfLetter = 26;
	
	for (int i =0; i<numOfLetter; i++) {
		const char target_row = table[i].row;
		const char target_col = table[i].col;
		//printf("comparing %d - %d with %d -%d\n", target_row, target_col, row, col);
		if (target_row == row && target_col == col) {
			//printf("row/col %d & %d found => letter: %c\n", row, col, table[i].letter);
			return table + i;
		}
	}
	return NULL;
}

char* pbDecode(const char *ciphertext, struct pb_node* table) {
	/**
	Decode the ciphertext. Assuming there are only number and space in the ciphertext (based on how we encoded in pbEncode,there can
	only be space and numeric).
	*/
	// loop through each character in const char * and get the size of the ciphertext
	int size = 0;
	int counter = 0;
	for (int i=0; ciphertext[i] != '\0'; i++) {
		int ascii_num = ciphertext[i];
		if (ascii_num >= 48 && ascii_num <= 57) {
			counter = counter + 1;
			// if counter = 2 that means we get a character now (counter = 2 is equivalent to getting row and col number)
			if (counter == 2) {
				size = size + 1;	// increment the size
				counter = 0;			// reset counter back to 0
			}
		}
		// else increment the size cause this is the space or .,/? and so on
		else {size = size + 1;}
	}
	
	// after getting the size, initialized character array  
	//printf("size of ciphertext %d\n", size);
	char* char_array = (char*)malloc(size * sizeof(char));
	if (char_array == NULL) {
		printf("Memory allocation failed");
		exit(1);
	}
	
	// loop through the ciphertext to compare its row and char; then grab its letter from there
	int index = 0;
	
	for (int i = 0; ciphertext[i] != '\0'; i ++) {
			char row = ciphertext[i];
			i = i+1;
			if (row == ' ') {
				//printf("pretty sure this is space\n");
				char_array[index] = ' ';
            	index = index + 1;
				i=i+1;
				row = ciphertext[i-1];
			}
			char col = ciphertext[i];
			//printf("checking row and col: %c, %c\n", row, col);
			const struct pb_node* result = isRowColInStructArray(table, row - '0', col - '0');
			if (result != NULL) {
            	char_array[index] = result->letter;
            	index = index+1;
            }
            else {
            	char_array[index] = ' ';
            	index = index + 1;
            }
	}
	char_array[index]='\0';
	printf("decoding %s to %s\n", ciphertext, char_array);
	return char_array;
}

/***
int main() {
	int numOfLetter = 25;
	struct pb_node* table = populate_pb_table(numOfLetter);
	const char *text = "This is a secret message\n";
	char *pbEncodeResult = pbEncode(text, table);
	free(pbEncodeResult);
	
	const char *ciphertext = "433254 1434243322 2334321552344225";
	const char *ciphertext2 = "44232443 2443 11 431513421544 32154343112215";
	char *pbDecodeResult = pbDecode(ciphertext, table);
	char *pbDecodeResult2 = pbDecode(ciphertext2, table);
	free(pbDecodeResult);
	free(pbDecodeResult2);
	
	free(table);
	return 0;
}*/
