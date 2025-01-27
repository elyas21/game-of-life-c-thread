#!/bin/bash

# Define program arguments (modify as needed)
n=100  # Number of mesh points in one dimension
i=100  # Number of generations (or iterations)
s=1234  # Random seed (optional)
p=0.2  # Distribution probability (optional)
d=0  # Enable display (optional, 1 to disable)
step=0  # Disable step-by-step mode (optional, 1 to enable)

# Run serial version
echo "Running serial version (life.c)..."
./life -n $n -i $i -s $s -p $p -d $d -step $step

# Run pthreads version
echo "Running pthreads version (pthread_life.c)..."
./pthread_life -n $n -i $i -s $s -p $p -d $d -step $step -t 2  # Adjust number of threads (t)

# Run OpenMP version
echo "Running OpenMP version (openmp_life.c)..."
./openmp_life -n $n -i $i -s $s -p $p -d $d -step $step -t 2  # Adjust number of threads (t)

echo "All versions completed!"
