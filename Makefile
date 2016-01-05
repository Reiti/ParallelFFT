#fft makefile
CC = gcc
DEFS = -D_XOPEN_SOURCE=500 -D_BSD_SOURCE
CFLAGS = -Wall -g -std=c99 -pedantic $(DEFS)
LDFLAGS = -lrt -pthread -lm
OBJECTFILES = dft.o fft_it.o fft_rec.c 
EXECS = dft fft_it fft_rec 
.PHONY: all clean 

all: dft fft_it fft_rec

dft: dft.o
	$(CC)  -o $@ $^ $(LDFLAGS)
fft_it: fft_it.o
	$(CC)  -o $@ $^ $(LDFLAGS)
fft_rec: fft_rec.o
	$(CC)  -o $@ $^ $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJECTFILES) $(EXECS)

