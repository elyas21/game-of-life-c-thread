import subprocess
import csv
import matplotlib.pyplot as plt
import numpy as np

# Function to execute a command and return the result
def run_command(command):
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    return result.stdout

# Define your parameters
params = [
    {"command": "./life -n 500 -i 200 -p 0.2", "label": "Serial"},
    {"command": "./pthread_life -n 500 -i 200 -p 0.2 -t 4", "label": "Pthreads"},
    {"command": "./openmp_life -n 500 -i 200 -p 0.2 -t 4", "label": "OpenMP"}
]

# Initialize results list
results = []

# Run the commands and capture the output
for param in params:
    output = run_command(param["command"])
    # Assuming the output contains time information, parse it
    # You can adjust this based on how your program outputs the results
    lines = output.splitlines()
    time_taken = None
    for line in lines:
        if "Time" in line:  # If output contains time info
            time_taken = float(line.split(":")[1].strip())
            break
    results.append({"label": param["label"], "time": time_taken})

# Write the results to a CSV file
with open("game_of_life_results.csv", mode='w', newline='') as file:
    writer = csv.DictWriter(file, fieldnames=["label", "time"])
    writer.writeheader()
    writer.writerows(results)

# Plot the results
labels = [result["label"] for result in results]
times = [result["time"] for result in results]

# Simple bar chart
plt.figure(figsize=(10, 6))
plt.bar(labels, times, color=['blue', 'green', 'red'])
plt.title("Game of Life Execution Time")
plt.xlabel("Execution Type")
plt.ylabel("Time (seconds)")
plt.savefig("execution_time_bar_chart.png")
plt.show()

# Line plot to show performance trend
plt.figure(figsize=(10, 6))
x = np.arange(len(labels))
plt.plot(x, times, marker='o', linestyle='-', color='b')
plt.xticks(x, labels)
plt.title("Game of Life Execution Time (Line Plot)")
plt.xlabel("Execution Type")
plt.ylabel("Time (seconds)")
plt.savefig("execution_time_line_chart.png")
plt.show()

# Scatter plot for visualizing performance
plt.figure(figsize=(10, 6))
plt.scatter(labels, times, color='purple')
plt.title("Game of Life Execution Time (Scatter Plot)")
plt.xlabel("Execution Type")
plt.ylabel("Time (seconds)")
plt.savefig("execution_time_scatter_plot.png")
plt.show()

