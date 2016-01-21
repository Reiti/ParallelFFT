#!/bin/bash
FFT=$1
START_EXP=$2
END_EXP=$3
ITERATIONS=$4
OUTPUT=$5

echo $FFT >> benchmark_res
for ((exp = $START_EXP; exp <= END_EXP; exp++))
  do
    res="$(./num_gen -e $exp | $FFT -b $ITERATIONS)"
    sorted=($(printf '%s\n' "${res[@]}"|sort -n))
    echo "$exp;${sorted[$ITERATIONS/2]}" >> $OUTPUT
  done
  for ((i = 0; i < $ITERATIONS; i++))
  do
    echo "${sorted[$i]}"
  done
