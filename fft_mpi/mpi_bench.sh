#!/bin/bash
FFT=./fft_it_mpi
START_EXP=$1
END_EXP=$2
ITERATIONS=$3
OUTPUT=$4
PROCS="2 4 8 16 32 64"
# 4 8 16 32 64 128 256 512"
echo $FFT >> benchmark_res
for ((exp = $START_EXP; exp <= END_EXP; exp++))
  do
	for p in $PROCS
		do
		echo "FFT mit 2^$exp Zahlen mit $p kernen"
    		for ((i = 0; i < $ITERATIONS; i++))	
   			do
				#echo "./mpi.sh $p $FFT -b -f $exp"
  		  		res[i]="$(./mpi.sh $p $FFT -b -f $exp)"
				#echo $((res[i]))
    			done
    sorted=($(printf '%s\n' "${res[@]}"|sort))
    echo "$exp;$p;${sorted[$ITERATIONS/2]}" >> $OUTPUT
	done
  done
