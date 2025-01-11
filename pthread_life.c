


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 4  // Number of threads to use

// Function declarations
void *update_cells(void *threadid);
void swap_grids();
int MeshPlot(int t, int m, int n, char **mesh);
double real_rand();
int seed_rand(long sd);
double getTime();

// Global variables
static char **currWorld = NULL, **nextWorld = NULL;
static int nx = 100, ny = 100, maxiter = 200;
static int population[2] = {0, 0};  // For tracking population

// Mutex for synchronizing population update
pthread_mutex_t population_mutex;

// Structure to pass to each thread
typedef struct {
    int thread_id;
    int num_threads;
} ThreadData;

int main(int argc, char **argv) {
    int i, j, t;
    long seedVal = 0;
    float prob = 0.5;  // Probability of initial live cell
    int disable_display = 0;
    int s_step = 0;

    // Random seed setup
    seed_rand(seedVal);
    
    // Allocate memory for the 2D grids
    nx += 2;  // Account for ghost cells
    ny = nx;  // Square grid
    currWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    nextWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    for (i = 0; i < nx; i++) {
        currWorld[i] = (char *)(currWorld + nx) + i * ny;
        nextWorld[i] = (char *)(nextWorld + nx) + i * ny;
    }

    // Initialize boundaries
    for (i = 0; i < nx; i++) {
        currWorld[i][0] = 0;
        currWorld[i][ny - 1] = 0;
        nextWorld[i][0] = 0;
        nextWorld[i][ny - 1] = 0;
    }
    for (i = 0; i < ny; i++) {
        currWorld[0][i] = 0;
        currWorld[nx - 1][i] = 0;
        nextWorld[0][i] = 0;
        nextWorld[nx - 1][i] = 0;
    }

    // Randomly initialize the grid
    for (i = 1; i < nx - 1; i++) {
        for (j = 1; j < ny - 1; j++) {
            currWorld[i][j] = (real_rand() < prob);
            population[1] += currWorld[i][j];
        }
    }

    if (!disable_display) {
        MeshPlot(0, nx, ny, currWorld);
    }

    // Create threads for updating cells
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    
    // Initialize mutex
    pthread_mutex_init(&population_mutex, NULL);

    double t0 = getTime();
    for (t = 0; t < maxiter && population[1]; t++) {
        population[0] = 0;

        // Create threads for each section of the grid
        for (i = 0; i < NUM_THREADS; i++) {
            thread_data[i].thread_id = i;
            thread_data[i].num_threads = NUM_THREADS;
            pthread_create(&threads[i], NULL, update_cells, (void *)&thread_data[i]);
        }

        // Wait for all threads to finish
        for (i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        // Swap grids
        swap_grids();

        // Display grid (optional)
        if (!disable_display) {
            MeshPlot(t, nx, ny, currWorld);
        }

        // Single step mode for debugging
        if (s_step) {
            printf("Finished with step %d\n", t);
            printf("Press enter to continue.\n");
            getchar();
        }
    }

    double t1 = getTime();
    printf("Running time: %f sec.\n", t1 - t0);

    // Clean up
    free(nextWorld);
    free(currWorld);
    pthread_mutex_destroy(&population_mutex);

    return 0;
}

// Thread function to update the cells
void *update_cells(void *threadarg) {
    int i, j;
    ThreadData *data = (ThreadData *)threadarg;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;

    int chunk_size = (nx - 2) / num_threads;  // Divide grid among threads
    int start_row = 1 + thread_id * chunk_size;
    int end_row = (thread_id == num_threads - 1) ? (nx - 1) : (start_row + chunk_size);

    for (i = start_row; i < end_row; i++) {
        for (j = 1; j < ny - 1; j++) {
            int nn = currWorld[i + 1][j] + currWorld[i - 1][j] +
                     currWorld[i][j + 1] + currWorld[i][j - 1] +
                     currWorld[i + 1][j + 1] + currWorld[i - 1][j - 1] +
                     currWorld[i - 1][j + 1] + currWorld[i + 1][j - 1];

            nextWorld[i][j] = currWorld[i][j] ? (nn == 2 || nn == 3) : (nn == 3);
            
            pthread_mutex_lock(&population_mutex);
            population[0] += nextWorld[i][j];
            pthread_mutex_unlock(&population_mutex);
        }
    }

    pthread_exit(NULL);
}

// Swap the current world and next world grids
void swap_grids() {
    char **temp = currWorld;
    currWorld = nextWorld;
    nextWorld = temp;
}

// Function to get the current time (for timing)
/*double getTime() {*/
/*    struct timespec ts;*/
/*    clock_gettime(CLOCK_MONOTONIC, &ts);*/
/*    return ts.tv_sec + ts.tv_nsec / 1e9;*/
/*}*/
