# import subprocess
# import pandas as pd
# import matplotlib.pyplot as plt

# def run_experiment(executable, params):
#     command = [executable] + params
#     subprocess.run(command, check=True)

# def test_grid_size():
#     grid_sizes = [100, 500, 1000, 2000]
#     for n in grid_sizes:
#         run_experiment('./life', ['-n', str(n), '-i', '200', '-d'])
#         run_experiment('./pthread_life', ['-n', str(n), '-i', '200', '-t', '4', '-d'])
#         run_experiment('./openmp_life', ['-n', str(n), '-i', '200', '-t', '4', '-d'])
#     df = pd.read_csv('results.csv')
#     for codetype in df['codetype'].unique():
#         subset = df[df['codetype'] == codetype]
#         plt.plot(subset['nx'], subset['elapsed_time'], label=codetype)
#     plt.xlabel('Grid Size')
#     plt.ylabel('Time (s)')
#     plt.legend()
#     plt.savefig('grid_size.png')

# def test_threads():
#     threads = [1, 2, 4, 8, 16]
#     for t in threads:
#         run_experiment('./pthread_life', ['-n', '1000', '-i', '200', '-t', str(t), '-d'])
#         run_experiment('./openmp_life', ['-n', '1000', '-i', '200', '-t', str(t), '-d'])
#     df = pd.read_csv('results.csv')
#     for codetype in ['pthread', 'openmp']:
#         subset = df[df['codetype'] == codetype]
#         plt.plot(subset['numthreads'], subset['elapsed_time'], label=codetype)
#     plt.xlabel('Threads')
#     plt.ylabel('Time (s)')
#     plt.legend()
#     plt.savefig('threads.png')

# if __name__ == "__main__":
#     open('results.csv', 'w').close()  # Clear previous results
#     test_grid_size()
#     test_threads()
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import time
import csv

# Function to run experiment and measure time
def run_experiment(executable, params):
    command = [executable] + params
    start_time = time.time()  # Start timer
    subprocess.run(command, check=True)  # Run the command
    end_time = time.time()  # End timer
    return end_time - start_time  # Return the elapsed time

# Function to write results to CSV
def write_results(codetype, nx, numthreads, elapsed_time):
    with open('results1.csv', mode='a', newline='') as file:
        writer = csv.writer(file)
        writer.writerow([codetype, nx, numthreads, elapsed_time])

# Test different grid sizes
def test_grid_size():
    grid_sizes = [100, 500, 1000, 2000]
    for n in grid_sizes:
        # Measure time for each implementation
        time_life = run_experiment('./life', ['-n', str(n), '-i', '200', '-d'])
        write_results('life', n, None, time_life)
        
        time_pthread = run_experiment('./pthread_life', ['-n', str(n), '-i', '200', '-t', '4', '-d'])
        write_results('pthread', n, 4, time_pthread)
        
        time_openmp = run_experiment('./openmp_life', ['-n', str(n), '-i', '200', '-t', '4', '-d'])
        write_results('openmp', n, 4, time_openmp)
    
    # Read results and plot
    df = pd.read_csv('results1.csv')
    for codetype in df['codetype'].unique():
        subset = df[df['codetype'] == codetype]
        plt.plot(subset['nx'], subset['elapsed_time'], label=codetype)
    plt.xlabel('Grid Size')
    plt.ylabel('Time (s)')
    plt.legend()
    plt.savefig('grid_size.png')

# Test different thread counts
def test_threads():
    threads = [1, 2, 4, 8, 16]
    for t in threads:
        time_pthread = run_experiment('./pthread_life', ['-n', '1000', '-i', '200', '-t', str(t), '-d'])
        write_results('pthread', 1000, t, time_pthread)
        
        time_openmp = run_experiment('./openmp_life', ['-n', '1000', '-i', '200', '-t', str(t), '-d'])
        write_results('openmp', 1000, t, time_openmp)
    
    # Read results and plot
    df = pd.read_csv('results1.csv')
    for codetype in ['pthread', 'openmp']:
        subset = df[df['codetype'] == codetype]
        plt.plot(subset['numthreads'], subset['elapsed_time'], label=codetype)
    plt.xlabel('Threads')
    plt.ylabel('Time (s)')
    plt.legend()
    plt.savefig('threads.png')

if __name__ == "__main__":
    # Clear previous results and write header
    with open('results1.csv', mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['codetype', 'nx', 'numthreads', 'elapsed_time'])
    
    # Run tests
    test_grid_size()
    test_threads()
