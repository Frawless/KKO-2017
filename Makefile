CC = g++ 
CFLAGS = -std=c++98 -pedantic -Wall -W -O2 -Wextra
all: main

main:
	$(CC) $(CFLAGS) ahed.c main.c -o ahed
debug:
	gcc -ggdb3 ahed.c main.c -o ahed

clean:
	rm -rf ahead

