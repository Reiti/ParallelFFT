#!/bin/bash
FFT=$1
START_EXP=$2
END_EXP=$3
ITERATIONS=$4
OUTPUT=$5

echo $FFT >> benchmark_res
for ((exp = $START_EXP; exp <= END_EXP; exp++))
  do
    for ((i = 0; i < $ITERATIONS; i++))
    do
      res[i]="$(./num_gen -e $exp --rectangle | $FFT -b)"
    done
    sorted=($(printf '%s\n' "${res[@]}"|sort))
    echo "$exp;${sorted[$ITERATIONS/2]}" >> $OUTPUT
  done
