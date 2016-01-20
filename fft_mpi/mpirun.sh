#!/bin/bash
./num_gen -e $1 --triangle | mpirun -np $2 ./fft_it_mpi $3
