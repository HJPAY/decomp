# Makefile for decomp

# Program name and objects
PROGRAM = decomp
OBJS = main.o

CC = gcc
CFLAGS = -Wall -O0 -g3

.SUFFIXES: .c .o


$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)

