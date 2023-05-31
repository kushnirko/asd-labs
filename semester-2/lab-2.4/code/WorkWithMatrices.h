#include <stdio.h>
#include <stdlib.h>
#include "PrimitiveTableOutput.h"

/************************************************ ПЕРЕЛІК ФУНКЦІЙ *****************************************************/
/***** Функції, що задані в умові лабораторної роботи *****************************************************************/
double **randm                            (int n1, int n2);
int    **mulmr                            (int n1, int n2, double **matrix_T, double coefficient);
/***** Функції, що виконують основні обчислення ***********************************************************************/
int    **SymmetrizeMatrix                 (int n, int **matrix_A);
int    **GetVerticesDegsOfDGraph          (int n, int **matrix_A);
void     PrintDGraphHomogeneityDeg        (int n, int **vertices_degs);
void     PrintLIVerticesOfDGraph          (int n, int **vertices_degs);
int     *GetVerticesDegsOfUGraph          (int n, int **graph_matrix);
void     PrintUGraphHomogeneityDeg        (int n, int *vertices_degs);
void     PrintLIVerticesOfUGraph          (int n, int *vertices_degs);
int    **MultSquareMatrices               (int n, int **matrix_1, int **matrix_2);
void     PrintPathsWithLen2               (int n, int **matrix_pow1, int **matrix_pow2);
void     PrintPathsWithLen3               (int n, int **matrix_pow1, int **matrix_pow3);
int    **MultSquareMatricesElemByElem     (int n, int **matrix_1, int **matrix_2);
int    **TransposeSquareMatrix            (int n, int **matrix);
int    **SumSquareMatrices                (int n, int **matrix_1, int **matrix_2);
int    **GetConnectComponents             (int n, int **matrix_S);
int    **GetReachabilityMatrix            (int n, int **matrix_A);
int    **GetCondensationGraphMatrix       (int n, int **components, int **matrix_mA);
/***** Допоміжні функції **********************************************************************************************/
double   RandInRange                      (double min, double max);
void     PrintConnectComponents           (int n, int **components);
void     PrintDouble2dArr                 (int rows, int cols, double **arr);
int    **Create2dIntArr                   (int rows, int cols);
void     PrintInt2dArr                    (int rows, int cols, int *lengths, char **headings, int **arr);
void     PrintIntArr                      (int arr_length, int *lengths, char *heading, int *arr);
void     PrintBooleanMatrix               (int n, int **matrix);
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

int **SymmetrizeMatrix(int n, int **matrix_A)
{
    int **matrix = Create2dIntArr(n, n);
    int i, j;
    for (i = 0; i < n; i++)
        for (j = i; j < n; j++)
            if (matrix_A[i][j] == 1 || matrix_A[j][i] == 1)
                matrix[i][j] = matrix[j][i] = 1;
    return matrix;
}

int **GetVerticesDegsOfDGraph(int n, int **matrix_A)
{
    int **degs = Create2dIntArr(n, 2);
    /*
     * degs[0-10][1] - напівстепінь виходу вершини;
     * degs[0-10][0] - напівстепінь заходу вершини;
     */
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            if (matrix_A[i][j])
                degs[i][0]++;
            if (matrix_A[j][i])
                degs[i][1]++;
        }
    return degs;
}

void PrintDGraphHomogeneityDeg(int n, int **vertices_degs)
{
    int r = 0, i;
    for (i = 1; i < n; i++)
        if (vertices_degs[0][0] != vertices_degs[i][0])
        {
            r = -1;
            break;
        }
    if (r == -1)
        printf("Graph isn't homogeneous\n");
    else
    {
        r = vertices_degs[0][0];
        printf("Graph homogeneity degree: %d\n", r);
    }
}

void PrintLIVerticesOfDGraph(int n, int **vertices_degs)
{
    int i;
    int leaf_ct = 0, isolated_ct = 0;
    printf("Leaf vertices    : ");
    for (i = 0; i < n; i++)
        if ((vertices_degs[i][0] == 1 && vertices_degs[i][1] == 0) ||
            (vertices_degs[i][0] == 0 && vertices_degs[i][1] == 1))
        {
            printf("%d ", (i + 1));
            leaf_ct++;
        }
    if (!leaf_ct)
        printf("graph hasn't leaf vertices");
    printf("\nIsolated vertices: ");
    for (i = 0; i < n; i++)
        if (vertices_degs[i][0] == 0 &&
            vertices_degs[i][1] == 0)
        {
            printf("%d ", (i + 1));
            isolated_ct++;
        }
    if (!isolated_ct)
        printf("graph hasn't isolated vertices");
    printf("\n");
}

int *GetVerticesDegsOfUGraph(int n, int **graph_matrix)
{
    int *degs = (int *) calloc(n, sizeof(int));
    /* degs[0-10] - степінь вершини */
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (graph_matrix[i][j])
                (i == j) ? (degs[i] += 2) : ++degs[i];
    return degs;
}

