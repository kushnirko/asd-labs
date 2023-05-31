void PerformPrimAlgStep(graph *Gt, graph *G)
{
    if (IsVerticesSetEmpty(Gt))
    {
        CopyVertex(Gt, G, G->vertices->first_vertex);
        return;
    }
    edge *current_e = G->edges->first_edge, *lightest_e = NULL;
    while (current_e != NULL)
    {
        if (FindVertex(Gt, current_e->vertex1->index) != NULL &&
            FindVertex(Gt, current_e->vertex2->index) == NULL)
            if (lightest_e == NULL || current_e->weight < lightest_e->weight)
                lightest_e = current_e;
        current_e = current_e->next;
    }
    CopyVertex(Gt, G, lightest_e->vertex2);
    CopyEdge(Gt, G, lightest_e->vertex1, lightest_e->vertex2);
}
