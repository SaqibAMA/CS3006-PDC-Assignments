#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // argument format: <sizeOfArray>

    // check args length
    if (argc != 2) {
        printf("Wrong number of arguments.\n");
        return 1;
    }

    // get size of array
    int n = atoi(argv[1]);

    // allocate array
    int *a = (int *) malloc(n * sizeof(int));

    // fill array with random elements
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100;
    }

    // get which key user wants to search
    int key;
    printf("Enter key to search: ");
    scanf("%d", &key);

    // perform linear search on array
    int occurences = 0;
    int par_start = omp_get_wtime();
    #pragma omp parallel for reduction(+:occurences)
    for (int i = 0; i < n; i++)
        occurences += (a[i] == key);

    // print the occurences
    printf("Parallel occurences found: %d\n", occurences);
    printf("Parallel time taken: %lf\n", omp_get_wtime() - par_start);

    // free memory
    free(a);

    return 0;
}