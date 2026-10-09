#!/bin/bash
# Runs pi for each process count, 3 times each, saves to pi_times.txt
mpicc pi.c -o pi || exit 1
> pi_times.txt
for np in 1 2 4 8; do
    for run in 1 2 3; do
        mpirun --oversubscribe -np $np ./pi | tee -a pi_times.txt
    done
done
echo "Saved to pi_times.txt"
