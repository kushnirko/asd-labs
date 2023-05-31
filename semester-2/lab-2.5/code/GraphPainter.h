#include "DrawingDataSetter.h"

void DrawEdgeLines(int *start, int *center, int *end, HPEN e_pen, HDC hdc);
void DrawLoop(int *center, HPEN e_pen, HDC hdc);
void DrawArrow(double angle, int *arrow_end, HPEN e_pen, HDC hdc);
void DrawVertex(int vertex, int **coords, HBRUSH v_brush, HPEN v_pen, HDC hdc);

void DrawGraph(int n,
               int **graph_matrix,
               int **coords,
               HPEN e_pen,
               HBRUSH v_brush,
               HPEN v_pen,
               HDC hdc)
{
    /* Зображаємо ребра */
    int drawn_lines[N][N] = { 0 };
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (graph_matrix[i][j] == 1)
            {
                draw_data data = SetEdgeDrawData(i, j, coords, drawn_lines);
                switch (data.edge_type)
                {
                case 1:
                    DrawEdgeLines(data.start, data.center, data.end, e_pen, hdc);
                    break;
                case 2:
                    DrawLoop(data.center, e_pen, hdc);
                    break;
                }
                DrawArrow(data.angle, data.arrow_end, e_pen, hdc);
            }
        }
    }
    /* Зображаємо вершини */
    for (i = 0; i < n; i++)
        DrawVertex(i, coords, v_brush, v_pen, hdc);
}

void DrawBFSStep(int **graph_matrix, int **coords, queue *Q,
                 HPEN tree_e_pen,
                 HBRUSH active_v_brush, HPEN active_v_pen,
                 HBRUSH visited_v_brush, HPEN visited_v_pen,
                 HDC hdc)
{
    int active_vertex = Q->tail->data;
    int visited_vertex = Q->head->data;
    int drawn_lines[N][N] = { 0 };
    if (active_vertex > visited_vertex)
        if (graph_matrix[visited_vertex][active_vertex])
            drawn_lines[visited_vertex][active_vertex] =
            drawn_lines[active_vertex][visited_vertex] = 1;
    draw_data data = SetEdgeDrawData(active_vertex, visited_vertex,
                                     coords, drawn_lines);
    if (active_vertex == visited_vertex)
    {
        /* Якщо вершина належить до нової компоненти, зображаємо лише її */
        DrawVertex(active_vertex,
                   coords,
                   active_v_brush, active_v_pen, hdc);
    }
    else
    {
        /* Виділяємо ребро між активною та відвіданою вершиною */
        DrawEdgeLines(data.start, data.center, data.end, tree_e_pen, hdc);
        DrawArrow(data.angle, data.arrow_end, tree_e_pen, hdc);
        /* Виділяємо активну вершину */
        DrawVertex(active_vertex,
                   coords,
                   active_v_brush, active_v_pen, hdc);
        /* Виділяємо щойно відвідану вершину */
        DrawVertex(visited_vertex,
                   coords,
                   visited_v_brush, visited_v_pen, hdc);
    }
}

void DrawDFSStep(int **graph_matrix, int **coords, stack *S,
                 HPEN tree_e_pen,
                 HBRUSH active_v_brush, HPEN active_v_pen,
                 HBRUSH visited_v_brush, HPEN visited_v_pen,
                 HDC hdc)
{
    int active_vertex = S->head->data;
    if (S->head->next == NULL)
    {
        /* Якщо вершина належить до нової компоненти, зображаємо лише її */
        DrawVertex(active_vertex,
                   coords,
                   active_v_brush, active_v_pen, hdc);
    }
    else
    {
        int previous_active_vertex = S->head->next->data;
        int drawn_lines[N][N] = { 0 };
        if (previous_active_vertex > active_vertex)
            if (graph_matrix[active_vertex][previous_active_vertex])
                drawn_lines[active_vertex][previous_active_vertex] =
                drawn_lines[previous_active_vertex][active_vertex] = 1;
        draw_data data = SetEdgeDrawData(previous_active_vertex, active_vertex,
                                         coords, drawn_lines);
        /* Виділяємо ребро між попередньою та поточною активними вершинами */
        DrawEdgeLines(data.start, data.center, data.end, tree_e_pen, hdc);
        DrawArrow(data.angle, data.arrow_end, tree_e_pen, hdc);
        /* Повертаємо попередній активній вершині статус відвіданої */
        DrawVertex(previous_active_vertex,
                   coords,
                   visited_v_brush, visited_v_pen, hdc);
        /* Виділяємо поточну активну вершину */
        DrawVertex(active_vertex,
                   coords,
                   active_v_brush, active_v_pen, hdc);
    }
}

void DrawEdgeLines(int *start, int *center, int *end, HPEN e_pen, HDC hdc)
{
    SelectObject(hdc, e_pen);
    MoveToEx(hdc, start[x], start[y], NULL);
    LineTo(hdc, center[x], center[y]);
    MoveToEx(hdc, center[x], center[y], NULL);
    LineTo(hdc, end[x], end[y]);
}

void DrawLoop(int *center, HPEN e_pen, HDC hdc)
{
    SelectObject(hdc, e_pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc,
            (center[x] - LOOP_RADIUS),
            (center[y] - LOOP_RADIUS),
            (center[x] + LOOP_RADIUS),
            (center[y] + LOOP_RADIUS));
}

void DrawArrow(double angle, int *arrow_end, HPEN e_pen, HDC hdc)
{
    SelectObject(hdc, e_pen);
    double fi = PI - angle;
    int leftLineEnd[2], rightLineEnd[2];
    rightLineEnd[x] = arrow_end[x] + (int) (30 * cos(fi + 0.3));
    rightLineEnd[y] = arrow_end[y] + (int) (30 * sin(fi + 0.3));
    leftLineEnd[x] = arrow_end[x] + (int) (30 * cos(fi - 0.3));
    leftLineEnd[y] = arrow_end[y] + (int) (30 * sin(fi - 0.3));
    MoveToEx(hdc, leftLineEnd[x], leftLineEnd[y], NULL);
    LineTo(hdc, arrow_end[x], arrow_end[y]);
    LineTo(hdc, rightLineEnd[x], rightLineEnd[y]);
}

void DrawVertex(int vertex, int **coords, HBRUSH v_brush, HPEN v_pen, HDC hdc)
{
    int left =      (coords[vertex][x] - VERTEX_RADIUS);
    int top =       (coords[vertex][y] - VERTEX_RADIUS);
    int right =     (coords[vertex][x] + VERTEX_RADIUS);
    int bottom =    (coords[vertex][y] + VERTEX_RADIUS);
    int print_pos[2];
    if (vertex > 8)
        print_pos[x] = coords[vertex][x] - (int)(1.5 * vertex_print_offset[x]);
        /* Ці елементи складаються з двох цифр, тому зміщення має бути більшим */
    else
        print_pos[x] = coords[vertex][x] - vertex_print_offset[x];
    print_pos[y] = coords[vertex][y] - vertex_print_offset[y];
    SelectObject(hdc, v_brush);
    SelectObject(hdc, v_pen);
    Ellipse(hdc, left, top, right, bottom);
    TextOut(hdc, print_pos[x], print_pos[y], vertices_names[vertex], 2);
}
