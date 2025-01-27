#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define MATCH(s) (!strcmp(argv[ac], (s)))

int MeshPlot(int t, int m, int n, char **mesh);

double real_rand();
int seed_rand(long sd);

static char **currWorld=NULL, **nextWorld=NULL, **tmesh=NULL;
static int maxiter = 200; /* number of iteration timesteps */
static int population[2] = {0,0}; /* number of live cells */

int nx = 100;      /* number of mesh points in the x dimension */
int ny = 100;      /* number of mesh points in the y dimension */

static int w_update = 0;
static int w_plot = 1;

double getTime();
extern FILE *gnu;

// CSV writing function
void write_csv(const char *filename, int mesh_points, int iterations, float probability, int threads, long seed, int disable_display, int game_type, double execution_time) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        printf("Error opening CSV file.\n");
        return;
    }
    
    // Write header if file is empty
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "mesh_points,iterations,probability,threads,seed,disable_display,game_type,execution_time\n");
    }

    // Write data
    fprintf(fp, "%d,%d,%f,%d,%ld,%d,%d,%f\n", mesh_points, iterations, probability, threads, seed, disable_display, game_type, execution_time);
    fclose(fp);
}

int main(int argc, char **argv) {
    struct timespec start, end;
    
    // Set up output CSV file
    const char *csv_filename = "serial_execution_results.csv";

    // Different parameter combinations (you can adjust these)
    int mesh_sizes[] = {100, 200, 300, 500, 1000, 2000, 3000, 5000, 10000};
    int iterations[] = {100, 200, 300, 1000, 2000, 3000, 5000, 10000};
    float probabilities[] = {0.2, 0.3, 0.4, 0.5,0.6,0.7,0.8,0.9 };
    int seeds[] = {42, 123, 37};
    int games[] = {0, 1};

    for (int ms = 0; ms < 9; ms++) {
        for (int it = 0; it < 999999999; it++) {
            for (int p = 0; p < 9; p++) {
                for (int s = 0; s < 3; s++) {
                    for (int g = 0; g < 2; g++) {
                        // Reset parameters
                        nx = mesh_sizes[ms];
                        maxiter = iterations[it];
                        float prob = probabilities[p];
                        long seedVal = seeds[s];
                        int game = games[g];
                        int disable_display = 1;  // Disable display for performance measurement
                        int numthreads = 1; // For serial execution

                        // Start timing
                        clock_gettime(CLOCK_MONOTONIC, &start);

                        // Your main simulation logic here (initialization, world creation, running game of life iterations, etc.)

                        // Example of world initialization (from your given code):
                        currWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
                        nextWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
                        for (int i = 0; i < nx; i++) {
                            currWorld[i] = (char *)(currWorld + nx) + i * ny;
                            nextWorld[i] = (char *)(nextWorld + nx) + i * ny;
                        }

                        // Your game simulation iterations (this is a simplified example from the original code):
                        for (int t = 0; t < maxiter && population[w_plot]; t++) {
                            population[w_update] = 0;
                            for (int i = 1; i < nx - 1; i++) {
                                for (int j = 1; j < ny - 1; j++) {
                                    // Game of life update logic (this is from your original code)
                                    int nn = currWorld[i + 1][j] + currWorld[i - 1][j] +
                                             currWorld[i][j + 1] + currWorld[i][j - 1] +
                                             currWorld[i + 1][j + 1] + currWorld[i - 1][j - 1] +
                                             currWorld[i - 1][j + 1] + currWorld[i + 1][j - 1];

                                    nextWorld[i][j] = currWorld[i][j] ? (nn == 2 || nn == 3) : (nn == 3);
                                    population[w_update] += nextWorld[i][j];
                                }
                            }

                            // Pointer swap
                            tmesh = nextWorld;
                            nextWorld = currWorld;
                            currWorld = tmesh;
                        }

                        // Stop timing
                        clock_gettime(CLOCK_MONOTONIC, &end);
                        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

                        // Write the result to the CSV
                        write_csv(csv_filename, nx, maxiter, prob, numthreads, seedVal, disable_display, game, elapsed_time);

                        // Clean up
                        free(currWorld);
                        free(nextWorld);
                    }
                }
            }
        }
    }

    return 0;
}

