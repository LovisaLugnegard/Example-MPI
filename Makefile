CC=mpicc
CCFLAGS = -03
CCGFLAGS=-g
CFLAGS=-Wall -O3
LIBS=-lmpi -lm

BINS = vectExample

vectExample: vectExample.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

