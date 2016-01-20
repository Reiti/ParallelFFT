#!/bin/bash
mpirun -np $1 ./fft_it_mpi -f $2 $3
