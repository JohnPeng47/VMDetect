CC=gcc
CFLAGS=-I .
DEPS=utils.h
MACROS=DEBUG

%.o: %.c $(DEPS)
	$(CC) -g -m32 -c -o $@ $< $(CFLAGS) -D DEBUG
inject: inject.o utils.o
	gcc -g -o inject inject.o utils.o -m32 -I . -D DEBUG
test: test.c
	gcc -m32 -o test test.c -g
clean_test:
	rm test 
clean:
	rm inject.o inject utils.o
