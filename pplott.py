import matplotlib.pyplot as plt
import pandas as pd

# Load the CSV file into a pandas dataframe
df = pd.read_csv('serial_execution_results.csv')

# Plot 1: Execution Time vs. Mesh Points (for a specific number of iterations and probability)
def plot_execution_time_vs_mesh_points():
    mesh_sizes = df['mesh_points'].unique()
    plt.figure(figsize=(10, 6))
    
    for size in mesh_sizes:
        data = df[df['mesh_points'] == size]
        plt.plot(data['iterations'], data['execution_time'], label=f'Mesh {size}')
    
    plt.title('Execution Time vs. Mesh Points')
    plt.xlabel('Number of Iterations')
    plt.ylabel('Execution Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.savefig('execution_time_vs_mesh_points.png')
    plt.show()

# Plot 2: Execution Time vs. Probability (for a specific number of mesh points and iterations)
def plot_execution_time_vs_probability():
    probabilities = df['probability'].unique()
    plt.figure(figsize=(10, 6))

    for prob in probabilities:
        data = df[df['probability'] == prob]
        plt.plot(data['iterations'], data['execution_time'], label=f'Prob {prob}')
    
    plt.title('Execution Time vs. Probability')
    plt.xlabel('Number of Iterations')
    plt.ylabel('Execution Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.savefig('execution_time_vs_probability.png')
    plt.show()

# Plot 3: Execution Time vs. Number of Threads (for a specific mesh size and probability)
def plot_execution_time_vs_threads():
    threads = df['threads'].unique()
    plt.figure(figsize=(10, 6))
    
    for t in threads:
        data = df[df['threads'] == t]
        plt.plot(data['iterations'], data['execution_time'], label=f'Threads {t}')
    
    plt.title('Execution Time vs. Threads')
    plt.xlabel('Number of Iterations')
    plt.ylabel('Execution Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.savefig('execution_time_vs_threads.png')
    plt.show()

# Plot 4: Execution Time vs. Iterations (for a specific mesh size and probability)
def plot_execution_time_vs_iterations():
    iterations = df['iterations'].unique()
    plt.figure(figsize=(10, 6))

    for it in iterations:
        data = df[df['iterations'] == it]
        plt.plot(data['mesh_points'], data['execution_time'], label=f'Iterations {it}')
    
    plt.title('Execution Time vs. Iterations')
    plt.xlabel('Mesh Points')
    plt.ylabel('Execution Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.savefig('execution_time_vs_iterations.png')
    plt.show()

# Call the plot functions
plot_execution_time_vs_mesh_points()
plot_execution_time_vs_probability()
plot_execution_time_vs_threads()
plot_execution_time_vs_iterations()

