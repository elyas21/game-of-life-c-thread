#!/bin/bash

# Define the output CSV file
output_csv="game_of_life_results.csv"
echo "Threads,Iterations,Time(life),Time(pthread_life),Time(openmp_life)" > $output_csv

# Function to run a command and measure the execution time
run_command() {
    local command=$1
    local iterations=$2
    local threads=$3
    local result

    # Run the command with the parameters and capture time
    start_time=$(date +%s.%N)
    $command -n 500 -i $iterations -t $threads -p 0.2
    end_time=$(date +%s.%N)

    # Calculate elapsed time
    elapsed_time=$(echo "$end_time - $start_time" | bc)
    echo $elapsed_time
}

# Loop through the number of threads and iterations
for threads in 1 2 4 8 16; do
    for iterations in 100 200 500; do
        # Run each program and capture the results
        life_time=$(run_command "./life" $iterations $threads)
        pthread_life_time=$(run_command "./pthread_life" $iterations $threads)
        openmp_life_time=$(run_command "./openmp_life" $iterations $threads)

        # Write results to CSV file
        echo "$threads,$iterations,$life_time,$pthread_life_time,$openmp_life_time" >> $output_csv
    done
done

# Plotting using gnuplot
gnuplot <<EOF
set terminal png size 800,600
set output "game_of_life_plot.png"
set title "Game of Life Performance"
set xlabel "Threads"
set ylabel "Time (seconds)"
set grid

# Plot data from the CSV file
plot for [i=3:5] '$output_csv' using 1:i with linespoints title columnheader
EOF

echo "Results have been written to $output_csv and the plot is saved as game_of_life_plot.png."

