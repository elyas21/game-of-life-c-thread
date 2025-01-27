import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# Load data using pandas for easier data manipulation
try:
    df = pd.read_csv('task_three_results.csv')
except FileNotFoundError:
    print("Error: task_three_results.csv not found. Please run the data generation script first.")
    exit()

# Convert relevant columns to numeric, handling errors
df['n'] = pd.to_numeric(df['n'], errors='coerce')
df['i'] = pd.to_numeric(df['i'], errors='coerce')
df['s'] = pd.to_numeric(df['s'], errors='coerce')
df['p'] = pd.to_numeric(df['p'], errors='coerce')
df['Threads'] = pd.to_numeric(df['Threads'], errors='coerce')
df['Time (s)'] = pd.to_numeric(df['Time (s)'], errors='coerce')

# Remove rows with errors or empty values in Time (s)
df = df.dropna(subset=['Time (s)'])

# Create a figure with subplots
fig, axes = plt.subplots(3, 2, figsize=(15, 15))
fig.suptitle("Game of Life Performance Analysis", fontsize=16)

# 1. Time vs. Threads (Line Plot with Seaborn)
sns.lineplot(x='Threads', y='Time (s)', hue='Program', style='n', markers=True, data=df[df['Program'] != 'life'], ax=axes[0, 0])
axes[0, 0].set_title("Time vs. Threads (Parallel Versions)")
axes[0, 0].set_xticks(df[df['Program'] != 'life']['Threads'].unique()) # Set x-axis ticks explicitly
axes[0, 0].set_xlabel("Number of Threads")
axes[0, 0].set_ylabel("Execution Time (s)")

# 2. Time vs. n (Bar Plot)
sns.barplot(x='n', y='Time (s)', hue='Program', data=df, ax=axes[0, 1])
axes[0, 1].set_title("Time vs. Mesh Size (n)")
axes[0, 1].set_xlabel("Mesh Size (n)")
axes[0, 1].set_ylabel("Execution Time (s)")

# 3. Time vs. i (Bar Plot)
sns.barplot(x='i', y='Time (s)', hue='Program', data=df, ax=axes[1, 0])
axes[1, 0].set_title("Time vs. Iterations (i)")
axes[1, 0].set_xlabel("Number of Iterations (i)")
axes[1, 0].set_ylabel("Execution Time (s)")

# 4. Time vs. p (Box Plot)
sns.boxplot(x='p', y='Time (s)', hue='Program', data=df, ax=axes[1, 1])
axes[1, 1].set_title("Time vs. Probability (p)")
axes[1, 1].set_xlabel("Probability (p)")
axes[1, 1].set_ylabel("Execution Time (s)")

# 5. Time vs. Threads (Separate plots for each n, using FacetGrid)
g = sns.FacetGrid(df[df['Program'] != 'life'], col='n', hue='Program', height=4, aspect=1)
g.map(sns.lineplot, 'Threads', 'Time (s)', marker='o')
g.set_axis_labels("Number of Threads", "Execution Time (s)")
g.set_titles("Time vs. Threads (n = {col_name})")
g.add_legend()
axes[2,0].remove()
axes[2,1].remove()

plt.tight_layout(rect=[0, 0.03, 1, 0.95]) # Adjust layout to prevent overlap
plt.show()