void PrintUGraphHomogeneityDeg(int n, int *vertices_degs)
{
    int r = 0, i;
    for (i = 1; i < n; i++)
        if (vertices_degs[0] != vertices_degs[i])
        {
            r = -1;
            break;
        }
    if (r == -1)
        printf("Graph isn't homogeneous\n");
    else
    {
        r = vertices_degs[0];
        printf("Graph homogeneity degree: %d\n", r);
    }
}

void PrintLIVerticesOfUGraph(int n, int *vertices_degs)
{
    int i;
    int leaf_ct = 0, isolated_ct = 0;
    printf("Leaf vertices    : ");
    for (i = 0; i < n; i++)
        if (vertices_degs[i] == 1)
        {
            printf("%d ", (i + 1));
            leaf_ct++;
        }
    if (!leaf_ct)
        printf("graph hasn't leaf vertices");
    printf("\nIsolated vertices: ");
    for (i = 0; i < n; i++)
        if (vertices_degs[i] == 0)
        {
            printf("%d ", (i + 1));
            isolated_ct++;
        }
    if (!isolated_ct)
        printf("graph hasn't isolated vertices");
    printf("\n");
}

int **MultSquareMatrices(int n, int **matrix_1, int **matrix_2)
{
    int **result = Create2dIntArr(n, n);
    int i, j, k;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++)
                result[i][j] += matrix_1[i][k] * matrix_2[k][j];
    return result;
}

int **MultSquareMatricesElemByElem(int n, int **matrix_1, int **matrix_2)
{
    int **result = Create2dIntArr(n, n);
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            result[i][j] = matrix_1[i][j] * matrix_2[i][j];
    return result;
}

int **TransposeSquareMatrix(int n, int **matrix)
{
    int **result = Create2dIntArr(n, n);
    int i, j;
    for (i = 0; i < n; i++)
        for (j = i; j < n; j++)
        {
            result[i][j] = matrix[j][i];
            result[j][i] = matrix[i][j];
        }
    return result;
}

int **SumSquareMatrices(int n, int** matrix_1, int **matrix_2)
{
    int **result = Create2dIntArr(n, n);
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            result[i][j] = matrix_1[i][j] + matrix_2[i][j];
    return result;
}

void PrintPathsWithLen2(int n, int **matrix_pow1, int **matrix_pow2)
{
    int cols_quantity = 4;
    int cols_lengths[] = { 11, 7, 7, 7 };
    PrintTableLine(cols_quantity, cols_lengths, 1);
    printf("%c    No.    %c  v-1  %c  v-2  %c  v-3  %c\n",
           179, 179, 179, 179, 179);
    int path_counter = 1;
    int path[3];
    int i, j, k;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (matrix_pow2[i][j])
            {
                path[0] = i;
                path[2] = j;
                for (k = 0; k < n; k++)
                {
                    if (matrix_pow1[i][k] && matrix_pow1[k][j])
                    {
                        path[1] = k;
                        PrintTableLine(cols_quantity, cols_lengths, 2);
                        printf("%c   %5d   %c  %2d   %c  %2d   %c  %2d   %c\n",
                               179, path_counter,
                               179, (path[0] + 1),
                               179, (path[1] + 1),
                               179, (path[2] + 1), 179);
                        path_counter++;
                    }
                }
            }
        }
    }
    PrintTableLine(cols_quantity, cols_lengths, 3);
}

void PrintPathsWithLen3(int n, int **matrix_pow1, int **matrix_pow3)
{
    int cols_quantity = 5;
    int cols_lengths[] = { 11, 7, 7, 7, 7 };
    PrintTableLine(cols_quantity, cols_lengths, 1);
    printf("%c    No.    %c  v-1  %c  v-2  %c  v-3  %c  v-4  %c\n",
           179, 179, 179, 179, 179, 179);
    int path_counter = 1;
    int path[4];
    int i, j, k, l;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (matrix_pow3[i][j])
            {
                path[0] = i;
                path[3] = j;
                for (k = 0; k < n; k++)
                {
                    if (matrix_pow1[i][k])
                    {
                        for (l = 0; l < n; l++)
                        {
                            if (matrix_pow1[k][l] && matrix_pow1[l][j])
                            {
                                path[1] = k;
                                path[2] = l;
                                PrintTableLine(cols_quantity, cols_lengths, 2);
                                printf("%c   %5d   %c  %2d   %c  %2d   %c  %2d   %c  %2d   %c\n",
                                       179, path_counter,
                                       179, (path[0] + 1),
                                       179, (path[1] + 1),
                                       179, (path[2] + 1),
                                       179, (path[3] + 1), 179);
                                path_counter++;
                            }
                        }
                    }
                }
            }
        }
    }
    PrintTableLine(cols_quantity, cols_lengths, 3);
}

int **GetConnectComponents(int n, int **matrix_S)
{
    int **components = Create2dIntArr(n, (n + 1));
    /*
     * Максимальне число компонент зв'язності = к-сті вершин = n;
     * максимальне число вершин у компоненті зв'язності = к-сті вершин = n;
     * останній елемент у рядку вказуватиме на першу вільну позицію у components[0-10][]
     */
    int counter = 0; /* лічильник вказуватиме на першу вільну позицію у components[] */
    int *included_vertices = (int *) calloc(n, sizeof(int));
    int i, j;
    for (i = 0; i < n; i++)
    {
        if (!included_vertices[i])
        {
            for (j = 0; j < n; j++)
            {
                if (matrix_S[i][j] != 0)
                {
                    included_vertices[j]++;
                    components[counter][components[counter][n]] = j + 1;
                    components[counter][n]++;
                }
            }
            counter++;
        }
    }
    free(included_vertices);
    return components;
}

