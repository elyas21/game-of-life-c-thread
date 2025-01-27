import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load CSV data
data = pd.read_csv('task_three_results.csv')

# Filter for relevant programs: life, pthread_life, and openmp_life
life_data = data[data['Program'] == 'life']
pthread_data = data[data['Program'] == 'pthread_life']
openmp_data = data[data['Program'] == 'openmp_life']

# General settings for plots
plt.style.use('seaborn-darkgrid')

# Function to plot Time vs n for different Programs (life, pthread, openmp)
def plot_time_vs_n(df, program_name, ax, color):
    sns.lineplot(x='n', y='Time (s)', hue='Threads', marker='o', data=df, ax=ax, palette=color)
    ax.set_title(f'Time vs n for {program_name}')
    ax.set_xlabel('n')
    ax.set_ylabel('Time (s)')

# Plot 1: Time (s) vs n for life program
fig, ax = plt.subplots(figsize=(8, 5))
plot_time_vs_n(life_data, 'Life', ax, 'Set1')
plt.show()  # Ensure the first plot displays

# Plot 2: Time (s) vs n for pthread_life program
fig, ax = plt.subplots(figsize=(8, 5))
plot_time_vs_n(pthread_data, 'Pthread_Life', ax, 'Set2')
plt.show()  # Ensure the second plot displays

# Plot 3: Time (s) vs n for openmp_life program (if available)
if not openmp_data.empty:
    fig, ax = plt.subplots(figsize=(8, 5))
    plot_time_vs_n(openmp_data, 'OpenMP_Life', ax, 'Set3')
    plt.show()

# Function to plot Time vs Threads for pthread_life and openmp_life
def plot_time_vs_threads(df, ax, title, color):
    sns.lineplot(x='Threads', y='Time (s)', hue='n', marker='o', data=df, ax=ax, palette=color)
    ax.set_title(title)
    ax.set_xlabel('Threads')
    ax.set_ylabel('Time (s)')

# Plot 4: Time (s) vs Threads for pthread_life program
fig, ax = plt.subplots(figsize=(8, 5))
plot_time_vs_threads(pthread_data, ax, 'Pthread_Life: Time vs Threads', 'Set2')
plt.show()

# Plot 5: Speedup vs Threads (for pthread life only)
pthread_data['Speedup'] = pthread_data.groupby(['n', 'i', 's'])['Time (s)'].transform('min') / pthread_data['Time (s)']
fig, ax = plt.subplots(figsize=(8, 5))
sns.lineplot(x='Threads', y='Speedup', hue='n', marker='o', data=pthread_data, ax=ax, palette='Set2')
ax.set_title('Pthread_Life: Speedup vs Threads')
ax.set_xlabel('Threads')
ax.set_ylabel('Speedup')
plt.show()

# Plot 6: Efficiency vs Threads (for pthread life only)
pthread_data['Efficiency'] = pthread_data['Speedup'] / pthread_data['Threads']
fig, ax = plt.subplots(figsize=(8, 5))
sns.lineplot(x='Threads', y='Efficiency', hue='n', marker='o', data=pthread_data, ax=ax, palette='Set2')
ax.set_title('Pthread_Life: Efficiency vs Threads')
ax.set_xlabel('Threads')
ax.set_ylabel('Efficiency')
plt.show()

# Plot 7: Compare life, pthread, and openmp (if available) by plotting Time vs n in separate plots
fig, axes = plt.subplots(3, 1, figsize=(10, 15))
plot_time_vs_n(life_data, 'Life', axes[0], 'Set1')
plot_time_vs_n(pthread_data, 'Pthread_Life', axes[1], 'Set2')
if not openmp_data.empty:
    plot_time_vs_n(openmp_data, 'OpenMP_Life', axes[2], 'Set3')
plt.tight_layout()
plt.show()

# Additional Plot: Time vs Parameters 'n' and 'i' for each program
fig, ax = plt.subplots(figsize=(10, 6))
sns.scatterplot(x='n', y='Time (s)', hue='Program', size='i', data=data, ax=ax)
ax.set_title('Time vs n and i for All Programs')
ax.set_xlabel('n')
ax.set_ylabel('Time (s)')
plt.show()

