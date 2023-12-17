#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

int checkIfDigit(char number[]) {
	/**
	Check if the char number is actually an int
	*/
	int i = 0;
	for (; number[i] != 0; i++)
    {
        if (!isdigit(number[i]))
            return 1;
    }
    
    int result = atoi(number);
    //printf("%s convert to int %d\n", number, result);
    return result;
}

long getMicroSecond() {
	/**
	Get microseconds from the current time. This is use as the seed in LCG.
	*/
	// using time(NULL) to get second does not work with genRand cause the program run faster than that so we use microsecond instead
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	long microseconds = currentTime.tv_usec;
	return microseconds;
}


int genRand(int min, int max) {
	/**
	Generate random number using LCG.
	source: http://www.columbia.edu/~ks20/4106-18-Fall/Simulation-LCG.pdf
	*/
	unsigned long long int multiplier=1;			// put abitrary num
	unsigned long long int sequenceRandomNumbers=getMicroSecond();
	unsigned long long int increment=2;				//put abitrary num
	unsigned long long int modulus = 4294967296;	//assuming we are using 32bits
	
	int result;
	result = (multiplier*sequenceRandomNumbers + increment)%modulus;
	return min + (int)(sequenceRandomNumbers % (max - min + 1));
}