int **GetReachabilityMatrix(int n, int **matrix_A)
{
    int **matrix = Create2dIntArr(n, n);
    int **t_closing = Create2dIntArr(n, n);
    int i, j;
    for (i = 0; i < n; i++)
    {
        matrix[i][i] = 1; /* додавання одиничної матриці */
        for (j = 0; j < n; j++)
            t_closing[i][j] = matrix_A[i][j];
    }
    for (i = 0; i < n; i++)
    {
        matrix = SumSquareMatrices(n, matrix, t_closing);
        t_closing = MultSquareMatrices(n, t_closing, matrix_A);
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            if (matrix[i][j] != 0)
                matrix[i][j] = 1; /* застосування булевого відображення */
    }
    FreeInt2dArr(n, t_closing);
    return matrix;
}

int **GetCondensationGraphMatrix(int n, int **components, int **matrix_mA)
{
    int index = 0;
    while (index < n && components[index][n] != 0)
        index++;
    int **graph_matrix = Create2dIntArr((index + 1), (index + 1));
    int i, j, k, l;
    for (i = 0; i < index; i++)
    {
        for (j = 0; j < index; j++)
        {
            if (i != j)
            {
                for (k = 0; k < components[i][n]; k++)
                {
                    for (l = 0; l < components[j][n]; l++)
                    {
                        if (matrix_mA[components[i][k] - 1][components[j][l] - 1])
                        {
                            /* Пошук ребра, що з'єднує компоненти */
                            graph_matrix[i][j] = 1;
                            break;
                        }
                    }
                    if (graph_matrix[i][j] == 1)
                        break;
                }
            }
        }
    }
    return graph_matrix;
}

double RandInRange(double min, double max)
{
    double random = (double) rand() / RAND_MAX;
    double range = max - min;
    return min + range * random;
}

void PrintConnectComponents(int n, int **components)
{
    int cols_lengths[] = { 5, 44 };
    PrintTableLine(2, cols_lengths, 1);
    printf("%c No. %c           Vertices in component            %c\n",
           179, 179, 179);
    int components_ct = 0;
    int i;
    while (components_ct < n && components[components_ct][n] != 0)
    {
        PrintTableLine(2, cols_lengths, 2);
        printf("%c %2d  %c", 179, (components_ct + 1), 179);
        for (i = 0; i < n; i++)
        {
            if (components[components_ct][i] != 0)
                printf(" %2d ", components[components_ct][i]);
            else
                printf("    ");
        }
        printf("%c\n", 179);
        components_ct++;
    }
    PrintTableLine(2, cols_lengths, 3);
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
}

int **Create2dIntArr(int rows, int cols)
{
    int **arr = (int **) malloc(sizeof(int *) * rows);
    for (int i = 0; i < rows; i++)
        arr[i] = (int *) calloc(cols, sizeof(int));
    return arr;
}

void PrintInt2dArr(int rows, int cols,
                   int *lengths, char **headings,
                   int **arr)
{
    PrintTableLine((cols + 1), lengths, 1);
    int i, j, k;
    printf("%c No. %c", 179, 179);
    for (i = 0; i < cols; i++)
        printf(" %s %c", headings[i], 179);
    printf("\n");
    int space_before, space_after;
    for (i = 0; i < rows; i++)
    {
        PrintTableLine((cols + 1), lengths, 2);
        printf("%c %2d  %c", 179, (i + 1), 179);
        for (j = 0; j < cols; j++)
        {
            space_before = (int) (lengths[j + 1] / 2) - 1;
            space_after = space_before + (lengths[j + 1] % 2);
            for (k = 0; k < space_before; k++)
                printf(" ");
            printf("%2d", arr[i][j]);
            for (k = 0; k < space_after; k++)
                printf(" ");
            printf("%c", 179);
        }
        printf("\n");
    }
    PrintTableLine((cols + 1), lengths, 3);
}

void PrintIntArr(int arr_length,
                 int *lengths, char *heading,
                 int *arr)
{
    PrintTableLine(2, lengths, 1);
    int i, j;
    printf("%c No. %c %s %c\n", 179, 179, heading, 179);
    int space_before = (int) (lengths[1] / 2) - 1;
    int space_after = space_before + (lengths[1] % 2);
    for (i = 0; i < arr_length; i++)
    {
        PrintTableLine(2, lengths, 2);
        printf("%c %2d  %c", 179, (i + 1), 179);
        for (j = 0; j < space_before; j++)
            printf(" ");
        printf("%2d", arr[i]);
        for (j = 0; j < space_after; j++)
            printf(" ");
        printf("%c\n", 179);
    }
    PrintTableLine(2, lengths, 3);
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
