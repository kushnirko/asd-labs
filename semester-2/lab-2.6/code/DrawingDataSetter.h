#include <math.h>
#include "Configurations.h"
#include "WorkWithMatrices.h"
#include "WorkWithGraphList.h"

double Pow2(int value);

typedef struct DrawingData
{
    int edge_type;
    int start[2];
    int center[2];
    int end[2];
    int text_pos[2];
} draw_data;

int **SetVerticesCoords(int n)
{
    int **coords = Create2dIntArr(n, n);
    int current_pos[2] = { min_coords[x], min_coords[y] };
    for (int i = 0; i < n; i++)
    {
        coords[i][x] = current_pos[x];
        coords[i][y] = current_pos[y];
        if (current_pos[x] < max_coords[x] && current_pos[y] == min_coords[y])
            current_pos[x] += ONE_STEP_LENGTH;
        else if (current_pos[y] < max_coords[y] && current_pos[x] == max_coords[x])
            current_pos[y] += ONE_STEP_LENGTH;
        else if (current_pos[x] > min_coords[x] && current_pos[y] == max_coords[y])
            current_pos[x] -= ONE_STEP_LENGTH;
        else if (current_pos[y] > min_coords[y] && current_pos[x] == min_coords[x])
            current_pos[y] -= GRAPH_HEIGHT / (3 - n % 2);
        /*
         * Якщо к-сть вершин парна та більша ніж 10, то з
         * лівого боку буде розміщено дві вершини,
         * а якщо непарна - 1
         */
    }
    return coords;
}

draw_data SetEdgeDrawData(edge *e)
{
    draw_data data;
    data.edge_type = 1;
    data.start[x] = e->vertex1->coords[x];
    data.start[y] = e->vertex1->coords[y];
    data.end[x] = e->vertex2->coords[x];
    data.end[y] = e->vertex2->coords[y];
    data.center[x] = (data.start[x] + data.end[x]) / 2;
    data.center[y] = (data.start[y] + data.end[y]) / 2;
    int dx = data.end[x] - data.start[x];
    int dy = data.end[y] - data.start[y];
    int variable_delta, static_coord;
    if (e->vertex1 == e->vertex2)
    {
        data.edge_type = 2;
        int loop_offset_direction[2] = { 0 };
        if (data.center[x] > min_coords[x] && data.center[y] == min_coords[y])
        {
            --loop_offset_direction[x];
            --loop_offset_direction[y];
        }
        else if (data.center[x] == max_coords[x] && data.center[y] > min_coords[y])
        {
            ++loop_offset_direction[x];
            --loop_offset_direction[y];
        }
        else if (data.center[x] < max_coords[x] && data.center[y] == max_coords[y])
        {
            ++loop_offset_direction[x];
            ++loop_offset_direction[y];
        }
        else if (data.center[x] == min_coords[x] && data.center[y] < max_coords[y])
        {
            --loop_offset_direction[x];
            ++loop_offset_direction[y];
        }
        int loop_center_offset = (int) round(
                (VERTEX_RADIUS * SQRT_2 / 2 +
                 sqrt(Pow2(LOOP_RADIUS) - Pow2(VERTEX_RADIUS) / 2))
                / SQRT_2);
        data.center[x] += loop_center_offset * loop_offset_direction[x];
        data.center[y] += loop_center_offset * loop_offset_direction[y];
        return data;
    }
    else if ((dx == 0 ? (variable_delta = dy, static_coord = x, 1) : 0) ||
             (dy == 0 ? (variable_delta = dx, static_coord = y, 1) : 0))
    {
        int abs_variable_data = abs(variable_delta);
        if (abs_variable_data > MAX_ONE_STEP_LENGTH)
        {
            int edge_center_offset = (int)
            (abs_variable_data / edge_center_offset_divs[abs_variable_data / ONE_STEP_LENGTH - 2]);
            /*
             * У масиві індекси починаються з 0, а мінімальна частка,
             * за якої слід надавати зміщення ребру - 2
             */
            if (data.start[static_coord] == min_coords[static_coord])
                data.center[static_coord] -= edge_center_offset;
            else if (data.start[static_coord] == max_coords[static_coord])
                data.center[static_coord] += edge_center_offset;
        }
        data.text_pos[x] = data.center[x];
        data.text_pos[y] = data.center[y];
    }
    else
    {
        int dx_text_offset = (int) ((data.end[x] - data.center[x]) / weight_text_offset_div);
        int dy_text_offset = (int) ((data.end[y] - data.center[y]) / weight_text_offset_div);
        if (e->vertex1->index < e->vertex2->index)
        {
            data.text_pos[x] = data.center[x] - dx_text_offset;
            data.text_pos[y] = data.center[y] - dy_text_offset;
        }
        else
        {
            data.text_pos[x] = data.center[x] + dx_text_offset;
            data.text_pos[y] = data.center[y] + dy_text_offset;
        }
    }
    return data;
}

double Pow2(int value)
{
    return (double)(value * value);
}
