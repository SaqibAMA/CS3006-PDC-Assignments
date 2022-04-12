#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

void* row_wise(void* args);

int main()
{

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

    // create a thread array with size of A.rows
    pthread_t* threads = (pthread_t*)malloc(C.rows * sizeof(pthread_t));
    struct row_params** t_params = (struct row_params**)malloc(C.rows * sizeof(struct row_params*));

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

    // clear t params
    for (int i = 0; i < C.rows; i++)
    {
        free(t_params[i]);
    }

    return 0;
}

void* row_wise(void* args)
{
    
    struct row_params* params = (struct row_params*)args;
    struct matrix A = params->A;
    struct matrix B = params->B;
    struct matrix C = params->C;
    int row = params->row;

    printf("Thread %d started.\n", row);

    return NULL;
}