#include <stdio.h>
#include <stdlib.h>

/* Допоміжні функції */
double RandInRange(double min, double max);
int **CreateInt2dArr(int rows, int cols);
void PrintDouble2dArr(int rows, int cols, double **arr);
void FreeInt2dArr(int rows, int **arr);
void FreeDouble2dArr(int rows, double **arr);

double **randm(int rows, int cols)
{
    double **matrix_T = (double **) malloc(sizeof(double *) * rows);
    int i, j;
    for (i = 0; i < rows; i++)
    {
        matrix_T[i] = (double *) malloc(sizeof(double) * cols);
        for (j = 0; j < cols; j++)
            matrix_T[i][j] = RandInRange(0.0, 2.0);
    }
    return matrix_T;
}

int **mulmr(int rows, int cols, double **matrix_T, double coefficient)
{
    int **matrix_A = CreateInt2dArr(rows, cols);
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
            matrix_A[i][j] = (int) (matrix_T[i][j] * coefficient);
    }
    return matrix_A;
}

int **SymmetrizeMatrix(int n, int **matrix_A)
{
    int **symmetric_matrix = CreateInt2dArr(n, n);
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n; j++)
            if (matrix_A[i][j] == 1 || matrix_A[j][i] == 1)
                symmetric_matrix[i][j] = symmetric_matrix[j][i] = 1;
    }
    return symmetric_matrix;
}

double RandInRange(double min, double max)
{
    double random = (double) rand() / RAND_MAX;
    double range = max - min;
    return min + range * random;
}

int **CreateInt2dArr(int rows, int cols)
{
    int **arr = (int **) malloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
        arr[i] = (int *) calloc(cols, sizeof(int));
    return arr;
}

void PrintDouble2dArr(int rows, int cols, double **arr)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
            printf("%lf ", arr[i][j]);
        printf("\n");
    }
    printf("\n");
}

void FreeDouble2dArr(int rows, double **arr)
{
    for (int i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);
}

void FreeInt2dArr(int rows, int **arr)
{
    for (int i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);
}
