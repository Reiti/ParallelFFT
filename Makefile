#fft makefile
CC = gcc-4.9
DEFS = -D_XOPEN_SOURCE=500 -D_BSD_SOURCE
CFLAGS = -Wall -O3 -g -std=c99 -pedantic $(DEFS)
LDFLAGS = -lrt -pthread -lm
OBJECTFILES = dft.o fft_it.o fft_rec.o fft_rec_cilk.o fft_rec_omp.o fft_it_omp.o prints.o num_gen.o numgenparser.o fft_rec_mpi.o fft_it_cilk.o
EXECS = dft fft_it fft_rec num_gen fft_rec_cilk fft_rec_omp fft_rec_mpi fft_it_omp fft_rec omp fft_it_cilk
.PHONY: all clean

all: dft fft_it fft_rec num_gen fft_rec_cilk fft_rec_omp fft_it_omp fft_it_cilk fft_rec_mpi

num_gen: num_gen.o
	$(CC)  -o $@ $^ $(LDFLAGS)
dft: dft.o prints.o numgenparser.o
	$(CC)  -o $@ $^ $(LDFLAGS)
fft_it: fft_it.o prints.o numgenparser.o
	$(CC)  -o $@ $^ $(LDFLAGS)
fft_rec: fft_rec.o prints.o numgenparser.o
	$(CC)  -o $@ $^ $(LDFLAGS)
fft_rec_omp: fft_rec_omp.o prints.o numgenparser.o
	$(CC)  -fopenmp -o $@ $^ $(LDFLAGS)
fft_rec_cilk: fft_rec_cilk.o prints.o numgenparser.o
	$(CC)  -fcilkplus -o $@ $^ $(LDFLAGS)
fft_rec_mpi: fft_rec_mpi.o prints.o numgenparser.o
	mpicc -o $@ $^ $(LDFLAGS)
fft_it_omp: fft_it_omp.o prints.o numgenparser.o
	$(CC)  -fopenmp -o $@ $^ $(LDFLAGS)
fft_it_cilk: fft_it_cilk.o prints.o numgenparser.o
	$(CC)  -fcilkplus -o $@ $^ $(LDFLAGS)

fft_it_omp.o: fft_it_omp.c
	$(CC) -fopenmp $(CFLAGS) -c -o $@ $<
fft_rec_omp.o: fft_rec_omp.c
	$(CC) -fopenmp $(CFLAGS) -c -o $@ $<
fft_rec_cilk.o: fft_rec_cilk.c
	$(CC) -fcilkplus $(CFLAGS) -c -o $@ $<
fft_it_cilk.o: fft_it_cilk.c
	$(CC) -fcilkplus $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJECTFILES) $(EXECS)
