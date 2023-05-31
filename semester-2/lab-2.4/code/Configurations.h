/*
 * Список скорочень, застосованих для найменування деяких ідентифікаторів
 * d або D => directed   (напрямлений)
 * u або U => undirected (ненапрямлений)
 * m або M => modified   (для позначення модифікованої матриці)
 * l або L => leaf       (для висячих вершин)
 * i або I => isolated   (для ізольованих вершин)
 */

/* N1N2 - номер групи, N3N4 - порядковий номер у списку групи */
#define N1  2
#define N2  2
#define N3  1
#define N4  3
/* Кількість рядків і стовпців матриць суміжності графів */
#define N (10 + N3)
/* Для позначення осей координат (позиції елементів записані у векторах) */
#define x   0
#define y   1
/* Значення, що використовуються в обчисленнях */
#define PI      3.1415926536
#define SQRT_2  1.4142135624

#define VERTEX_RADIUS           40
#define LOOP_RADIUS             (5 * VERTEX_RADIUS / 4)
#define ONE_STEP_LENGTH         (9 * VERTEX_RADIUS / 2) /* Найменша відстань між вершинами графа */
#define MAX_ONE_STEP_LENGTH     (3 * ONE_STEP_LENGTH / 2)
#define WINDOW_BORDER_OFFSET    (2 * LOOP_RADIUS + 10)

const wchar_t *vertices_names[] =
{
    L"1", L"2", L"3",
    L"4", L"5", L"6",
    L"7", L"8", L"9",
    L"10", L"11",
};

#define GRAPH_WIDTH     ((int) ((double) (N - 5) * 0.5) * ONE_STEP_LENGTH)
#define GRAPH_HEIGHT    (3 * ONE_STEP_LENGTH)
/* Щоб граф коректно відобразився у вікні, його висота має бути сталою */

const int min_coords[] =
{
    WINDOW_BORDER_OFFSET,
    WINDOW_BORDER_OFFSET
};
const int max_coords[] =
{
    WINDOW_BORDER_OFFSET + GRAPH_WIDTH,
    WINDOW_BORDER_OFFSET + GRAPH_HEIGHT
};

const int vertex_print_offset[] = { 5, 8 };

/* Визначають зміщення центру ребер для огинання вершин або вже намальованих ребер */
const double edge_center_offset_dividers[] = { 1, 3.6, 4.5, 6.5, 7 };
