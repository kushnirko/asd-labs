#include "DrawingDataSetter.h"

void DrawEdgeLines(draw_data data, int weight, HPEN e_pen, COLORREF num_color, HDC hdc);
void DrawLoop(int *center, HPEN e_pen, HDC hdc);
void DrawVertex(vertex* v, HBRUSH v_brush, HPEN v_pen, COLORREF text_color, HDC hdc);

void DrawGraph(graph *g,
               HPEN e_pen, COLORREF w_text_color,
               HBRUSH v_brush, HPEN v_pen,
               COLORREF v_text_color,
               HDC hdc)
{
    /* Зображаємо ребра */
    edge *current_e = g->edges->first_edge;
    int v1_index, v2_index;
    while (current_e != NULL)
    {
        v1_index = current_e->vertex1->index;
        v2_index = current_e->vertex2->index;
        if (!(v1_index > v2_index &&
              FindEdge(g, current_e->vertex2, current_e->vertex1) != NULL))
        {
            draw_data data = SetEdgeDrawData(current_e);
            switch (data.edge_type)
            {
            case 1:
                DrawEdgeLines(data, current_e->weight, e_pen, w_text_color, hdc);
                break;
            case 2:
                DrawLoop(data.center, e_pen, hdc);
            }
        }
        current_e = current_e->next;
    }
    /* Зображаємо вершини */
    vertex *current_v = g->vertices->first_vertex;
    while (current_v != NULL)
    {
        DrawVertex(current_v, v_brush, v_pen, v_text_color, hdc);
        current_v = current_v->next;
    }
}

void DrawPAStep(graph *g,
                HPEN e_pen, COLORREF w_text_color,
                HBRUSH v_brush, HPEN v_pen,
                COLORREF v_text_color,
                HDC hdc)
{
    if (!IsEdgeSetEmpty(g))
    {
        draw_data data  = SetEdgeDrawData(g->edges->last_edge);
        DrawEdgeLines(data, g->edges->last_edge->weight, e_pen, w_text_color, hdc);
        DrawVertex(g->edges->last_edge->vertex1, v_brush, v_pen, v_text_color, hdc);
    }
    DrawVertex(g->vertices->last_vertex, v_brush, v_pen, v_text_color, hdc);
}

void DrawEdgeLines(draw_data data, int weight, HPEN e_pen, COLORREF num_color, HDC hdc)
{
    SelectObject(hdc, e_pen);
    MoveToEx(hdc, data.start[x], data.start[y], NULL);
    LineTo(hdc, data.center[x], data.center[y]);
    MoveToEx(hdc, data.center[x], data.center[y], NULL);
    LineTo(hdc, data.end[x], data.end[y]);

    wchar_t output[5];
    swprintf(output, 5, L"%d", weight);
    int rect_sides_half[] = { 4, 6 };
    weight < 10 ? rect_sides_half[x] :
        weight < 100 ? (rect_sides_half[x] *= 2) :
            (rect_sides_half[x] *= 3);
    RECT rect;
    rect.left = data.text_pos[x] - rect_sides_half[x];
    rect.top = data.text_pos[y] - rect_sides_half[y];
    rect.right = data.text_pos[x] + rect_sides_half[x];
    rect.bottom = data.text_pos[y] + rect_sides_half[y];
    HBRUSH w_background = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rect, w_background);
    DeleteObject(w_background);
    HFONT hFont = CreateFont(15,
                             0, 0, 0,
                             FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                             CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                             VARIABLE_PITCH, TEXT("Times New Roman"));
    SelectObject(hdc, hFont);
    SetTextColor(hdc, num_color);
    DrawText(hdc, output, -1, &rect,
             DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    DeleteObject(hFont);
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

void DrawVertex(vertex *v, HBRUSH v_brush, HPEN v_pen, COLORREF text_color, HDC hdc)
{
    RECT rect;
    rect.left = v->coords[x] - VERTEX_RADIUS;
    rect.top = v->coords[y] - VERTEX_RADIUS;
    rect.right = v->coords[x] + VERTEX_RADIUS;
    rect.bottom = v->coords[y] + VERTEX_RADIUS;
    SelectObject(hdc, v_brush);
    SelectObject(hdc, v_pen);
    Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
    HFONT hFont = CreateFont(40,0, 0, 0,
                             FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                             CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                             VARIABLE_PITCH, TEXT("Segoe Script"));
    SelectObject(hdc, hFont);
    SetTextColor(hdc, text_color);
    DrawText(hdc, v->name, -1, &rect,
             DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    DeleteObject(hFont);
}
