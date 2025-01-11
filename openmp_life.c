#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <omp.h>

#define MATCH(s) (!strcmp(argv[ac], (s)))

int MeshPlot(int t, int m, int n, char **mesh);
double real_rand();
int seed_rand(long sd);
double getTime();
extern FILE *gnu;

static char **currWorld = NULL, **nextWorld = NULL, **tmesh = NULL;
static int maxiter = 200; /* number of iteration timesteps */
static int population[2] = {0, 0}; /* number of live cells */

int nx = 100; /* number of mesh points in the x dimension */
int ny = 100; /* number of mesh points in the y dimension */

int main(int argc, char **argv) {
  double start_time, end_time;
    
    start_time = omp_get_wtime();
    int i, j, ac, t;
    float prob = 0.5; /* Probability of placing a cell */
    long seedVal = 0;
    int game = 0;
    int s_step = 0;
    int numthreads = 1;
    int disable_display = 0;

    /* Over-ride with command-line input parameters (if any) */
    for (ac = 1; ac < argc; ac++) {
        if (MATCH("-n")) { nx = atoi(argv[++ac]); }
        else if (MATCH("-i")) { maxiter = atoi(argv[++ac]); }
        else if (MATCH("-t")) { numthreads = atoi(argv[++ac]); }
        else if (MATCH("-p")) { prob = atof(argv[++ac]); }
        else if (MATCH("-s")) { seedVal = atol(argv[++ac]); }
        else if (MATCH("-step")) { s_step = 1; }
        else if (MATCH("-d")) { disable_display = 1; }
        else if (MATCH("-g")) { game = atoi(argv[++ac]); }
        else {
            printf("Usage: %s [-n < meshpoints>] [-i <iterations>] [-s seed] [-p prob] [-t numthreads] [-step] [-g <game #>] [-d]\n", argv[0]);
            return (-1);
        }
    }

    int rs = seed_rand(seedVal);
    nx = nx + 2;
    ny = nx;

    /* Allocate memory for worlds */
    currWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    for (i = 0; i < nx; i++) currWorld[i] = (char *)(currWorld + nx) + i * ny;

    nextWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    for (i = 0; i < nx; i++) nextWorld[i] = (char *)(nextWorld + nx) + i * ny;

    /* Set boundary cells to '0' */
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

    // Random or predefined game state
    if (game == 0) {
        for (i = 1; i < nx - 1; i++)
            for (j = 1; j < ny - 1; j++) {
                currWorld[i][j] = (real_rand() < prob);
                population[1] += currWorld[i][j];
            }
    }

    if (!disable_display)
        MeshPlot(0, nx, ny, currWorld);

    omp_set_num_threads(numthreads);

    double t0 = getTime();

    /* Parallel loop for generations */
    for (t = 0; t < maxiter && population[1]; t++) {
        population[0] = 0;
        #pragma omp parallel shared(currWorld, nextWorld, population)
        {
            /* Parallel cell update */
            #pragma omp for collapse(2)
            for (i = 1; i < nx - 1; i++) {
                for (j = 1; j < ny - 1; j++) {
                    int nn = currWorld[i + 1][j] + currWorld[i - 1][j] +
                             currWorld[i][j + 1] + currWorld[i][j - 1] +
                             currWorld[i + 1][j + 1] + currWorld[i - 1][j - 1] +
                             currWorld[i - 1][j + 1] + currWorld[i + 1][j - 1];

                    nextWorld[i][j] = currWorld[i][j] ? (nn == 2 || nn == 3) : (nn == 3);
                    #pragma omp atomic
                    population[0] += nextWorld[i][j];
                }
            }
            /* Synchronize cell update and display */
            #pragma omp single
            {
                tmesh = nextWorld;
                nextWorld = currWorld;
                currWorld = tmesh;

                if (!disable_display)
                    MeshPlot(t, nx, ny, currWorld);

                if (s_step) {
                    printf("Finished with step %d\n", t);
                    printf("Press enter to continue.\n");
                    getchar();
                }
            }
        }
    }

    double t1 = getTime();
    printf("Running time: %f sec.\n", t1 - t0);
    printf("Press enter to end.\n");
    getchar();

    if (gnu != NULL)
        pclose(gnu);

    free(nextWorld);
    free(currWorld);
    end_time = omp_get_wtime();  // End time
    
    printf("Execution time: %f seconds\n", end_time - start_time);
    return 0;
}

