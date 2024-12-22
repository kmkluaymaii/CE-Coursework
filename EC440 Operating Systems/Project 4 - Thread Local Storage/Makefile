CC=gcc -Wall -Werror -g
all: tls.o

tls.o: tls.c
	$(CC) -c -lpthread -o tls.o tls.c

clean:
	rm -f tls.o 