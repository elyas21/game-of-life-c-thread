import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the CSV file
csv_filename = "serial_execution_results.csv"
data = pd.read_csv(csv_filename)

# Display first few rows to understand the structure
print(data.head())

# 1. Bar Plot for Execution Time by Mesh Points and Iterations
plt.figure(figsize=(10, 6))
sns.barplot(data=data, x='mesh_points', y='execution_time', hue='iterations')
plt.title("Execution Time by Mesh Points and Iterations")
plt.xlabel("Mesh Points")
plt.ylabel("Execution Time (seconds)")
plt.legend(title="Iterations")
plt.tight_layout()
plt.show()

# 2. Scatter Plot for Execution Time vs Probability
plt.figure(figsize=(10, 6))
sns.scatterplot(data=data, x='probability', y='execution_time', hue='mesh_points', style='iterations', markers=True, s=100)
plt.title("Execution Time vs Probability")
plt.xlabel("Probability")
plt.ylabel("Execution Time (seconds)")
plt.legend(title="Mesh Points and Iterations")
plt.tight_layout()
plt.show()

# 3. Line Plot for Execution Time Across Mesh Points, Grouped by Probability
plt.figure(figsize=(10, 6))
sns.lineplot(data=data, x='mesh_points', y='execution_time', hue='probability', style='iterations', markers=True)
plt.title("Execution Time Across Mesh Points, Grouped by Probability")
plt.xlabel("Mesh Points")
plt.ylabel("Execution Time (seconds)")
plt.legend(title="Probability and Iterations")
plt.tight_layout()
plt.show()

# 4. Box Plot for Execution Time Distribution by Mesh Points
plt.figure(figsize=(10, 6))
sns.boxplot(data=data, x='mesh_points', y='execution_time', hue='iterations')
plt.title("Execution Time Distribution by Mesh Points")
plt.xlabel("Mesh Points")
plt.ylabel("Execution Time (seconds)")
plt.legend(title="Iterations")
plt.tight_layout()
plt.show()

# 5. Heatmap for Execution Time by Mesh Points, Iterations, and Probability
plt.figure(figsize=(10, 6))

# Pivot the table to have mesh_points and iterations as axes and probability as color
pivot_data = data.pivot_table(values='execution_time', 
                              index=['mesh_points', 'iterations'], 
                              columns=['probability'], 
                              aggfunc='mean')

sns.heatmap(pivot_data, annot=True, fmt='.2f', cmap='coolwarm', cbar_kws={'label': 'Execution Time (seconds)'})
plt.title("Execution Time Heatmap by Mesh Points, Iterations, and Probability")
plt.xlabel("Probability")
plt.ylabel("Mesh Points and Iterations")
plt.tight_layout()
plt.show()

# 6. Pair Plot for All Numeric Variables
sns.pairplot(data)
plt.title("Pair Plot of Execution Time and Parameters")
plt.tight_layout()
plt.show()

# 7. Execution Time Distribution Histogram
plt.figure(figsize=(10, 6))
sns.histplot(data['execution_time'], kde=True, bins=30)
plt.title("Distribution of Execution Times")
plt.xlabel("Execution Time (seconds)")
plt.ylabel("Frequency")
plt.tight_layout()
plt.show()

