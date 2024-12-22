CC=gcc -Wall -Werror -g
all: fs

fs: fs.o 
	$(CC) fs.o disk.o -o fs

fs.o: fs.c
	$(CC) -c -o fs.o fs.c

clean:
	rm -f fs.o fs