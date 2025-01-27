#!/bin/bash

# Define program arguments (modify as needed)
n=100  # Number of mesh points in one dimension
i=100  # Number of generations (or iterations)
s=1234  # Random seed (optional)
p=0.2  # Distribution probability (optional)
d=1 # Disable display for accurate timing

# CSV output file
output_file="results.csv"

# Create CSV header if the file doesn't exist
if [ ! -f "$output_file" ]; then
  echo "Program,Threads,Time (s)" > "$output_file"
fi

# Function to run a specific version, time it, and append to CSV
run_version() {
  local program="$1"
  local threads="$2"

  echo "Running $program..."

  start_time=$(date +%s.%N)
  if [[ $step -eq 1 ]]; then
    ./"$program" -n $n -i $i -s $s -p $p -d $d -step
  else
    ./"$program" -n $n -i $i -s $s -p $p -d $d
  fi
  end_time=$(date +%s.%N)
  elapsed_time=$(echo "scale=9; $end_time - $start_time" | bc)

  echo "$program,$threads,$elapsed_time" >> "$output_file"
  echo "  Time: $elapsed_time seconds"
    if [[ $threads ]]; then
    echo "  (using $threads threads)"
  fi
}

# Read "-step" option from command line arguments
step=0
while getopts ":s:" opt; do
  case $opt in
    s) step=1 ;;  # Set step flag if "-s" is present
    \?) echo "Invalid option: -$OPTARG" >&2; exit 1 ;;
  esac
done

# Run serial version
run_version life 1

# Run pthreads versions with different thread counts
for threads in 1 2 4 8; do # Test with 1, 2, 4, and 8 threads
  run_version pthread_life $threads
done

# Run OpenMP versions with different thread counts
for threads in 1 2 4 8; do # Test with 1, 2, 4, and 8 threads
  export OMP_NUM_THREADS=$threads # Set OpenMP thread count
  run_version openmp_life $threads
  unset OMP_NUM_THREADS
done

echo "Results written to $output_file"
