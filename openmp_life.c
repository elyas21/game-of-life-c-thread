#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include <unistd.h> // For getopt

#define MATCH(s) (!strcmp(optarg, (s)))

int MeshPlot(int t, int m, int n, char **mesh);
double real_rand();
int seed_rand(long sd);
double getTime();
extern FILE *gnu;

static char **currWorld = NULL, **nextWorld = NULL, **tmesh = NULL;
static int maxiter = 200;
static int population[2] = {0, 0};
static int nx = 100;
static int ny = 100;

void saveResult(const char *codetype, int nx, int ny, int maxiter, float prob, long seedVal, int numthreads, double elapsed_time) {
    FILE *fp;
    fp = fopen("results.csv", "a");
    if (fp == NULL) {
        perror("Error opening file!");
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
    double start_time, end_time;
    start_time = omp_get_wtime();

    int i, j, t;
    float prob = 0.5;
    long seedVal = time(NULL); // Default seed
    int game = 0;
    int s_step = 0;
    int numthreads = 1;
    int disable_display = 0;
    int opt;

    // Use getopt for argument parsing
    while ((opt = getopt(argc, argv, "n:i:t:p:s:dg:h")) != -1) {
        switch (opt) {
            case 'n': nx = atoi(optarg); break;
            case 'i': maxiter = atoi(optarg); break;
            case 't': numthreads = atoi(optarg); break;
            case 'p': prob = atof(optarg); break;
            case 's': seedVal = atol(optarg); break;
            case 'd': disable_display = 1; break;
            case 'g': game = atoi(optarg); break;
            case 'h':
                printf("Usage: %s -n <meshpoints> -i <iterations> -t <numthreads> -p <prob> -s <seed> [-d] [-g <game #>] [-h]\n", argv[0]);
                return 0;
            default:
                fprintf(stderr, "Usage: %s -n <meshpoints> -i <iterations> -t <numthreads> -p <prob> -s <seed> [-d] [-g <game #>] [-h]\n", argv[0]);
                return 1;
        }
    }

    int rs = seed_rand(seedVal);
    nx = nx + 2;
    ny = nx;

    currWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    if (currWorld == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    for (i = 0; i < nx; i++) currWorld[i] = (char *)(currWorld + nx) + i * ny;

    nextWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
        if (nextWorld == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    for (i = 0; i < nx; i++) nextWorld[i] = (char *)(nextWorld + nx) + i * ny;

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

    for (t = 0; t < maxiter && population[1]; t++) {
        population[0] = 0;
#pragma omp parallel shared(currWorld, nextWorld, population)
        {
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

    if (gnu != NULL)
        pclose(gnu);

    free(nextWorld);
    free(currWorld);

    end_time = omp_get_wtime();
    printf("Execution time: %f seconds\n", end_time - start_time);
    saveResult("openmp", nx, ny, maxiter, prob, seedVal, numthreads, end_time - start_time);

    return 0;
}