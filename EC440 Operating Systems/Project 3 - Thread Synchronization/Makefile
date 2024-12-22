CC=gcc -Wall -Werror -g
all: main
# 	$(CC) -o main main.o threads.o

# main: main.c
# 	$(CC) -c -o main.o main.c

thread.o: threads.c
	$(CC) -c -o threads.o threads.c

main: threads.o main.c
	$(CC) -o main main.c threads.o

clean:
	rm -f threads.o main