#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t lock;   // mutex lock

struct matrix
{
    int** m;
    int rows;
    int cols;
};

struct row_params
{
    int row;
    struct matrix A;
    struct matrix B;
    struct matrix C;
};

struct col_params
{
    int col;
    int row;
    struct matrix A;
    struct matrix B;
    struct matrix C;
};

void* row_wise(void* args);
void* col_wise(void* args);

int main()
{

    // mutex init
    pthread_mutex_init(&lock, NULL);

    // matrix structs    
    struct matrix A;
    struct matrix B;

    struct matrix C;

    // matrix A inputs
    printf("Enter the number of rows for matrix A: ");
    scanf("%d", &A.rows);
    printf("Enter the number of columns for matrix A: ");
    scanf("%d", &A.cols);

    // matrix B inputs
    printf("Enter the number of rows for matrix B: ");
    scanf("%d", &B.rows);
    printf("Enter the number of columns for matrix B: ");
    scanf("%d", &B.cols);

    // check if matrices can be multiplied
    if (A.cols != B.rows)
    {
        printf("Matrices cannot be multiplied.\n");
        return 0;
    }

    // set rows and columns for resultant matrix c
    C.rows = A.rows;
    C.cols = B.cols;

    // allocate memory for both matrices
    A.m = (int**)malloc(A.rows * sizeof(int*));
    B.m = (int**)malloc(B.rows * sizeof(int*));
    C.m = (int**)malloc(C.rows * sizeof(int*));

    // allocate memory for each row
    for (int i = 0; i < A.rows; i++)
    {
        A.m[i] = (int*)malloc(A.cols * sizeof(int));
    }

    for (int i = 0; i < B.rows; i++)
    {
        B.m[i] = (int*)malloc(B.cols * sizeof(int));
    }

    for (int i = 0; i < C.rows; i++)
    {
        C.m[i] = (int*)malloc(C.cols * sizeof(int));
    }

    // fill both matrices with random values
    for (int i = 0; i < A.rows; i++)
    {
        for (int j = 0; j < A.cols; j++)
        {
            A.m[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < B.rows; i++)
    {
        for (int j = 0; j < B.cols; j++)
        {
            B.m[i][j] = rand() % 10;
        }
    }

    // fill the resultant matrix with 0s
    for (int i = 0; i < C.rows; i++)
    {
        for (int j = 0; j < C.cols; j++)
        {
            C.m[i][j] = 0;
        }
    }

    // print both matrices
    printf("Matrix A:\n");
    for (int i = 0; i < A.rows; i++)
    {
        for (int j = 0; j < A.cols; j++)
        {
            printf("%d ", A.m[i][j]);
        }
        printf("\n");
    }

    printf("Matrix B:\n");
    for (int i = 0; i < B.rows; i++)
    {
        for (int j = 0; j < B.cols; j++)
        {
            printf("%d ", B.m[i][j]);
        }
        printf("\n");
    }

    // starting to calculate time
    clock_t start = clock();

    // create a thread array with size of A.rows
    pthread_t* threads = (pthread_t*)malloc(A.rows * sizeof(pthread_t));
    struct row_params** t_params = (struct row_params**)malloc(A.rows * sizeof(struct row_params*));

    // task decomposition for matrix c
    for (int i = 0; i < C.rows; i++)
    {
        t_params[i] = (struct row_params*)malloc(sizeof(struct row_params));
        t_params[i]->A = A;
        t_params[i]->B = B;
        t_params[i]->C = C;
        t_params[i]->row = i;
        pthread_create(&threads[i], NULL, &row_wise, t_params[i]);
    }

    // join the threads
    for (int i = 0; i < C.rows; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // free threads
    free(threads);

    // calculate the total time taken
    clock_t end = clock();
    printf("Total time taken: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    // clear t params
    for (int i = 0; i < C.rows; i++)
    {
        free(t_params[i]);
    }

    // print matrix c
    printf("Resultant Matrix C:\n");
    for (int i = 0; i < C.rows; i++)
    {
        for (int j = 0; j < C.cols; j++)
        {
            printf("%d ", C.m[i][j]);
        }
        printf("\n");
    }

    // destroy mutex
    pthread_mutex_destroy(&lock);

    return 0;
}

void* row_wise(void* args)
{
    
    struct row_params* params = (struct row_params*)args;
    struct matrix A = params->A;
    struct matrix B = params->B;
    struct matrix C = params->C;
    int row = params->row;

    // task decomposition

    pthread_t* col_threads = (pthread_t*)malloc(B.cols * sizeof(pthread_t));
    struct col_params** c_params = (struct col_params**)malloc(B.cols * sizeof(struct col_params*));
    for (int i = 0; i < B.cols; i++)
    {
        c_params[i] = (struct col_params*)malloc(sizeof(struct col_params));
        c_params[i]->A = A;
        c_params[i]->B = B;
        c_params[i]->C = C;
        c_params[i]->row = row;
        c_params[i]->col = i;

        pthread_create(&col_threads[i], NULL, &col_wise, c_params[i]);
    }

    // join all threads
    for (int i = 0; i < B.cols; i++)
    {
        pthread_join(col_threads[i], NULL);
    }

    // free threads
    free(col_threads);

    // free col params
    for (int i = 0; i < B.cols; i++)
    {
        free(c_params[i]);
    }


    return NULL;
}

void* col_wise(void* args)
{
    struct col_params* params = (struct col_params*)args;
    struct matrix A = params->A;
    struct matrix B = params->B;
    struct matrix C = params->C;
    int row = params->row;
    int col = params->col;

    // data decomposition
    for (int i = 0; i < A.cols; i++)
    {
        // critical section -- start
        pthread_mutex_lock(&lock);
        C.m[row][col] += A.m[row][i] * B.m[i][col];
        pthread_mutex_unlock(&lock);
        // critical section -- end
    }

    return NULL;

}