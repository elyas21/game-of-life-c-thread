import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Assuming the data is already loaded
#data = pd.read_csv('game_of_life_data.csv')

data = pd.read_csv("serial_execution_results.csv")

# 1. Execution Time vs. Number of Threads
plt.figure(figsize=(10, 6))
sns.lineplot(data=data, x='threads', y='execution_time', hue='parallel_type', marker='o')
plt.title("Execution Time vs. Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (seconds)")
plt.legend(title="Parallel Type")
plt.savefig('execution_time_vs_threads.png', bbox_inches='tight')
plt.close()

# 2. Execution Time vs. Mesh Points
plt.figure(figsize=(10, 6))
sns.scatterplot(data=data, x='mesh_points', y='execution_time', hue='threads')
plt.title("Execution Time vs. Mesh Points")
plt.xlabel("Mesh Points (N)")
plt.ylabel("Execution Time (seconds)")
plt.legend(title="Number of Threads")
plt.savefig('execution_time_vs_mesh_points.png', bbox_inches='tight')
plt.close()

# 3. Speedup Comparison (single thread vs multi-thread)
plt.figure(figsize=(10, 6))
data['speedup'] = data['time_single_thread'] / data['execution_time']
sns.barplot(data=data, x='threads', y='speedup', hue='parallel_type')
plt.title("Speedup vs. Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.legend(title="Parallel Type")
plt.savefig('speedup_vs_threads.png', bbox_inches='tight')
plt.close()

# 4. OpenMP vs Pthreads Execution Time Comparison
plt.figure(figsize=(10, 6))
sns.barplot(data=data, x='threads', y='execution_time', hue='parallel_type')
plt.title("OpenMP vs Pthreads Execution Time Comparison")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (seconds)")
plt.legend(title="Parallel Type")
plt.savefig('openmp_vs_pthreads.png', bbox_inches='tight')
plt.close()

# 5. Heatmap for Game Evolution (for a single generation)
plt.figure(figsize=(8, 8))
sns.heatmap(data.pivot('row', 'column', 'state'), cmap='binary', cbar=False)
plt.title("Game of Life - Single Generation")
plt.savefig('game_of_life_single_gen.png', bbox_inches='tight')
plt.close()

