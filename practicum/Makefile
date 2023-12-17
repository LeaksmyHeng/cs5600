all: memory_simulation

memory_simulation: memory_simulation.c memory_cache.c main.c
	gcc memory_simulation.c memory_cache.c main.c -o main

memory:
	valgrind ./main

clean:
	rm -f main

