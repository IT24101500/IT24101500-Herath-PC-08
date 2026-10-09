#!/bin/bash
# Runs sum for each process count, 10 times each, saves to sum_times.txt
mpicc sum.c -o sum || exit 1
> sum_times.txt
for np in 1 2 4 8; do
    for run in $(seq 1 10); do
        mpirun --oversubscribe -np $np ./sum | tee -a sum_times.txt
    done
done
echo "Saved to sum_times.txt"
