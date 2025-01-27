#!/bin/bash

# Parameters for the Game of Life programs
N=500           # Size of the grid (mesh points in one dimension)
I=200           # Number of generations (iterations)
P=0.2           # Distribution probability
S=$(date +%s)   # Seed based on the current time (or use a fixed number for repeatability)
T_MAX=8          # Max number of threads (you can adjust based on your system)
OUTPUT_FILE="results.csv"

# Create or overwrite the output CSV file and add headers
echo "Threads,Time_life,Time_pthread_life,Time_openmp_life" > $OUTPUT_FILE

# Loop through different numbers of threads
for T in $(seq 1 $T_MAX); do
    echo "Running with $T threads..."
    
    # Run the serial program (Life) and capture execution time
    START=$(date +%s%N)
    ./life -n $N -i $I -s $S -p $P -t 1   > life_output.log 2>&1
    END=$(date +%s%N)
    TIME_LIFE=$((($END - $START) / 1000000))  # Time in milliseconds
    echo "Life execution time (Serial): $TIME_LIFE ms"

    # Run the pthread-based program (pthread_life) and capture execution time
    START=$(date +%s%N)
    ./pthread_life -n $N -i $I -s $S -p $P -t $T   > pthread_life_output.log 2>&1
    END=$(date +%s%N)
    TIME_PTHREAD_LIFE=$((($END - $START) / 1000000))  # Time in milliseconds
    echo "Pthread Life execution time: $TIME_PTHREAD_LIFE ms"

    # Run the OpenMP-based program (openmp_life) and capture execution time
    START=$(date +%s%N)
    ./openmp_life -n $N -i $I -s $S -p $P -t $T > openmp_life_output.log 2>&1
    END=$(date +%s%N)
    TIME_OPENMP_LIFE=$((($END - $START) / 1000000))  # Time in milliseconds
    echo "OpenMP Life execution time: $TIME_OPENMP_LIFE ms"

    # Write results to CSV
    echo "$T,$TIME_LIFE,$TIME_PTHREAD_LIFE,$TIME_OPENMP_LIFE" >> $OUTPUT_FILE

    # Optional: Add a delay to ensure system resources are cleared
    sleep 1
done

echo "Results saved to $OUTPUT_FILE"

