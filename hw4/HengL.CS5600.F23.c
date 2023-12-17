#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
 
 // homework part3 create a global array of integer
 // give arbitrary number of 100 size array
 unsigned long int global_int_array[120];
 
void *worker(void *data)
{
    char *name = (char*)data;
 
    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        
        // part 3 of HW: if this is thread X, write to array
        if (name == "X") {
        	global_int_array[i] = pthread_self();
        	printf("Thread %s write %lu to index %d\n", name, pthread_self(), i);
        }
        else {
        	printf("Thread %s found %ld at index %d\n", name, global_int_array[i], i);
        }
    }
 
    printf("Thread %s done!\n", name);
    return NULL;
}
 
int main(void)
{
 	pthread_t th1;
    pthread_t th2;
    pthread_create(&th1, NULL, worker, "X");
    pthread_create(&th2, NULL, worker, "Y");
    //sleep(5);
    //printf("exiting from main program\n");
    //pthread_join(th1, NULL);
    //pthread_join(th2, NULL);
    sleep(1);
    //printf("====> Cancelling Thread Y!!\n");
    
    // homework part 2 modify the print statement to print the thread id
    printf("Cancelling Thread Y id %lu!!\n", th2);
    pthread_cancel(th2);
    usleep(100000);
    printf("Cancelling Thread X id %lu!!\n", th1);
    pthread_cancel(th1);
    printf("exiting from main program\n");
    return 0;
}

/**
Part 4:
The threads' behaviour is inconsistent. This is expected because when multiple threads are used to read from or write to the same variable simultaneously, it creates a race condition and some other synchronization issues. This leads to unconsistent/unpredictable result. 

To avoid this issue, we can use lock or atomic operations. Locks enable use to manage access to a shared resource by preventing other threads from accessing it while it's being changed by the other. Conversely, atomic operations enable us to execute specific actions on a shared variable in a manner that ensures they are indivisible and cannot be interrupted by other threads.
*/
