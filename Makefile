CC = g++ 
CFLAGS = -std=c++98 -pedantic -Wall -W -O2 -Wextra
FILES = ahed.c ahed.h Makefile main.c ahed.pdf
NAME = kko.proj1.xstejs24
all: main

main:
	$(CC) $(CFLAGS) ahed.c main.c -o ahed
debug:
	gcc -ggdb3 ahed.c main.c -o ahed

clean:
	rm -rf ahead

pack:
	rm -f $(NAME).zip
	zip -r $(NAME).zip $(FILES)
