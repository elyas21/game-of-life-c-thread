import subprocess
import csv
import time
import logging
import os

# Configure logging
logging.basicConfig(filename='game_of_life.log', level=logging.DEBUG,
                    format='%(asctime)s - %(levelname)s - %(message)s')

def run_game(program, n, i, s, p, t, d, step, g):
    """Runs the specified Game of Life program."""

    command = [f"./{program}", "-n", str(n), "-i", str(i), "-s", str(s), "-p", str(p)]
    if t is not None:
        command.extend(["-t", str(t)])
    if d:  # Append -d to DISABLE display
        command.append("-d")
    if step:
        command.append("-step")
    if g is not None:
        command.extend(["-g", str(g)])

    logging.debug(f"Running command: {' '.join(command)}")

    start_time = time.perf_counter()
    output = None  # Initialize output to None
    try:
        process = subprocess.run(command, capture_output=True, text=True, check=False, timeout=10)
        if process.returncode == 0:  # Only if successful
            output = process.stdout
            logging.debug(f"Output from {program}:\n{output}")
        else:
            logging.error(f"{program} exited with non-zero code: {process.returncode}")
            logging.error(f"Stderr: {process.stderr}")
    except subprocess.TimeoutExpired:
        logging.error(f"{program} timed out after 10 seconds.")
    except FileNotFoundError as e:
        logging.error(f"File Not Found Error running {program}: {e}")
    except Exception as e:
        logging.error(f"An unexpected error occurred: {e}")

    end_time = time.perf_counter()
    elapsed_time = end_time - start_time
    logging.debug(f"Elapsed time for {program}: {elapsed_time}")
    return output, elapsed_time

def compare_outputs(output1, output2):
    """Compares two outputs."""
    if output1 is None or output2 is None:
        logging.warning("One or both outputs are None. Cannot compare.")
        return "Error"
    if output1 == "" or output2 == "":
        logging.warning("One or both outputs are empty.")
        return "Empty Output"
    match = output1 == output2
    logging.debug(f"Outputs match: {match}")
    return "Yes" if match else "No"

def main():
    """Main function."""

    programs = ["life", "pthread_life", "openmp_life"]
    n_values = [50]  # Reduced for testing
    i_values = [30]  # Reduced for testing
    s_values = [678]
    p_values = [ 0.4]
    t_values = [1]  # Test with multiple threads
    g_values = [None]
    d_value = False  # Disable display
    step_value = 0

    output_file = "task_three_results.csv"

    with open(output_file, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Program", "n", "i", "s", "p", "Threads", "Time (s)", "Output Match"])

        for program in programs:
            serial_output = None
            for n in n_values:
                for i in i_values:
                    for s in s_values:
                        for p in p_values:
                            for g in g_values:
                                if program == "life":
                                    threads = None
                                    output, elapsed_time = run_game(program, n, i, s, p, threads, d_value, step_value, g)
                                    serial_output = output
                                    if output is not None: #Only write to file if there was output
                                        writer.writerow([program, n, i, s, p, threads, elapsed_time, "N/A"])
                                else:
                                    for threads in t_values:
                                        output, elapsed_time = run_game(program, n, i, s, p, threads, d_value, step_value, g)
                                        output_match = compare_outputs(serial_output, output)
                                        if output is not None: #Only write to file if there was output
                                            writer.writerow([program, n, i, s, p, threads, elapsed_time, output_match])

    print(f"Results written to {output_file}")

if __name__ == "__main__":
    main()
