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
#include <stdio.h>

void saveResult(const char *codetype, int nx, int ny, int maxiter, float prob, long seedVal, int numthreads, double elapsed_time) {
    FILE *fp;
    fp = fopen("results.csv", "a");  // Open in append mode
    
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    // If file is empty, write the header
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "codetype,nx,ny,maxiter,prob,seedVal,numthreads,elapsed_time\n");
    }

    // Write the results with codetype as the first column
    fprintf(fp, "%s,%d,%d,%d,%.2f,%ld,%d,%f\n", codetype, nx, ny, maxiter, prob, seedVal, numthreads, elapsed_time);
    
    fclose(fp);
}

void cleanup() {
    if (currWorld != NULL) free(currWorld);
    if (nextWorld != NULL) free(nextWorld);
    if (gnu != NULL) pclose(gnu);
}

int main(int argc,char **argv)
{
    struct timespec start, end;
    
    // Get start time
    clock_gettime(CLOCK_MONOTONIC, &start);
    int i,j,ac;

    /* Set default input parameters */
    
    float prob = 0.5;   /* Probability of placing a cell */
    long seedVal = 0;
    int game = 0;
    int s_step = 0;
    int numthreads = 1;
    int disable_display = 0;

    /* Over-ride with command-line input parameters (if any) */
    
    for(ac=1;ac<argc;ac++)
    {
        if(MATCH("-n")) {nx = atoi(argv[++ac]);}
        else if(MATCH("-i")) {maxiter = atoi(argv[++ac]);}
        else if(MATCH("-t"))  {numthreads = atof(argv[++ac]);}
        else if(MATCH("-p"))  {prob = atof(argv[++ac]);}
        else if(MATCH("-s"))  {seedVal = atof(argv[++ac]);}
        else if(MATCH("-step"))  {s_step = 1;}
        else if(MATCH("-d"))  {disable_display = 1;}
        else if(MATCH("-g"))  {game = atoi(argv[++ac]);}
        else {
            printf("Usage: %s [-n < meshpoints>] [-i <iterations>] [-s seed] [-p prob] [-t numthreads] [-step] [-g <game #>] [-d]\n",argv[0]);
            return(-1);
        }
    }

    int rs = seed_rand(seedVal);
    /* Increment sizes to account for boundary ghost cells */
    
    nx = nx + 2;
    ny = nx; 
    
    /* Allocate contiguous memory for two 2D arrays of size nx*ny. */
    currWorld = (char**)malloc(sizeof(char*) * nx + sizeof(char) * nx * ny);
    nextWorld = (char**)malloc(sizeof(char*) * nx + sizeof(char) * nx * ny);

    for (i = 0; i < nx; i++) {
        currWorld[i] = (char*)(currWorld + nx) + i * ny;
        nextWorld[i] = (char*)(nextWorld + nx) + i * ny;
    }
    
    /* Initialize the boundary ghost cells */
    for (i = 0; i < nx; i++) {
        currWorld[i][0] = 0;
        currWorld[i][ny-1] = 0;
        nextWorld[i][0] = 0;
        nextWorld[i][ny-1] = 0;
    }

    for (i = 0; i < ny; i++) {
        currWorld[0][i] = 0;
        currWorld[nx-1][i] = 0;
        nextWorld[0][i] = 0;
        nextWorld[nx-1][i] = 0;
    }

    // Generate a world
    
    if (game == 0) { // Use Random input
        for (i = 1; i < nx - 1; i++)
            for (j = 1; j < ny - 1; j++) {
                currWorld[i][j] = (real_rand() < prob);
                population[w_plot] += currWorld[i][j];
            }
    } else if (game == 1) { // Block, still life
        printf("2x2 Block, still life\n");
        int nx2 = nx / 2;
        int ny2 = ny / 2;
        currWorld[nx2+1][ny2+1] = currWorld[nx2][ny2+1] = currWorld[nx2+1][ny2] = currWorld[nx2][ny2] = 1;
        population[w_plot] = 4;
    } else {
        printf("Unknown game %d\n", game);
        cleanup();
        exit(-1);
    }
    
    printf("probability: %f\n", prob);
    printf("Random # generator seed: %d\n", rs);

    /* Plot the initial data */
    if (!disable_display) {
        MeshPlot(0, nx, ny, currWorld);
    }

    /* Perform updates for maxiter iterations */
    double t0 = getTime();
    int t;

    for (t = 0; t < maxiter && population[w_plot]; t++) {
        population[w_update] = 0;
        for (i = 1; i < nx - 1; i++)
            for (j = 1; j < ny - 1; j++) {
                int nn = currWorld[i+1][j] + currWorld[i-1][j] +
                         currWorld[i][j+1] + currWorld[i][j-1] +
                         currWorld[i+1][j+1] + currWorld[i-1][j-1] +
                         currWorld[i-1][j+1] + currWorld[i+1][j-1];
                
                nextWorld[i][j] = currWorld[i][j] ? (nn == 2 || nn == 3) : (nn == 3);
                population[w_update] += nextWorld[i][j];
            }
        
        /* Pointer Swap: nextWorld <-> currWorld */
        tmesh = nextWorld;
        nextWorld = currWorld;
        currWorld = tmesh;
        
        /* Start the new plot */
        if (!disable_display)
            MeshPlot(t, nx, ny, currWorld);
        
        if (s_step) {
            printf("Finished with step %d\n", t);
            printf("Press enter to continue.\n");
            getchar();
        }
    }

    double t1 = getTime(); 
    printf("Running time for the iterations: %f sec.\n", t1 - t0);
    
    // Clean up and free resources
    cleanup();

    // Get end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate time difference in seconds and nanoseconds
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Execution time: %f seconds\n", elapsed_time);
        saveResult("serial", nx, ny, maxiter, prob, seedVal, numthreads, elapsed_time);


    return 0;
}
