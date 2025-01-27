#!/bin/bash

# Define program arguments (modify as needed)
n=100  # Number of mesh points in one dimension
i=100  # Number of generations (or iterations)
s=1234  # Random seed (optional)
p=0.2  # Distribution probability (optional)
d=1     # Disable display for accurate timing

# CSV output file
output_file="task_three_results.csv"

# Create CSV header if the file doesn't exist
if [ ! -f "$output_file" ]; then
  echo "Program,Threads,Time (s),Output Match" > "$output_file"
fi

# Function to run a specific version, time it, output board, and compare
run_version() {
  local program="$1"
  local threads="$2"

  echo "Running $program..."

  # Run the program and capture output to a temporary file
  start_time=$(date +%s.%N)
  if [[ $step -eq 1 ]]; then
      ./"$program" -n $n -i $i -s $s -p $p -d $d -step > "$program.output"
  else
      ./"$program" -n $n -i $i -s $s -p $p -d $d > "$program.output"
  fi
  end_time=$(date +%s.%N)
  elapsed_time=$(echo "scale=9; $end_time - $start_time" | bc)

  # Compare output with the serial version (assuming "life" is the serial executable)
  diff_output=$(diff life.output "$program.output")
  if [[ -z "$diff_output" ]]; then
    output_match="Yes"
  else
    output_match="No"
  fi

  echo "$program,$threads,$elapsed_time,$output_match" >> "$output_file"
  echo "  Time: $elapsed_time seconds, Output Match: $output_match"
  if [[ $threads ]]; then
    echo "  (using $threads threads)"
  fi
  rm "$program.output" # Clean up the temporary output file
}

# Read "-step" option from command line arguments
step=0
while getopts ":s:" opt; do
  case $opt in
    s) step=1 ;;  # Set step flag if "-s" is present
    \?) echo "Invalid option: -$OPTARG" >&2; exit 1 ;;
  esac
done

# Run serial version and capture output
run_version life 1
mv life.output life.output.baseline #Rename the output of serial version for comparison

# Run pthreads versions with different thread counts
for threads in 1 2 4 8; do
  run_version pthread_life $threads
done

# Run OpenMP versions with different thread counts
for threads in 1 2 4 8; do
  export OMP_NUM_THREADS=$threads
  run_version openmp_life $threads
  unset OMP_NUM_THREADS
done
rm life.output.baseline
echo "Results written to $output_file"
