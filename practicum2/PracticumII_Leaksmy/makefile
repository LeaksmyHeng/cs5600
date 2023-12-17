all: server client test

server: server.c
	gcc server.c commonfunction.c -o server -pthread

client: client.c
	gcc client.c commonfunction.c -o rfs

test: unit_tests.c
	gcc unit_tests.c commonfunction.c -o test
