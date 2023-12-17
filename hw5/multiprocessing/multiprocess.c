#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "genRandHelper-Heng.h"
#include "queue.h"

const char charset[] = "abcdefghijklmnopqrstuvwxyz";
const int max_word_length = 50;
const int desired_word_count = 10000;
const char* file_name = "random_words.txt";
const char* output_file_name = "random_words.txt";
const int max_word_in_sentence = 100;

char* generate_word(int max_length) {
	/**
	Part 1
	Function to generate_word. Assuming the word is in between 1 to n.
	@param max_length: the maximum length of the word
	@type max_length: int
	*/
	int min_length = 1;
	int length = genRand(1, max_length);	// generate random number and write to file
	char* word = (char*)malloc(length+1);	// 1 for null terminator
	
	// check if we allocate the memory successfully
	if (word == NULL) {
		printf("Unable to allocate memory for generate_word correctly\n");
		exit(1);
	}
	
	for (int i=0; i<length; i++) {
		int index = genRand(1, 27);		// there are 27 character from a-z
		word[i] = charset[index-1];		// the index in charset however is an array so start at 0 instead of 1
	}
	
	word[length] = '\0';				// end of character
	return word;
}

void generate_random_word_file(const char* file_name, int desired_word_count, int max_word_length) {
	/**
	Part 1:
	Generate the file by inputing the file_name, the desired_word_count and the max_word_length
	
	@param file_name: the name of the file you want to write word to
	@type file_name: char *
	@param desired_word_count: the total number for word you want to write to the file. No less than 10,000
	@type desired_word_count: int
	@param max_length: the maximum length of the word
	@type max_length: int
	*/
	
	// do the check in case the desired word is less than 10,000
	if (desired_word_count < 10000) {
		printf("Desired word can't be less than 10000");
		exit(1);
	}
	
	
	FILE* file = fopen(file_name, "w");
	// check if file is NULL
	if (file == NULL) {
		printf("Error opening file;");
		exit(1);
	}
	
	int word_count = 0;
	printf("Start writing to file\n");
	while (word_count < desired_word_count) {
		char* random_word = generate_word(max_word_length);
		fprintf(file, "%s ", random_word);	// write word to file
		free(random_word);					// free the memory
		word_count++;						// increment the word
	}
	fclose(file);
	
	printf("Finish writing to file\n");
}

int file_exits(const char* file_name) {
	/**
	Check if file_name exists.
	*/
	return access(file_name, F_OK) != -1;
}

void read_file_and_add2queue(const char* file_name, queue_t* queue) {
	/**
	Part2:
	Open file, read word then write to queue.
	@param file_name: the name of the file you want to write word to
	@type file_name: char *
	@param queue: queue you want to add each word to
	@type queue_t: queue_t
	*/
	
	if (!file_exits(file_name)) {
		printf("File does not exist yet so calling generate_random_word_file now\n");
		// all the variable are define as a constant on top of the file so please use that
		generate_random_word_file(file_name, desired_word_count, max_word_length);
	}
	
	printf("Opening file in read_file_and_add2queue\n");
	FILE* file = fopen(file_name, "r");
	if (file == NULL) {
		printf("File open error\n");
		exit(1);
	}
	
	char word[max_word_length+1];		// buffer size of aword
    char format_specifier[20]; // A reasonable buffer size for the format specifier
    snprintf(format_specifier, sizeof(format_specifier), "%%%d%s", max_word_length, "s");

	while (fscanf(file, format_specifier, word) == 1) {
        // Print the word to validate that it is printing correctly before adding to queue
        //printf("%s\n", word);
        char* word_copy = (char*)malloc(strlen(word) + 1);
        if (word_copy == NULL) {
        	printf("Memory allocation failed\n");
        	fclose(file);
        	return;
        }
        strcpy(word_copy, word);		
        add2q(queue, word_copy);		// add word to queue
    }
    fclose(file);
    printf("calling read_file_and_add2queue successfully\n");
}


