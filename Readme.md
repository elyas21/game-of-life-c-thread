# Conway's Game of Life: Parallel Performance Analysis

## Project Overview

This project analyzes the performance of different implementations of Conway's Game of Life, a popular cellular automaton, focusing on serial execution, pthread-based parallelization, and OpenMP-based parallelization. The experiment evaluates the execution time for various grid sizes and the number of threads used, providing insights into the scalability and efficiency of each approach.

### Implementations:
- **Serial (`life`)**: A single-threaded implementation.
- **Pthreads (`pthread_life`)**: A parallelized implementation using POSIX threads.
- **OpenMP (`openmp_life`)**: A parallelized implementation using OpenMP.

## Project Structure

├── README.md # Project overview and usage instructions ├── report.pdf # Performance analysis report with results ├── src/ │ ├── life.c # Serial implementation │ ├── pthread_life.c # pthread-based implementation │ ├── openmp_life.c # OpenMP-based implementation │ └── Makefile # Build instructions ├── data/ │ ├── grid_size.png # Plot for execution time vs grid size │ ├── threads.png # Plot for execution time vs number of threads └── results/ └── results.csv # CSV file containing execution time results

python
Copy
Edit

## Requirements

- **C Compiler (GCC recommended)**
- **POSIX threads (pthreads) library**
- **OpenMP support**

## Compilation

To compile the different implementations, use the provided `Makefile`:

```bash
# Compile all versions
make all

# Compile only the serial version
make life

# Compile only the pthread version
make pthread_life

# Compile only the OpenMP version
make openmp_life

# Clean up compiled binaries
make clean
Usage
Once compiled, you can run each version with the following commands:

bash
Copy
Edit
# Run the serial implementation
./life <grid_size> <num_iterations>

# Run the pthread implementation (with specified number of threads)
./pthread_life <grid_size> <num_iterations> <num_threads>

# Run the OpenMP implementation (with specified number of threads)
./openmp_life <grid_size> <num_iterations> <num_threads>
Example usage for a 1000x1000 grid over 200 iterations with 4 threads:

bash
Copy
Edit
./pthread_life 1000 200 4
./openmp_life 1000 200 4
Results
The performance of each implementation was evaluated for different grid sizes and thread counts. Results are stored in results/results.csv and visualized in data/grid_size.png and data/threads.png. A detailed analysis is provided in the report.pdf.

Key Observations:
Serial Version: The serial implementation (life) scales poorly as the grid size increases.
Pthreads Version: The pthread-based implementation (pthread_life) shows improvement with more threads but exhibits diminishing returns after 4 threads due to overhead.
OpenMP Version: The OpenMP-based implementation (openmp_life) outperforms the pthread version for most configurations, demonstrating better scaling as thread count increases.
Report
For a detailed performance analysis, including the graphs and experiment results, refer to the report.pdf.

Contributing
Contributions are welcome! If you'd like to improve the performance or add more parallelization strategies, feel free to fork the repository and create a pull request.

License
This project is licensed under the MIT License. See the LICENSE file for more details.

markdown
Copy
Edit

### Key Sections of the `README.md`:
1. **Project Overview**: Brief description of the project and the types of implementations.
2. **Project Structure**: File structure of the project.
3. **Requirements**: Lists the dependencies and libraries required.
4. **Compilation**: Instructions on how to compile the project using the provided `Makefile`.
5. **Usage**: Commands to run the serial, pthread, and OpenMP implementations.
6. **Results**: Location of the results and key findings.
7. **Report**: Link to the detailed performance report.
8. **Contributing**: Invitation for contributions.
9. **License**: License information.

This structure should be clear and informative for anyone who wants to un