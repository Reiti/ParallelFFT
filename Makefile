#fft makefile
CC = gcc
DEFS = -D_XOPEN_SOURCE=500 -D_BSD_SOURCE
CFLAGS = -Wall -O3 -g -std=c99 -pedantic $(DEFS)
LDFLAGS = -lrt -pthread -lm
OBJECTFILES = dft.o fft_it.o fft_rec.o fft_rec_cilk.o prints.o num_gen.o numgenparser.o
EXECS = dft fft_it fft_rec num_gen fft_rec_cilk
.PHONY: all clean

all: dft fft_it fft_rec num_gen

num_gen: num_gen.o
	$(CC)  -o $@ $^ $(LDFLAGS)
dft: dft.o prints.o numgenparser.o
	$(CC)  -o $@ $^ $(LDFLAGS)
fft_it: fft_it.o prints.o numgenparser.o
	$(CC)  -o $@ $^ $(LDFLAGS)
fft_rec: fft_rec.o prints.o numgenparser.o
	$(CC)  -o $@ $^ $(LDFLAGS)

fft_rec_cilk: fft_rec_cilk.o prints.o numgenparser.o
	$(CC)  -o $@ $^ $(LDFLAGS)

fft_rec_cilk.o: fft_rec_cilk.c
	$(CC) -fcilkplus $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJECTFILES) $(EXECS)
