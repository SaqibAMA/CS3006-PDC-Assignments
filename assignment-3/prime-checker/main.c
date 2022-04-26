#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{

    // p: number of threads
    // n: number of primes to check
    // file: file to write primes to

    // check command line args
    if (argc != 4)
    {
        printf("Usage: ./main <p> <n> <file|stdout>\n");
        return 1;
    }

    // get command line args
    int p = atoi(argv[1]);
    int n = atoi(argv[2]);
    char* filename = argv[3];
    int console_out = 0;

    // check if filename is stdout
    if (strcmp(filename, "stdout") == 0)
    {
        console_out = 1;
    }
    else {
        // open file and clean it
        FILE* file = fopen(filename, "w");
        fclose(file);
    }

    // set nested
    omp_set_nested(1);

    // fill a boolean array of all primes
    int* primes = malloc(sizeof(int) * n);
    memset(primes, 1, sizeof(int) * n);

    // core algorithm
    #pragma omp parallel for num_threads(p)
    for (int i = 3; i < n; i++)
    {
        #pragma omp parallel for num_threads(p) shared(primes)
        for (int j = 2; j < i; j++)
        {
            if (i % j == 0)
            {
                #pragma omp critical
                {
                    primes[i - 3] = 0;
                }
            }
        }
    }

    // write primes to file if console_out is 0
    for (int i = 3; i < n; i++)
    {
        // printf("%d\n", primes[i - 3]);
        if (primes[i - 3] != 0)
        {
            if (console_out == 0)
            {
                FILE* file = fopen(filename, "a");
                fprintf(file, "%d\n", i + 3);
                fclose(file);
            }
            else {
                printf("%d\n", i + 3);
            }
        }
    }

    // free memory
    free(primes);

    return 0;
}
