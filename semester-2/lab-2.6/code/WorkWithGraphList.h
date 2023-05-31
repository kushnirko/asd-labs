typedef struct VertexData
{
    int index;
    wchar_t *name;
    int coords[2];
    struct VertexData *next;
} vertex;

typedef struct EdgeData
{
    vertex *vertex1;
    vertex *vertex2;
    int weight;
    struct EdgeData *next;
} edge;

typedef struct SetOfVertices
{
    vertex *first_vertex;
    vertex *last_vertex;
} v_set;

typedef struct SetOfEdges
{
    edge *first_edge;
    edge *last_edge;
} e_set;

typedef struct GraphList
{
    v_set *vertices;
    e_set *edges;
} graph;

/****************************************** ПЕРЕЛІК ФУНКЦІЙ **************************************/
graph  *InitGraph           ();
void    RefreshGraph        (graph *g);
void    FreeGraph           (graph *g);
void    PrintGraph          (graph *g);
/***** Функції для роботи з множиною вершин ******************************************************/
int     IsVerticesSetEmpty  (graph *g);
int     IsVerticesSetFull   (int n, graph *g);
vertex *FindVertex          (graph *g, int v_index);
void    AddVertex           (graph *g, int v_index, wchar_t *v_name, const int *v_coords);
void    CopyVertex          (graph *to, graph *from, vertex *v);
/***** Функції для роботи з множиною ребер *******************************************************/
int     IsEdgeSetEmpty      (graph *g);
edge   *FindEdge            (graph *g, vertex *v1, vertex *v2);
void    AddEdge             (graph *g, vertex *v1_index, vertex *v2_index, int weight);
void    CopyEdge            (graph *to, graph *from, vertex *v1, vertex *v2);
/*************************************************************************************************/

graph *ConvertDataToGraphList(int n,
                              wchar_t **v_names,
                              int **v_coords,
                              int **matrix_uA,
                              int **matrix_W)
{
    graph *g = InitGraph();
    int i, j;
    for(i = 0; i < n; i++)
        AddVertex(g, i, v_names[i], v_coords[i]);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (matrix_uA[i][j] == 1)
                AddEdge(g,
                        FindVertex(g, i),
                        FindVertex(g, j),
                        matrix_W[i][j]);
    return g;
}

graph *InitGraph()
{
    graph *g = (graph *) malloc(sizeof(graph));
    g->vertices = (v_set *) malloc(sizeof (v_set));
    g->edges = (e_set *) malloc(sizeof(e_set));

    g->vertices->first_vertex = g->vertices->last_vertex = NULL;
    g->edges->first_edge = g->edges->last_edge = NULL;

    return g;
}

int IsVerticesSetEmpty(graph *g)
{
    return (g->vertices->first_vertex == NULL);
}

int IsVerticesSetFull(int n, graph *g)
{
    int max_indices_sum = 0;
    int current_indices_sum = 0;
    for (int i = 0; i < n; i++)
        max_indices_sum += i;
    vertex *current_v = g->vertices->first_vertex;
    while (current_v != NULL)
    {
        current_indices_sum += current_v->index;
        current_v = current_v->next;
    }
    return (current_indices_sum == max_indices_sum ? 1 : 0);
}

vertex *FindVertex(graph *g, int v_index)
{
    vertex *current_v = g->vertices->first_vertex;
    while(current_v != NULL)
    {
        if (current_v->index == v_index)
            return current_v;
        else
            current_v = current_v->next;
    }
    return NULL;
}

void AddVertex(graph *g, int v_index, wchar_t *v_name, const int *v_coords)
{
    vertex *v = (vertex *) malloc(sizeof(vertex));
    v->index = v_index;
    v->name = v_name;
    v->coords[x] = v_coords[x];
    v->coords[y] = v_coords[y];
    v->next = NULL;
    if (!IsVerticesSetEmpty(g))
    {
        g->vertices->last_vertex->next = v;
        g->vertices->last_vertex = v;
    }
    else
        g->vertices->last_vertex = g->vertices->first_vertex = v;
}

