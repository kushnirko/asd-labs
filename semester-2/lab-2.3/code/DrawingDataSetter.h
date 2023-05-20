#include <math.h>
#include "Configurations.h"
#include "WorkWithMatrices.h"

typedef struct DrawingData
{
    int edge_type;
    int start[2];
    int center[2];
    int end[2];
    double angle;
    int arrow_end[2];
} draw_data;

/* Допоміжні функції */
int CheckGraphType (int n, int **graph_matrix);
double Pow2 (int value);
double GetDistance (const int *v1_pos, const int *v2_pos);
double ConvertDegreeToRad (double degree_value);

int **SetVerticesCoords(int n)
{
    int **vertices_coords = CreateInt2dArr(n, n);
    int min_coords[2] =
    {
        WINDOW_BORDER_OFFSET,
        WINDOW_BORDER_OFFSET
    };
    int max_coords[2] =
    {
        min_coords[x] + GRAPH_WIDTH,
        min_coords[y] + GRAPH_HEIGHT
    };
    int current_pos[2] =
    {
        min_coords[x],
        min_coords[y]
    };
    for (int i = 0; i < n; i++)
    {
        vertices_coords[i][x] = current_pos[x];
        vertices_coords[i][y] = current_pos[y];
        if (current_pos[x] < max_coords[x] && current_pos[y] == min_coords[y])
            current_pos[x] += ONE_STEP_LENGTH;
        else if (current_pos[y] < max_coords[y] && current_pos[x] == max_coords[x])
            current_pos[y] += ONE_STEP_LENGTH;
        else if (current_pos[x] > min_coords[x] && current_pos[y] == max_coords[y])
            current_pos[x] -= ONE_STEP_LENGTH;
        else if (current_pos[y] > min_coords[y] && current_pos[x] == min_coords[x])
            current_pos[y] -= GRAPH_HEIGHT / (3 - n % 2);
    }
    return vertices_coords;
}

