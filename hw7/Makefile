all: memory_simulation

memory_simulation: memory_simulation.c
	gcc memory_simulation.c -o memory_simulation

memory:
	valgrind ./memory_simulation

clean:
	rm -f memory_simulation

