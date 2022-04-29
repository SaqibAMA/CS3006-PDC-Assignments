#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

int main(int argc, char *argv[])
{

    // argument format: <numProcs: p> <problemSize: n> <filename: f>

    // check number of arguments
    if (argc != 4)
    {
        printf("Error: incorrect number of arguments\n");
        return 1;
    }

    // get number of procs
    int numProcs = atoi(argv[1]);

    // get size of problem
    int problemSize = atoi(argv[2]);

    // get filename
    char* filename = argv[3];

    // check if num procs valid
    if (numProcs < 1)
    {
        printf("Error: number of procs must be greater than 0\n");
        return 1;
    }

    // check if problem size valid
    if (problemSize < 1)
    {
        printf("Error: problem size must be greater than 0\n");
        return 1;
    }

    // flags array
    int *hasFactor = (int *)malloc(problemSize * sizeof(int));
    memset(hasFactor, 0, problemSize * sizeof(int));

    // ---- SEQUENTIAL ----

    // double seq_start = omp_get_wtime();

    // // sequential loop -- just for benchmarking
    // for (int i = 3; i < problemSize; i += 2)
    //     for (int j = 3; j <= (int) sqrt(i); j++)
    //         if (i % j == 0) hasFactor[i] = 1;

    // printf("Sequential time: %lf\n", omp_get_wtime() - seq_start);
    // memset(hasFactor, 0, problemSize * sizeof(int));

    // ---- SEQUENTIAL END ----
    

    // ---- PARALLEL ----

    double par_start = omp_get_wtime();

    // parallel loop
    #pragma omp parallel for num_threads(numProcs)
    for (int i = 3; i < problemSize; i += 2) {
        int upper_bound = (int) sqrt(i);
        for (int j = 3; j <= upper_bound; j += 2)
            if (i % j == 0) hasFactor[i] = 1;
    }

    printf("Parallel time: %lf\n", omp_get_wtime() - par_start);

    return 0;

    int console_out = (strcmp(filename, "stdout") == 0);

    for (int i = 3; i < problemSize; i += 2)
    {
        if (!hasFactor[i])
        {
            if (console_out) {
                printf("%d\n", i);
            }
            else {
                FILE* fp = (FILE*) fopen(filename, "a");
                fprintf(fp, "%d\n", i);
                fclose(fp);
            }
        }
    }

    free(hasFactor);

    return 0;
}