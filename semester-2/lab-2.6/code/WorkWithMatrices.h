#include <stdio.h>
#include <stdlib.h>

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

int roundm(double value)
{
    int int_part = (int) value;
    return ((value - int_part) >= 0.5 ? (int_part + 1) : int_part);
}

int **GetWtMatrix(int n, int **matrix_A)
{
    int **matrix_Wt = Create2dIntArr(n, n);
    double **matrix_T = randm(n, n);
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            matrix_Wt[i][j] = roundm((matrix_T[i][j] * 100) * matrix_A[i][j]);
    }
    FreeDouble2dArr(n, matrix_T);
    return matrix_Wt;
}

int **SymmetrizeMatrix(int n, int **matrix)
{
    int **symmetric_matrix = Create2dIntArr(n, n);
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n; j++)
            if (matrix[i][j] > 0 || matrix[j][i] > 0)
                symmetric_matrix[i][j] = symmetric_matrix[j][i] =
                        (matrix[i][j] > matrix[j][i] ? matrix[i][j] : matrix[j][i]);
    }
    return symmetric_matrix;
}

int **GetMatrixOfWeights(int n, int **matrix_A)
{
    int **matrix_W;
    /* Формування матриці Wt - пункт 1) */
    int **matrix_Wt = GetWtMatrix(n, matrix_A);

    int **matrix_B = Create2dIntArr(n, n);
    int **matrix_C = Create2dIntArr(n, n);
    int **matrix_D = Create2dIntArr(n, n);

    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            /* Формування матриці B - пункт 2) */
            if (matrix_Wt[i][j] > 0)
                matrix_B[i][j] = 1;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            /* Формування матриці C - пункт 3) */
            if (matrix_B[i][j] != matrix_B[j][i])
                matrix_C[i][j] = 1;
            /* Формування матриці D - пункт 4) */
            if (matrix_B[i][j] == 1 && matrix_B[j][i] == 1)
                matrix_D[i][j] = 1;
            /* Множення матриці D на верхній трикутник матриці одиниць Tr */
            if (j <= i)
                matrix_D[i][j] = 0;
            /* Переприсвоювання значень матриці Wt - пункт 5) */
            matrix_Wt[i][j] = (matrix_C[i][j] + matrix_D[i][j]) * matrix_Wt[i][j];
        }
    }
    /* Формування матриці ваг W шляхом симетризування матриці Wt - пункт 6) */
    matrix_W = SymmetrizeMatrix(n, matrix_Wt);

    FreeInt2dArr(n, matrix_Wt);
    FreeInt2dArr(n, matrix_B);
    FreeInt2dArr(n, matrix_C);
    FreeInt2dArr(n, matrix_D);
    return matrix_W;
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
