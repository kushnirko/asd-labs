#include <stdio.h>
#include <stdlib.h>
#include "PrimitiveTableOutput.h"

/***** Допоміжні функції **********************************************************************************************/
double   RandInRange                      (double min, double max);
int    **Create2dIntArr                   (int rows, int cols);
void     FreeInt2dArr                     (int rows, int **arr);
void     FreeDouble2dArr                  (int rows, double **arr);
/**********************************************************************************************************************/

double **randm(int n1, int n2)
{
    double **matrix_T = (double **) malloc(sizeof(double *) * n1);
    int i, j;
    for (i = 0; i < n1; i++)
    {
        matrix_T[i] = (double *) malloc(sizeof(double) * n2);
        for (j = 0; j < n2; j++)
            matrix_T[i][j] = RandInRange(0.0, 2.0);
    }
    return matrix_T;
}

int **mulmr(int n1, int n2, double **matrix_T, double coefficient)
{
    int **matrix_A = Create2dIntArr(n1, n2);
    int i, j;
    for (i = 0; i < n1; i++)
    {
        for (j = 0; j < n2; j++)
            matrix_A[i][j] = (int) (matrix_T[i][j] * coefficient);
    }
    return matrix_A;
}

double RandInRange(double min, double max)
{
    double random = (double) rand() / RAND_MAX;
    double range = max - min;
    return min + range * random;
}

int **Create2dIntArr(int rows, int cols)
{
    int **arr = (int **) malloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
        arr[i] = (int *) calloc(cols, sizeof(int));
    return arr;
}

void PrintSequence(int n, int *sequence)
{
    int cols_lengths[] = { 8, 17 };
    PrintTableLine(2, cols_lengths, 1);
    printf("%c Vertex %c Sequence number %c\n", 179, 179, 179);
    for (int i = 0; i < n; i++)
    {
        PrintTableLine(2, cols_lengths, 2);
        printf("%c   %2d   %c       %2d        %c\n", 179, (i + 1), 179, sequence[i], 179);
    }
    PrintTableLine(2, cols_lengths, 3);
}

void PrintBooleanMatrix(int n, int **matrix)
{
    int i, j;
    int cols_quantity = n;
    int *cols_lengths = (int *) calloc(cols_quantity, sizeof(int));
    printf("      %c", 179);
    for (i = 0; i < n; i++)
    {
        cols_lengths[i] = 3;
        printf("%2d %c", (i + 1), 179);
    }
    printf("\n");
    for (i = 0; i < n; i++)
    {
        printf("   %c%c%c", 196, 196, 196);
        PrintTableLine(cols_quantity, cols_lengths, 2);
        printf("   %2d %c", (i + 1), 179);
        for (j = 0; j < n; j++)
            printf("%2d %c", matrix[i][j], 179);
        printf("\n");
    }
    printf("   %c%c%c", 196, 196, 196);
    PrintTableLine(cols_quantity, cols_lengths, 3);
    free(cols_lengths);
}

void FreeInt2dArr(int rows, int **arr)
{
    for (int i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);
}

void FreeDouble2dArr(int rows, double **arr)
{
    for (int i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);
}
