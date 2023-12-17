all: threading

threading: threading.c genRandHelper-Heng.c queue.c polybius.c
	gcc threading.c genRandHelper-Heng.c queue.c polybius.c -o threading -pthread

clean:
	rm -f threading
