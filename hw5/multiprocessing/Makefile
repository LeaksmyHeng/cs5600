all: cipher multiprocess

cipher: polybius.c cipher.c
	gcc polybius.c cipher.c -o cipher

multiprocess: multiprocess.c genRandHelper-Heng.c queue.c
	gcc multiprocess.c genRandHelper-Heng.c queue.c -o multiprocess

clean:
	rm -f cipher multiprocess

