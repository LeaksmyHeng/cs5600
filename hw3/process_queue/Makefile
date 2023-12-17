CC=gcc

all: qtest 

qtest: queuetest.c queue.c
	$(CC) queuetest.c queue.c -o qtest

clean:
	rm qtest

