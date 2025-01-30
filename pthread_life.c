#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> // For getopt

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
static int num_threads = 4; // Make num_threads global and modifiable

// Mutex for synchronizing population update
pthread_mutex_t population_mutex;

// Structure to pass to each thread
typedef struct {
    int thread_id;
    int num_threads;
} ThreadData;

void saveResult(const char *codetype, int nx, int ny, int maxiter, float prob, long seedVal, int numthreads, double elapsed_time) {
    FILE *fp;
    fp = fopen("results.csv", "a");
    if (fp == NULL) {
        perror("Error opening file!"); // Use perror for better error messages
        return;
    }
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "codetype,nx,ny,maxiter,prob,seedVal,numthreads,elapsed_time\n");
    }
    fprintf(fp, "%s,%d,%d,%d,%.2f,%ld,%d,%f\n", codetype, nx, ny, maxiter, prob, seedVal, numthreads, elapsed_time);
    fclose(fp);
}

int main(int argc, char **argv) {
    int i, j, t;
    long seedVal = time(NULL); // Default seed based on time
    float prob = 0.5;
    int disable_display = 0;
    int s_step = 0;
    int opt;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "n:i:p:s:t:dh")) != -1) {
        switch (opt) {
            case 'n': nx = atoi(optarg); ny = nx; break;
            case 'i': maxiter = atoi(optarg); break;
            case 'p': prob = atof(optarg); break;
            case 's': seedVal = atol(optarg); break;
            case 't': num_threads = atoi(optarg); break;
            case 'd': disable_display = 1; break;
            case 'h':
                printf("Usage: %s -n <grid_size> -i <iterations> -p <probability> -s <seed> -t <num_threads> [-d] [-h]\n", argv[0]);
                return 0;
            default: /* '?' */
                fprintf(stderr, "Usage: %s -n <grid_size> -i <iterations> -p <probability> -s <seed> -t <num_threads> [-d] [-h]\n", argv[0]);
                return 1;
        }
    }

    seed_rand(seedVal);

    nx += 2;
    ny = nx;
    currWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    nextWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    if (!currWorld || !nextWorld) { // Check for malloc errors
        perror("Memory allocation failed");
        return 1;
    }
    for (i = 0; i < nx; i++) {
        currWorld[i] = (char *)(currWorld + nx) + i * ny;
        nextWorld[i] = (char *)(nextWorld + nx) + i * ny;
    }

    // Initialize boundaries (no changes needed here)

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

    pthread_t threads[num_threads]; // Use the global num_threads
    ThreadData thread_data[num_threads];

    pthread_mutex_init(&population_mutex, NULL);

    double t0 = getTime();
    for (t = 0; t < maxiter && population[1]; t++) {
        population[0] = 0;

        for (i = 0; i < num_threads; i++) {
            thread_data[i].thread_id = i;
            thread_data[i].num_threads = num_threads;
            pthread_create(&threads[i], NULL, update_cells, (void *)&thread_data[i]);
        }

        for (i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        swap_grids();

        if (!disable_display) {
            MeshPlot(t, nx, ny, currWorld);
        }

        if (s_step) {
            printf("Finished with step %d\n", t);
            printf("Press enter to continue.\n");
            getchar();
        }
    }

    double t1 = getTime();
    printf("Running time: %f sec.\n", t1 - t0);
    saveResult("pthread", nx, ny, maxiter, prob, seedVal, num_threads, t1 - t0);

    free(nextWorld);
    free(currWorld);
    pthread_mutex_destroy(&population_mutex);

    return 0;
}

// ... (update_cells and swap_grids functions remain the same)
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


