#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

int quicksort_row(void* args);        // main thread function
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
    int sum_value = 0;
    struct thread_params** t_params = (struct thread_params**)malloc(rows * sizeof(struct thread_params*));

    double timer_state = omp_get_wtime();

    #pragma omp parallel num_threads(rows) reduction(+:sum_value)
    {   
        int thread_id = omp_get_thread_num();

        t_params[thread_id] = (struct thread_params*)malloc(sizeof(struct thread_params));
        t_params[thread_id]->rows = rows;
        t_params[thread_id]->cols = cols;
        t_params[thread_id]->row = thread_id;
        t_params[thread_id]->matrix = matrix;

        sum_value += (int) quicksort_row(t_params[thread_id]);
    }

    printf("\nTotal time taken: %lf\n", omp_get_wtime() - timer_state);

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
    printf("\nTotal sum: %d\n", sum_value);

    // free the matrix
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }

    // free t params
    for (int i = 0; i < rows; i++)
    {
        free(t_params[i]);
    }

    return 0;
}

int quicksort_row(void* args)
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
        #pragma omp parallel for
        for (int j = start; j <= end; j++)
            params->sum += matrix[row][j];

        // partitioning
        #pragma omp parallel for
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

        #pragma omp task
        {
            quicksort(l_q_params);
        }

        #pragma omp task
        {
            quicksort(r_q_params);
        }

    }

    return 0;

}