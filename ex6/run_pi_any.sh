#!/bin/bash
# Runs pi_any for each process count, 10 times each, saves to pi_any_times.txt
mpicc pi_any.c -o pi_any || exit 1
> pi_any_times.txt
for np in 1 2 4 8; do
    for run in $(seq 1 10); do
        mpirun --oversubscribe -np $np ./pi_any | tee -a pi_any_times.txt
    done
done
echo "Saved to pi_any_times.txt"
