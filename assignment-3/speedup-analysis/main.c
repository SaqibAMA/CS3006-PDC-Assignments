#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 1000

void core_function_sequential()
{
    long win = 0;
    int key = 111;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for (int k = 0; k < SIZE; k++)
            {
                int x = (i * i * 1000 / 35) % 1000;
                int y = (j * j * 1000 / 36) % 1000;
                int z = (k * k * 1000 / 37) % 1000;
                if (key == (x + y + z))
                {
                    win = win + 1;
                }
            }
        }
    }

    printf("total wins=%ld\n", win);
}

void core_function_parallel(int assigned_threads)
{
    long win = 0;
    int key = 111;

    #pragma omp parallel for reduction(+:win) num_threads(assigned_threads)
    for (int i = 0; i < SIZE; i++)
    {
        #pragma omp parallel for reduction(+:win) num_threads(assigned_threads)
        for (int j = 0; j < SIZE; j++)
        {
            #pragma omp parallel for reduction(+:win) num_threads(assigned_threads)
            for (int k = 0; k < SIZE; k++)
            {
                int x = (i * i * 1000 / 35) % 1000;
                int y = (j * j * 1000 / 36) % 1000;
                int z = (k * k * 1000 / 37) % 1000;
                if (key == (x + y + z))
                {
                    win = win + 1;
                }
            }
        }
    }

    printf("total wins=%ld\n", win);
}



int main(int argc, char *argv[])
{

    double time_start;
    double sequential_time;


    // -- sequential
    printf("Starting sequential execution...\n");
    time_start = omp_get_wtime();
    core_function_sequential();
    printf("Total time taken: %lf\n", omp_get_wtime() - time_start);
    sequential_time = omp_get_wtime() - time_start;


    // -- parallel
    printf("Starting parallel execution [2 threads]...\n");
    time_start = omp_get_wtime();
    core_function_parallel(2);
    printf("Total time taken: %lf\nSpeedup: %lf\n", omp_get_wtime() - time_start, sequential_time / (omp_get_wtime() - time_start));

    printf("Starting parallel execution [3 threads]...\n");
    time_start = omp_get_wtime();
    core_function_parallel(3);
    printf("Total time taken: %lf\nSpeedup: %lf\n", omp_get_wtime() - time_start, sequential_time / (omp_get_wtime() - time_start));

    printf("Starting parallel execution [4 threads]...\n");
    time_start = omp_get_wtime();
    core_function_parallel(4);
    printf("Total time taken: %lf\nSpeedup: %lf\n", omp_get_wtime() - time_start, sequential_time / (omp_get_wtime() - time_start));

    printf("Starting parallel execution [8 threads]...\n");
    time_start = omp_get_wtime();
    core_function_parallel(8);
    printf("Total time taken: %lf\nSpeedup: %lf\n", omp_get_wtime() - time_start, sequential_time / (omp_get_wtime() - time_start));

    return 0;
}