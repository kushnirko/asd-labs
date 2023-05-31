#include "DrawingDataSetter.h"

void DrawGraph(int n,
               int **graph_matrix,
               int **coords,
               HPEN e_pen,
               HBRUSH v_brush,
               HPEN v_pen,
               HDC hdc)
{
    /* Зображаємо ребра */
    int i, j;
    int is_directed = CheckGraphType(n, graph_matrix);
    int drawn_lines[N][N] = { 0 };
    SelectObject(hdc, e_pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (graph_matrix[i][j] == 1)
            {
                draw_data data = SetEdgeDrawData(i, j, is_directed, coords, drawn_lines);
                switch (data.edge_type)
                {
                case 0:
                    break;
                case 1:
                    MoveToEx(hdc, data.start[x], data.start[y], NULL);
                    LineTo(hdc, data.center[x], data.center[y]);
                    MoveToEx(hdc, data.center[x], data.center[y], NULL);
                    LineTo(hdc, data.end[x], data.end[y]);
                    break;
                case 2:
                    Ellipse(hdc,
                            (data.center[x] - LOOP_RADIUS),
                            (data.center[y] - LOOP_RADIUS),
                            (data.center[x] + LOOP_RADIUS),
                            (data.center[y] + LOOP_RADIUS));
                    break;
                }
                if (is_directed)
                {
                    /* Якщо граф напрямлений, малюємо стрілку */
                    double fi = PI - data.angle;
                    int leftLineEnd[2], rightLineEnd[2];
                    rightLineEnd[x] = data.arrow_end[x] + (int) (30 * cos(fi + 0.3));
                    rightLineEnd[y] = data.arrow_end[y] + (int) (30 * sin(fi + 0.3));
                    leftLineEnd[x] = data.arrow_end[x] + (int) (30 * cos(fi - 0.3));
                    leftLineEnd[y] = data.arrow_end[y] + (int) (30 * sin(fi - 0.3));
                    MoveToEx(hdc, leftLineEnd[x], leftLineEnd[y], NULL);
                    LineTo(hdc, data.arrow_end[x], data.arrow_end[y]);
                    LineTo(hdc, rightLineEnd[x], rightLineEnd[y]);
                }
            }
        }
    }
    /* Зображаємо вершини */
    int left, top, right, bottom;
    int print_pos[2];
    for (i = 0; i < n; i++)
    {
        left =      (coords[i][x] - VERTEX_RADIUS);
        top =       (coords[i][y] - VERTEX_RADIUS);
        right =     (coords[i][x] + VERTEX_RADIUS);
        bottom =    (coords[i][y] + VERTEX_RADIUS);
        if (i > 8)
            print_pos[x] = coords[i][x] - (int)(1.5 * vertex_print_offset[x]);
            /* Ці елементи складаються з двох цифр, тому зміщення має бути більшим */
        else
            print_pos[x] = coords[i][x] - vertex_print_offset[x];
        print_pos[y] = coords[i][y] - vertex_print_offset[y];
        SelectObject(hdc, v_brush);
        Ellipse(hdc, left, top, right, bottom);
        SelectObject(hdc, v_pen);
        Ellipse(hdc, left, top, right, bottom);
        TextOut(hdc, print_pos[x], print_pos[y], vertices_names[i], 2);
    }
}
