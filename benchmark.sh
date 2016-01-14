#!/bin/bash
FFT=$1
START_EXP=$2
END_EXP=$3
ITERATIONS=$4

echo $FFT >> benchmark_res
for ((exp = $START_EXP; exp <= END_EXP; exp++))
  do
    echo e$exp >> benchmark_res
    for ((i = 0; i < $ITERATIONS; i++))
    do
      ./num_gen -e $exp --rectangle | $FFT
      #need to somehow get the timing value here
    done
    echo ${res[$ITERATIONS/2]} >> benchmark_res
  done
