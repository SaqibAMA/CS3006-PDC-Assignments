#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_ACTIVE_LEVELS 1

struct matrix {
    int** data;
    int rows;
    int cols;
};

void default_scheduling_benchmark(struct matrix* a, struct matrix* b, struct matrix* c) {
    int i, j, k;
    #pragma omp parallel for
    for (i = 0; i < a->rows; i++) {
        #pragma omp parallel for
        for (j = 0; j < b->cols; j++) {
            c->data[i][j] = 0;
            #pragma omp parallel for
            for (k = 0; k < a->cols; k++) {
                c->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void static_scheduling_benchmark(struct matrix* a, struct matrix* b, struct matrix* c) {
    int i, j, k;
    #pragma omp parallel for schedule(static)
    for (i = 0; i < a->rows; i++) {
        #pragma omp parallel for schedule(static)
        for (j = 0; j < b->cols; j++) {
            c->data[i][j] = 0;
            #pragma omp parallel for schedule(static)
            for (k = 0; k < a->cols; k++) {
                c->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void dynamic_scheduling_benchmark(struct matrix* a, struct matrix* b, struct matrix* c) {
    int i, j, k;
    #pragma omp parallel for schedule(dynamic)
    for (i = 0; i < a->rows; i++) {
        #pragma omp parallel for schedule(dynamic)
        for (j = 0; j < b->cols; j++) {
            c->data[i][j] = 0;
            #pragma omp parallel for schedule(dynamic)
            for (k = 0; k < a->cols; k++) {
                c->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void explicit_thread_default_scheduling_benchmark(struct matrix* a, struct matrix* b, struct matrix* c) {

    omp_set_nested(1);
    omp_set_max_active_levels(MAX_ACTIVE_LEVELS);

    int i, j, k;
    #pragma omp parallel for num_threads(a->rows)
    for (i = 0; i < a->rows; i++) {
        #pragma omp parallel for num_threads(b->cols)
        for (j = 0; j < b->cols; j++) {
            c->data[i][j] = 0;
            #pragma omp parallel for
            for (k = 0; k < a->cols; k++) {
                c->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void explicit_thread_static_scheduling_benchmark(struct matrix* a, struct matrix* b, struct matrix* c) {

    omp_set_nested(1);
    omp_set_max_active_levels(MAX_ACTIVE_LEVELS);

    int i, j, k;
    #pragma omp parallel for schedule(static) num_threads(a->rows)
    for (i = 0; i < a->rows; i++) {
        #pragma omp parallel for schedule(static) num_threads(b->cols)
        for (j = 0; j < b->cols; j++) {
            c->data[i][j] = 0;
            #pragma omp parallel for schedule(static)
            for (k = 0; k < a->cols; k++) {
                c->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void explicit_thread_dynamic_scheduling_benchmark(struct matrix* a, struct matrix* b, struct matrix* c) {

    omp_set_nested(1);
    omp_set_max_active_levels(MAX_ACTIVE_LEVELS);

    int i, j, k;
    #pragma omp parallel for schedule(dynamic) num_threads(a->rows)
    for (i = 0; i < a->rows; i++) {
        #pragma omp parallel for schedule(dynamic) num_threads(b->cols) 
        for (j = 0; j < b->cols; j++) {
            c->data[i][j] = 0;
            #pragma omp parallel for schedule(dynamic)
            for (k = 0; k < a->cols; k++) {
                c->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

int main() {

    // initializing matrices
    struct matrix A, B, C;
    A.rows = B.rows = 100;
    A.cols = B.cols = 100;
    C.rows = A.rows;
    C.cols = B.cols;

    A.data = (int**) malloc(A.rows * sizeof(int*));
    B.data = (int**) malloc(B.rows * sizeof(int*));
    C.data = (int**) malloc(C.rows * sizeof(int*));

    for (int i = 0; i < A.rows; i++) {
        A.data[i] = (int*) malloc(A.cols * sizeof(int));
        B.data[i] = (int*) malloc(B.cols * sizeof(int));
        C.data[i] = (int*) malloc(C.cols * sizeof(int));
    }

    // fill both matrices with random values, C with 0
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            A.data[i][j] = rand() % 100;
            B.data[i][j] = rand() % 100;
            C.data[i][j] = 0;
        }
    }

    double timer_start;
    
    // running default benchmark
    printf(" -- default threads -- \n");
    timer_start = omp_get_wtime();
    default_scheduling_benchmark(&A, &B, &C);
    printf("Default benchmark took %f seconds\n", omp_get_wtime() - timer_start);

    // running static benchmark
    timer_start = omp_get_wtime();
    static_scheduling_benchmark(&A, &B, &C);
    printf("Static benchmark took %f seconds\n", omp_get_wtime() - timer_start);

    // running dynamic benchmark
    timer_start = omp_get_wtime();
    dynamic_scheduling_benchmark(&A, &B, &C);
    printf("Dynamic benchmark took %f seconds\n", omp_get_wtime() - timer_start);

    // running explicit thread default benchmark
    printf(" -- explicit thread default -- \n");
    timer_start = omp_get_wtime();
    explicit_thread_default_scheduling_benchmark(&A, &B, &C);
    printf("Explicit thread default benchmark took %f seconds\n", omp_get_wtime() - timer_start);

    // running explicit thread static benchmark
    timer_start = omp_get_wtime();
    explicit_thread_static_scheduling_benchmark(&A, &B, &C);
    printf("Explicit thread static benchmark took %f seconds\n", omp_get_wtime() - timer_start);

    // running explicit thread dynamic benchmark
    timer_start = omp_get_wtime();
    explicit_thread_dynamic_scheduling_benchmark(&A, &B, &C);
    printf("Explicit thread dynamic benchmark took %f seconds\n", omp_get_wtime() - timer_start);

    return 0;
}