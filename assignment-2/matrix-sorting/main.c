#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* quicksort_column(void* args);
void* quicksort(void* args);

struct thread_params
{
    int rows;
    int cols;
    int column;
    int** matrix;
};

struct quicksort_params
{
    int start;
    int end;
    int column;
    int** matrix;
};

int main()
{
    
    int rows, cols;
    int** matrix;
    
    // take input for rows and columns
    printf("Enter the number of rows: ");
    scanf("%d", &rows);
    printf("Enter the number of columns: ");
    scanf("%d", &cols);

    // add validation for rows and columns
    if (rows < 1 || cols < 1)
    {
        printf("Invalid input.\n");
        return 0;
    }

    // allocate memory for the matrix
    matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    // fill the entire matrix with random values
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = rand() % 100;
        }
    }

    // print matrix
    printf("\nMatrix:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // create threads equal to the number of rows
    pthread_t* threads = (pthread_t*)malloc(rows * sizeof(pthread_t));
    int* return_values = (int*)malloc(rows * sizeof(int));
    struct thread_params** t_params = (struct thread_params**)malloc(rows * sizeof(struct thread_params*));

    for (int i = 0; i < rows; i++)
    {   
        t_params[i] = (struct thread_params*)malloc(sizeof(struct thread_params));
        t_params[i]->rows = rows;
        t_params[i]->cols = cols;
        t_params[i]->column = i;
        t_params[i]->matrix = matrix;

        return_values[i] = pthread_create(&threads[i], NULL, &quicksort_column, t_params[i]);
    }

    // wait for all threads to finish
    for (int i = 0; i < rows; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Matrix
    printf("\nMatrix after sorting:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // free the matrix
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }

    // free return values
    free(return_values);

    // free t params
    for (int i = 0; i < rows; i++)
    {
        free(t_params[i]);
    }

    return 0;
}

void* quicksort_column(void* args)
{
    struct thread_params* params = (struct thread_params*)args;

    struct quicksort_params* q_params = (struct quicksort_params*)malloc(sizeof(struct quicksort_params));
    q_params->matrix = params->matrix;
    q_params->column = params->column;
    q_params->start = 0;
    q_params->end = (params->rows) - 1;

    quicksort(q_params);

    free(q_params);

    return 0;

}

void* quicksort(void* args)
{

    struct quicksort_params* params = (struct quicksort_params*)args;
    int** matrix = params->matrix;
    int start = params->start;
    int end = params->end;
    int column = params->column;

    if (start < end)
    {

        int pivot = matrix[end][column];

        int i = start - 1;

        for (int j = start; j < end; j++)
        {
            if (matrix[j][column] <= pivot)
            {
                i++;
                int temp = matrix[i][column];
                matrix[i][column] = matrix[j][column];
                matrix[j][column] = temp;
            }
        }

        int temp = matrix[i + 1][column];
        matrix[i + 1][column] = matrix[end][column];
        matrix[end][column] = temp;

        struct quicksort_params* l_q_params = (struct quicksort_params*)malloc(sizeof(struct quicksort_params));
        l_q_params->matrix = matrix;
        l_q_params->column = column;
        l_q_params->start = 0;
        l_q_params->end = (i + 1) - 1;

        struct quicksort_params* r_q_params = (struct quicksort_params*)malloc(sizeof(struct quicksort_params));
        r_q_params->matrix = matrix;
        r_q_params->column = column;
        r_q_params->start = (i + 1) + 1;
        r_q_params->end = end;

        pthread_t left_thread;
        pthread_t right_thread;

        int left_ret = pthread_create(&left_thread, NULL, &quicksort, l_q_params);
        int right_ret = pthread_create(&right_thread, NULL, &quicksort, r_q_params);

        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        free(l_q_params);
        free(r_q_params);

    }

    return 0;

}