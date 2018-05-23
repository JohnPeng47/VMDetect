CC=gcc
CFLAGS=-I .
DEPS=utils.h
MACROS=DEBUG

%.o: %.c $(DEPS)
	$(CC) -g -m32 -c -o $@ $< $(CFLAGS)
inject: inject.o utils.o
	gcc -g -o inject inject.o utils.o -m32 -I .
test: test.c
	gcc -m32 -o test test.c func2.c -g
clean:
	rm inject.o inject utils.o
