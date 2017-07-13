# Makefile for loancalc
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -lm   links the Math library
#
CC = gcc
CFLAGS = -g -Wall
LIBS = -lm

all: loancalc

clean:
	rm -f loancalc obj/*.o

loancalc: obj/loancalc.o
	$(CC) $(CFLAGS) -o loancalc obj/*.o $(LIBS)

obj/loancalc.o:
	$(CC) $(CFLAGS) -c src/loancalc.c -o obj/loancalc.o

