#include "WorkWithQueueAndStack.h"

int FindMaxSequenceNumber(int n, int *sequence);
int FindFirstVertex(int n, int **graph_matrix, int *sequence);

int PerformBFSStep(int n, int **graph_matrix, int *sequence, queue *Q, int **tree_matrix)
{
    int sequence_number = FindMaxSequenceNumber(n, sequence);
    if (IsQueueEmpty(Q))
    {
        int first_vertex = FindFirstVertex(n, graph_matrix, sequence);
        if (first_vertex != -1)
        {
            sequence_number++;
            sequence[first_vertex] = sequence_number;
            Enqueue(Q, first_vertex);
        }
        return -1;
    }
    int active_vertex = Q->tail->data;
    for (int i = 0; i < n; i++)
    {
        if (graph_matrix[active_vertex][i] == 1)
        {
            if (sequence[i] == 0)
            {
                sequence_number++;
                sequence[i] = sequence_number;
                tree_matrix[active_vertex][i] = 1;
                Enqueue(Q, i);
                return -1;
            }
        }
    }
    Dequeue(Q);
    return active_vertex;
}

int PerformDFSStep(int n, int **graph_matrix, int *sequence, stack *S, int **tree_matrix)
{
    int sequence_number = FindMaxSequenceNumber(n, sequence);
    if (IsStackEmpty(S))
    {
        int first_vertex = FindFirstVertex(n, graph_matrix, sequence);
        if (first_vertex != -1)
        {
            sequence_number++;
            sequence[first_vertex] = sequence_number;
            Push(S, first_vertex);
        }
        return -1;
    }
    int active_vertex = S->head->data;
    for (int i = 0; i < n; i++)
    {
        if (graph_matrix[active_vertex][i] == 1)
        {
            if (sequence[i] == 0)
            {
                sequence_number++;
                sequence[i] = sequence_number;
                tree_matrix[active_vertex][i] = 1;
                Push(S, i);
                return -1;
            }
        }
    }
    Pop(S);
    return active_vertex;
}

int FindMaxSequenceNumber(int n, int *sequence)
{
    int max_number = 0;
    int i;
    for (i = 0; i < n; i++)
        if (sequence[i] > max_number)
            max_number = sequence[i];
    return max_number;
}

int FindFirstVertex(int n, int **graph_matrix, int *sequence)
{
    int i, j;
    for (i = 0; i < n; i++)
        if (sequence[i] == 0 )
            for (j = 0; j < n; j++)
                if (graph_matrix[i][j] && (i != j))
                    return i;
    for (i = 0; i < n; i++)
        if (sequence[i] == 0 )
            return i;
    return -1;
}

int CheckTraversalState(int n, int *sequence)
{
    int sequence_nums_sum = 0;
    int max_sequence_nums_sum = 0;
    for (int i = 0; i < n; i++)
    {
        sequence_nums_sum += sequence[i];
        max_sequence_nums_sum += (i + 1);
    }
    if (sequence_nums_sum == 0)
        return 1; /* поточний стан обходу - TRAVERSAL_START */
    else if (sequence_nums_sum == max_sequence_nums_sum)
        return 3; /* поточний стан обходу - TRAVERSAL_END */
    else
        return 2; /* поточний стан обходу - TRAVERSAL_CONTINUATION */
}