void CopyVertex(graph *to, graph *from, vertex *v)
{
    vertex *copied_v = (vertex *) malloc(sizeof(vertex));
    vertex *original_v = FindVertex(from, v->index);
    copied_v->index = original_v->index;
    copied_v->name = original_v->name;
    copied_v->coords[x] = original_v->coords[x];
    copied_v->coords[y] = original_v->coords[y];
    copied_v->next = NULL;
    if (!IsVerticesSetEmpty(to))
    {
        to->vertices->last_vertex->next = copied_v;
        to->vertices->last_vertex = copied_v;
    }
    else
        to->vertices->last_vertex = to->vertices->first_vertex = copied_v;
}

int IsEdgeSetEmpty(graph *g)
{
    return (g->edges->first_edge == NULL);
}

edge *FindEdge(graph *g, vertex *v1, vertex *v2)
{
    edge *current_e = g->edges->first_edge;
    while (current_e != NULL)
    {
        if (current_e->vertex1->index == v1->index &&
            current_e->vertex2->index == v2->index)
            return current_e;
        else
            current_e = current_e->next;
    }
    return NULL;
}

void AddEdge(graph *g, vertex *v1, vertex *v2, int weight)
{
    edge *e = (edge *) malloc(sizeof(edge));
    e->vertex1 = v1;
    e->vertex2 = v2;
    e->weight = weight;
    e->next = NULL;
    if (!IsEdgeSetEmpty(g))
    {
        g->edges->last_edge->next = e;
        g->edges->last_edge = e;
    }
    else
        g->edges->last_edge = g->edges->first_edge = e;
}

void CopyEdge(graph *to, graph *from, vertex *v1, vertex *v2)
{
    edge *copied_e = (edge *) malloc(sizeof(edge));
    edge *original_e = FindEdge(from, v1, v2);
    copied_e->vertex1 = FindVertex(to, v1->index);
    copied_e->vertex2 = FindVertex(to, v2->index);
    copied_e->weight = original_e->weight;
    copied_e->next = NULL;
    if (!IsEdgeSetEmpty(to))
    {
        to->edges->last_edge->next = copied_e;
        to->edges->last_edge = copied_e;
    }
    else
        to->edges->last_edge = to->edges->first_edge = copied_e;
}

void RefreshGraph(graph *g)
{
    vertex *temp_v;
    while (!IsVerticesSetEmpty(g))
    {
        temp_v = g->vertices->first_vertex;
        g->vertices->first_vertex = g->vertices->first_vertex->next;
        free(temp_v);
    }
    edge *temp_e;
    while (!IsEdgeSetEmpty(g))
    {
        temp_e = g->edges->first_edge;
        g->edges->first_edge = g->edges->first_edge->next;
        free(temp_e);
    }
}

void FreeGraph(graph *g)
{
    RefreshGraph(g);
    free(g->vertices);
    free(g->edges);
    free(g);
    g = NULL;
}

void PrintGraph(graph *g)
{
    printf("{");
    /* Друкуємо множину вершин */
    if (IsVerticesSetEmpty(g))
        printf("%c", 155);
    else
    {
        printf("{");
        vertex *current_v = g->vertices->first_vertex;
        while (current_v->next != NULL)
        {
            wprintf(L"%ls, ", current_v->name);
            current_v = current_v->next;
        }
        wprintf(L"%ls", current_v->name);
        printf("},\n");
    }
    /* Друкуємо множину ребер */
    if (IsEdgeSetEmpty(g))
        printf(" %c ",155);
    else
    {
        edge *current_e = g->edges->first_edge;
        while (current_e->next != NULL)
        {
            wprintf(L" (%ls, %ls) ",
                   current_e->vertex1->name,
                   current_e->vertex2->name);
            printf("[ weight = %3d ],\n", current_e->weight);
            current_e = current_e->next;
        }
        wprintf(L" (%ls, %ls) ",
                current_e->vertex1->name,
                current_e->vertex2->name);
        printf("[ weight = %3d ] ", current_e->weight);
    }
    /* у таблиці ASCII ((char) 155) - це щось схоже на символ порожньої множини :) */
    printf("}\n");
}
