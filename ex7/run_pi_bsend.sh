#!/bin/bash
# Runs pi_bsend for each process count, 10 times each, saves to pi_bsend_times.txt
mpicc pi_bsend.c -o pi_bsend || exit 1
> pi_bsend_times.txt
for np in 1 2 4 8; do
    for run in $(seq 1 10); do
        mpirun --oversubscribe -np $np ./pi_bsend | tee -a pi_bsend_times.txt
    done
done
echo "Saved to pi_bsend_times.txt"