draw_data SetEdgeDrawData(int v1, int v2, int is_directed, int **coords, int drawn_lines[N][N])
{
    const int max_one_step_len = coords[N - 1][y] - coords[0][y];
    // int one_step_len = 180;
    int min_coords[2] =
    {
        WINDOW_BORDER_OFFSET,
        WINDOW_BORDER_OFFSET
    };
    int max_coords[2] =
    {
        min_coords[x] + GRAPH_WIDTH,
        min_coords[y] + GRAPH_HEIGHT
    };
    draw_data data;
    data.edge_type = 1;
    data.start[x] = coords[v1][x];
    data.start[y] = coords[v1][y];
    data.end[x] = coords[v2][x];
    data.end[y] = coords[v2][y];
    data.center[x] = (data.start[x] + data.end[x]) / 2;
    data.center[y] = (data.start[y] + data.end[y]) / 2;
    int dx = data.end[x] - data.start[x];
    int dy = data.end[y] - data.start[y];
    double distance = GetDistance(data.start, data.end);
    int center_offset[2] =
    {
        abs((int)(dy / edge_center_offset_dividers[(int)(distance / ONE_STEP_LENGTH)])),
        abs((int)(dx / edge_center_offset_dividers[(int)(distance / ONE_STEP_LENGTH)]))
    };
    int is_drawn = 0;
    if (drawn_lines[v1][v2] == 1 && drawn_lines[v2][v1] == 1)
    {
        is_drawn = 1;
        if (!is_directed)
        {
            data.edge_type = 0;
            return data;
        }
        else
        {
            data.center[x] += center_offset[x];
            data.center[y] += center_offset[y];
        }
    }
    else
        drawn_lines[v1][v2] = drawn_lines[v2][v1] = 1;
    if (v1 == v2)
    {
        data.edge_type = 2;
        int loop_offset_direction[2] = { 0 };
        int arrow_direction[2] = { 0 };
        if (data.center[x] > min_coords[x] && data.center[y] == min_coords[y])
        {
            --loop_offset_direction[x];
            --loop_offset_direction[y];
            --arrow_direction[y];
            data.angle = ConvertDegreeToRad(-87);
        }
        else if (data.center[x] == max_coords[x] && data.center[y] > min_coords[y])
        {
            ++loop_offset_direction[x];
            --loop_offset_direction[y];
            ++arrow_direction[x];
            data.angle = ConvertDegreeToRad(183);
        }
        else if (data.center[x] < max_coords[x] && data.center[y] == max_coords[y])
        {
            ++loop_offset_direction[x];
            ++loop_offset_direction[y];
            ++arrow_direction[y];
            data.angle = ConvertDegreeToRad(93);
        }
        else if (data.center[x] == min_coords[x] && data.center[y] < max_coords[y])
        {
            --loop_offset_direction[x];
            ++loop_offset_direction[y];
            --arrow_direction[x];
            data.angle = ConvertDegreeToRad(3);
        }
        int loop_center_offset = (int) round(
                (VERTEX_RADIUS * SQRT_2 / 2 +
                 sqrt(Pow2(LOOP_RADIUS) - Pow2(VERTEX_RADIUS) / 2))
                / SQRT_2);
        data.center[x] += loop_center_offset * loop_offset_direction[x];
        data.center[y] += loop_center_offset * loop_offset_direction[y];
        data.arrow_end[x] = data.end[x] + VERTEX_RADIUS * arrow_direction[x];
        data.arrow_end[y] = data.end[y] + VERTEX_RADIUS * arrow_direction[y];
        return data;
    }
    else if (dy == 0 || dx == 0)
    {
        if (abs(dx) > max_one_step_len || abs(dy) > max_one_step_len)
        {
            if (!is_drawn)
            {
                data.center[x] += center_offset[x];
                data.center[y] += center_offset[y];
            }
            else
            {
                data.center[x] -= 2 * center_offset[x];
                data.center[y] -= 2 * center_offset[y];
            }
        }
    }
    else
    {
        if (is_drawn)
        {
            int graph_center[2] =
            {
                ((max_coords[x] + min_coords[x]) / 2),
                ((max_coords[y] + min_coords[y]) / 2)
            };
            int alternative_center[2] =
            {
                (data.center[x] - 2 * center_offset[x]),
                (data.center[y] - 2 * center_offset[y])
            };
            if (GetDistance(data.center, graph_center) >
                GetDistance(alternative_center, graph_center))
            {
                data.center[x] = alternative_center[x];
                data.center[y] = alternative_center[y];
            }
        }
    }
    if (is_directed) {
        int new_dx = data.end[x] - data.center[x];
        int new_dy = data.end[y] - data.center[y];
        double hypotenuse = GetDistance(data.center, data.end);
        if (new_dx >= 0 && new_dy >= 0)
            data.angle = acos(abs(new_dx) / hypotenuse) * -1;
        else if (new_dx >= 0 && new_dy < 0)
            data.angle = acos(abs(new_dx) / hypotenuse);
        else if (new_dx < 0 && new_dy >= 0)
            data.angle = (PI - acos(abs(new_dx) / hypotenuse)) * -1;
        else if (new_dx < 0 && new_dy < 0)
            data.angle = PI - acos(abs(new_dx) / hypotenuse);
        data.arrow_end[x] = data.end[x] - (int) round((double) VERTEX_RADIUS * cos(data.angle));
        data.arrow_end[y] = data.end[y] + (int) round((double) VERTEX_RADIUS * sin(data.angle));
    }
    return data;
}

int CheckGraphType(int n, int **graph_matrix)
{
    int is_directed = 0;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (graph_matrix[i][j] != graph_matrix[j][i])
            {
                is_directed = 1;
                break;
            }
        }
    }
    return is_directed;
}

double Pow2(int value)
{
    return (double)(value * value);
}

double GetDistance(const int *v1_pos, const int *v2_pos)
{
    int a = v2_pos[x] - v1_pos[x];
    int b = v2_pos[y] - v1_pos[y];
    return sqrt(Pow2(a) + Pow2(b));
}

double ConvertDegreeToRad(double degree_value)
{
    return PI * degree_value / 180.0;
}