#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* quicksort_row(void* args);        // main thread function
void* quicksort(void* args);            // sub-thread callback


// thread parameter structures used for passing data to the threads

struct thread_params
{
    int rows;
    int cols;
    int row;
    int** matrix;
};

struct quicksort_params
{
    int start;
    int end;
    int row;
    int sum;
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
        t_params[i]->row = i;
        t_params[i]->matrix = matrix;

        pthread_create(&threads[i], NULL, &quicksort_row, t_params[i]);
    }

    // wait for all threads to finish
    int total_sum = 0;
    for (int i = 0; i < rows; i++)
    {
        pthread_join(threads[i], &return_values[i]);
        total_sum += return_values[i];
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

    // print total sum
    printf("\nTotal sum: %d\n", total_sum);

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

void* quicksort_row(void* args)
{
    struct thread_params* params = (struct thread_params*)args;

    // extract args
    struct quicksort_params* q_params = (struct quicksort_params*)malloc(sizeof(struct quicksort_params));
    q_params->matrix = params->matrix;
    q_params->row = params->row;
    q_params->start = 0;
    q_params->end = (params->rows) - 1;
    q_params->sum = 0;

    // send quicksort call for a single row
    quicksort(q_params);

    // the sum is changed by reference in the quicksort function
    int row_sum = q_params->sum;
    free(q_params);

    // return the sum
    return row_sum;
}

void* quicksort(void* args)
{

    // extract args
    struct quicksort_params* params = (struct quicksort_params*)args;
    int** matrix = params->matrix;
    int start = params->start;
    int end = params->end;
    int row = params->row;


    // quicksort algorithm
    if (start <= end)
    {

        int pivot = matrix[row][end];

        int i = start - 1;

        // row summing
        for (int j = start; j <= end; j++)
            params->sum += matrix[row][j];

        // partitioning
        for (int j = start; j < end; j++)
        {            
            if (matrix[row][j] > pivot)
            {
                i++;
                int temp = matrix[row][i];
                matrix[row][i] = matrix[row][j];
                matrix[row][j] = temp;
            }
        }

        int temp = matrix[row][i + 1];
        matrix[row][i + 1] = matrix[row][end];
        matrix[row][end] = temp;

        struct quicksort_params* l_q_params = (struct quicksort_params*)malloc(sizeof(struct quicksort_params));
        l_q_params->matrix = matrix;
        l_q_params->row = row;
        l_q_params->start = 0;
        l_q_params->end = (i + 1) - 1;
        l_q_params->sum = 0;

        struct quicksort_params* r_q_params = (struct quicksort_params*)malloc(sizeof(struct quicksort_params));
        r_q_params->matrix = matrix;
        r_q_params->row = row;
        r_q_params->start = (i + 1) + 1;
        r_q_params->end = end;
        r_q_params->sum = 0;


        pthread_t left_thread;
        pthread_t right_thread;


        // recurse on left and right halves
        // multilthreaded version
        int left_ret = pthread_create(&left_thread, NULL, &quicksort, l_q_params);
        int right_ret = pthread_create(&right_thread, NULL, &quicksort, r_q_params);

        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

    }

    return 0;

}