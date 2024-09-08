CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter -O2

all: fpuz

%.o: %.c
	$(CC) $(CFLAGS) -c $^

fpuz: utils.o generator.o solver.o answer.o
	$(CC) $(CFLAGS) main.c -o $@ $^ 

clean:
	rm fpuz *.o 