void process_word_batch(char* words[], int word_count, int batch_number, const char* output_prefix) {
	/**
	Process a batch of word (100 at a time). Assuming each word is stored in the queue cause we just loaded the word
	to the queue from the previous function. That would make 100 words at a time means 100 nodes at a time.
	@param queue: queue that already has word loaded in
	@type queue: queue_t
	@param process_id: the id of the process.
	@type process_id: int
	*/
	printf("Start calling process_word_batch\n");
	
	// create a pipe for communication with the cipher program
	// https://man7.org/linux/man-pages/man2/pipe.2.html#:~:text=pipe()%20creates%20a%20pipe,write%20end%20of%20the%20pipe.
	// https://www.gnu.org/software/libc/manual/html_node/Creating-a-Pipe.html#:~:text=The%20pipe%20function%20creates%20a,descriptor%201%20is%20standard%20output.
	
	// Create a unique file name for the output
    char output_file_name[50];
    snprintf(output_file_name, sizeof(output_file_name), "%s_batch%d.txt", output_prefix, batch_number);
	
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		printf("error while creating pipe\n");
		exit(1);
	}
	
	// Fork a child process
	pid_t pid = fork();
	if (pid == -1) {
		printf("error while creating a child process\n");
		exit(1);
	}
	
	if (pid == 0) {
		printf("Start reading from child process.\n");
		close(pipefd[0]);
		
		// Redirect stdout to the write end of the pipe
		// https://man7.org/linux/man-pages/man2/dup.2.html
    	dup2(pipefd[1], STDOUT_FILENO);
    	
    	// execute the cipher_program. The executable file's name is cipher
    	char* cipher_program = "./cipher";
    	
    	// Execute the cipher
    	// create the arary for command line
    	char* arguments[word_count+3];
    	arguments[0] = cipher_program;
    	arguments[1] = "-e";
    	for (int i = 0; i<word_count; i++) {
    		arguments[i+2] = words[i];
    	}
    	arguments[word_count + 2] = NULL;
        execvp(cipher_program, arguments);
        // If exec fails, print an error message
        printf("Error during exec\n");
        exit(1);
	}
	// parent process
	else {
		printf("Start writing to file in parent process.\n");
		close(pipefd[1]);
		wait(NULL);			// wait for child process
		
		FILE* output_file = fopen(output_file_name, "w");
        if (output_file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        	fwrite(buffer, 1, bytes_read, output_file);
        }
        // close file
        fclose(output_file);
        // Close the read end of the pipe
        close(pipefd[0]);
        
        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
	}	
}

int process(queue_t* queue, const char* output_prefix) {
	char* sentence;
    int batch_number = 1; // Batch identifier
    int word_count = 0;
    char* words[100]; // Assuming each sentence has a maximum of 100 words

    while ((sentence = popQ(queue)) != NULL) {
        // Split the sentence into words (space-separated)
        char* token = strtok(sentence, " ");
        
        

        while (token != NULL && word_count < 100) {
            words[word_count] = token;
            token = strtok(NULL, " ");
            word_count++;
        }
	
	if (word_count == 100) {
        	// Process the batch of words
        	process_word_batch(words, word_count, batch_number, output_prefix);
        	batch_number++; // Increment the batch identifier
        	// Reset word count and clear words array for the next batch
            word_count = 0;
            memset(words, 0, sizeof(words));
        }
    }
    // Process any remaining words in the last batch
    if (word_count > 0) {
        process_word_batch(words, word_count, batch_number, output_prefix);
        batch_number++;
    }

    // Clean up the queue.
    return 0;
}


void main(int argc, char* argv[]) {
	queue_t queue;
	initialize_queue(&queue);
	if (argc == 1) {
		read_file_and_add2queue(file_name, &queue);		// file constant is a global variable define on top of the file
		const char* output_prefix = "output";
		process(&queue, output_prefix);
	}
	else if (argc == 2) {
		const char* input_file = argv[1];
		const char* output_prefix = "output";
		read_file_and_add2queue(input_file, &queue);
		process(&queue, output_prefix);
	} 
	
	else if (argc == 3) {
		const char* input_file = argv[1];
		const char* output_prefix = argv[2];
		read_file_and_add2queue(input_file, &queue);
		process(&queue, output_prefix);
	} 
	else {
		printf("At most 2 argument. Not %d argument\n", argc);
	}
}

