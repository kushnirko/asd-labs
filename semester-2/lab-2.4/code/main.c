#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "GraphPainter.h"

#define TASK_1_DIRECTED_GRAPH       1
#define TASK_1_UNDIRECTED_GRAPH     2
#define TASK_2_DIRECTED_GRAPH       3
#define TASK_2_UNDIRECTED_GRAPH     4
#define TASK_3_DIRECTED_GRAPH       5
#define TASK_3_UNDIRECTED_GRAPH     6
#define TASK_4_1                    7
#define TASK_4_2                    8
#define TASK_4_3_1                  9
#define TASK_4_3_2                  10
#define TASK_4_4                    11
#define TASK_4_5                    12
#define TASK_4_6                    13
#define TASK_4_7                    14

int current_task = 0;
int **A;
int **uA;
int **mA;
int **current_matrix;
int **vertices_coords;
int **components_coords = NULL;
int **current_coords;
int current_n;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void AddMenu(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASS wndClass;
    wndClass.lpszClassName = L"Лабораторна робота 2.4";
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = 0;
    wndClass.lpszMenuName = 0;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    if (!RegisterClass(&wndClass)) return 0;
    HWND hWnd;
    MSG lpMsg;
    hWnd = CreateWindow(L"Лабораторна робота 2.4",
                        L"Лабораторна робота 2.4, виконав М.М.Кушнір",
                        WS_OVERLAPPEDWINDOW,
                        0, 0,
                        (WINDOW_BORDER_OFFSET * 2 + GRAPH_WIDTH + 40),
                        (WINDOW_BORDER_OFFSET * 2 + GRAPH_HEIGHT + 60),
                        (HWND)NULL,
                        (HMENU)NULL,
                        (HINSTANCE)hInstance,
                        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    int GetMessage_res;
    while ((GetMessage_res = GetMessage(&lpMsg, hWnd, 0, 0)) != 0)
    {
        if (GetMessage_res == -1)
            return lpMsg.wParam;
        else
        {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_COMMAND:
    {
        system("cls");
        RedrawWindow(hWnd, NULL, NULL,RDW_ERASE | RDW_INVALIDATE);
        switch (wParam)
        {
        case TASK_1_DIRECTED_GRAPH:
        {
            current_task = TASK_1_DIRECTED_GRAPH;
            current_matrix = A;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 1 (directed graph)\n\n");
            printf("Adjacency matrix of directed graph (matrix 'A'):\n\n");
            PrintBooleanMatrix(N, A);
            break;
        }
        case TASK_1_UNDIRECTED_GRAPH:
        {
            current_task = TASK_1_UNDIRECTED_GRAPH;
            current_matrix = uA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 1 (undirected graph)\n\n");
            printf("Adjacency matrix of undirected graph (matrix 'uA'):\n\n");
            PrintBooleanMatrix(N, uA);
            break;
        }
        case TASK_2_DIRECTED_GRAPH:
        {
            current_matrix = A;
            current_coords = vertices_coords;
            current_task = TASK_2_DIRECTED_GRAPH;
            current_n = N;

            printf("Task 2 (directed graph)\n\n");
            int **vertices_degs = GetVerticesDegsOfDGraph(N, A);
            int cols_lengths[] = { 5,12, 11 };
            char *headings[] = { "Outdegrees", "Indegrees" };
            PrintInt2dArr(N, 2,
                          cols_lengths, headings,
                          vertices_degs);
            printf("\n");
            PrintDGraphHomogeneityDeg(N, vertices_degs);
            FreeInt2dArr(N, vertices_degs);
            break;
        }
        case TASK_2_UNDIRECTED_GRAPH:
        {
            current_task = TASK_2_UNDIRECTED_GRAPH;
            current_matrix = uA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 2 (undirected graph)\n\n");
            int *vertices_degs = GetVerticesDegsOfUGraph(N, uA);
            int cols_lengths[] = { 5,9 };
            PrintIntArr(N, cols_lengths, "Degrees", vertices_degs);
            printf("\n");
            PrintUGraphHomogeneityDeg(N, vertices_degs);
            free(vertices_degs);
            break;
        }
        case TASK_3_DIRECTED_GRAPH:
        {
            current_task = TASK_3_DIRECTED_GRAPH;
            current_matrix = A;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 3 (directed graph)\n\n");
            int **vertices_degs = GetVerticesDegsOfDGraph(N, A);
            PrintLIVerticesOfDGraph(N, vertices_degs);
            FreeInt2dArr(N, vertices_degs);
            break;
        }
        case TASK_3_UNDIRECTED_GRAPH:
        {
            current_task = TASK_3_UNDIRECTED_GRAPH;
            current_matrix = uA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 3 (undirected graph)\n\n");
            int *vertices_degs = GetVerticesDegsOfUGraph(N, uA);
            PrintLIVerticesOfUGraph(N, vertices_degs);
            free(vertices_degs);
            break;
        }
        case TASK_4_1:
        {
            current_task = TASK_4_1;
            current_matrix = mA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 4.1\n\n");
            printf("Modified adjacency matrix of graph (matrix 'mA'):\n\n");
            PrintBooleanMatrix(N, mA);
            break;
        }
        case TASK_4_2:
        {
            current_task = TASK_4_2;
            current_matrix = mA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 4.2\n\n");
            int **vertices_degs = GetVerticesDegsOfDGraph(N, mA);
            int cols_lengths[] = { 5,12, 11 };
            /* Перший стовпчик буде використаний для нумерації рядків */
            char *headings[] = { "Outdegrees", "Indegrees" };
            PrintInt2dArr(N, 2,
                          cols_lengths, headings,
                          vertices_degs);
            FreeInt2dArr(N, vertices_degs);
            break;
        }
        case TASK_4_3_1:
        {
            current_task = TASK_4_3_1;
            current_matrix = mA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 4.3.1\n\n");
            printf("Paths of length 2:\n");
            int **mA2 =MultSquareMatrices(N, mA, mA);
            PrintPathsWithLen2(N, mA, mA2);
            FreeInt2dArr(N, mA2);
            break;
        }
        case TASK_4_3_2:
        {
            current_task = TASK_4_3_2;
            current_matrix = mA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 4.3.2\n\n");
            printf("Paths of length 3:\n");
            int **mA2 = MultSquareMatrices(N, mA, mA);
            int **mA3 = MultSquareMatrices(N,mA2,mA);
            PrintPathsWithLen3(N, mA, mA3);
            FreeInt2dArr(N, mA2);
            FreeInt2dArr(N, mA3);
            break;
        }
        case TASK_4_4:
        {
            current_task = TASK_4_4;
            current_matrix = mA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 4.4\n\n");
            printf("Reachability matrix:\n\n");
            int **R = GetReachabilityMatrix(N, mA);
            PrintBooleanMatrix(N, R);
            FreeInt2dArr(N, R);
            break;
        }
        case TASK_4_5:
        {
            current_task = TASK_4_5;
            current_matrix = mA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 4.5\n\n");
            printf("Components of strong connectivity:\n");
            int **R = GetReachabilityMatrix(N, mA);
            int **Rt = TransposeSquareMatrix(N, R);
            int **S = MultSquareMatricesElemByElem(N, R, Rt);
            int **components = GetConnectComponents(N, S);
            PrintConnectComponents(N, components);
            FreeInt2dArr(N, R);
            FreeInt2dArr(N, Rt);
            FreeInt2dArr(N, S);
            FreeInt2dArr(N, components);
            break;
        }
        case TASK_4_6:
        {
            current_task = TASK_4_6;
            current_matrix = mA;
            current_coords = vertices_coords;
            current_n = N;

            printf("Task 4.6\n\n");
            printf("Connectivity matrix:\n\n");
            int **R = GetReachabilityMatrix(N, mA);
            int **Rt = TransposeSquareMatrix(N, R);
            int **S = MultSquareMatricesElemByElem(N, R, Rt);
            PrintBooleanMatrix(N, S);
            FreeInt2dArr(N, R);
            FreeInt2dArr(N, Rt);
            FreeInt2dArr(N, S);
            break;
        }
        case TASK_4_7:
        {
            current_task = TASK_4_7;
            printf("Task 4.7\n\n");
            int **R = GetReachabilityMatrix(N, mA);
            int **Rt = TransposeSquareMatrix(N, R);
            int **S = MultSquareMatricesElemByElem(N, R, Rt);
            int **components = GetConnectComponents(N, S);
            int **graph_matrix = GetCondensationGraphMatrix(N, components, mA);
            int components_ct = 0;
            while (components_ct < N && components[components_ct][N] != 0)
                components_ct++;
            current_n = components_ct;
            printf("Adjacency matrix of the condensation graph:\n\n");
            PrintBooleanMatrix(current_n, graph_matrix);
            components_coords = SetVerticesCoords(current_n);
            current_matrix = graph_matrix;
            current_coords = components_coords;
            FreeInt2dArr(N, R);
            FreeInt2dArr(N, Rt);
            FreeInt2dArr(N, S);
            FreeInt2dArr(N, components);
            break;
        }
        }
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        HPEN ePen = CreatePen(PS_SOLID, 1, RGB(0, 38, 0));
        HPEN vOutlinePen = CreatePen(PS_SOLID, 3, RGB(3, 104, 65));
        HBRUSH vFillBrush = CreateSolidBrush(RGB(37, 255, 127));
        if (current_task)
            DrawGraph(current_n,
                      current_matrix,
                      current_coords,
                      ePen,
                      vFillBrush,
                      vOutlinePen,
                      hdc);
        if (current_n != N)
            FreeInt2dArr(current_n, components_coords);
        DeleteObject(ePen);
        DeleteObject(vOutlinePen);
        DeleteObject(vFillBrush);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_CREATE:
        AddMenu(hWnd);
        vertices_coords = SetVerticesCoords(N);
        srand(N1 * 1000 + N2 * 100 + N3 * 10 + N4);
        double **T = randm(N, N);
        A = mulmr(N, N, T,
                  (1.0 - N3 * 0.01 - N4 * 0.01 - 0.3));
        uA = SymmetrizeMatrix(N, A);
        mA = mulmr(N, N, T,
                   (1.0 - N3 * 0.005 - N4 * 0.005 - 0.27));
        printf("Matrix T: \n\n");
        PrintDouble2dArr(N, N, T);
        printf("\n*You can select an option from the window pop-up menu\n");
        FreeDouble2dArr(N, T);
        break;
    case WM_DESTROY:
        FreeInt2dArr(N, A);
        FreeInt2dArr(N, uA);
        FreeInt2dArr(N, mA);
        FreeInt2dArr(N, vertices_coords);
        PostQuitMessage(0);
        break;
    default :
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void AddMenu(HWND hwnd)
{
    HMENU hMenu = CreateMenu();
    /* Підменю для завдання 1 */
    HMENU hTask1 = CreateMenu();
    AppendMenu(hTask1, MF_STRING, TASK_1_DIRECTED_GRAPH,
               L"1) намалювати напрямлений граф");
    AppendMenu(hTask1, MF_STRING, TASK_1_UNDIRECTED_GRAPH,
               L"2) намалювати ненапрямлений граф");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTask1,
               L"Завдання 1");
    /* Підменю для завдання 2 */
    HMENU hTask2 = CreateMenu();
    AppendMenu(hTask2, MF_STRING, TASK_2_DIRECTED_GRAPH,
               L"1) визначити напівстепені вершин напрямленого графа");
    AppendMenu(hTask2, MF_STRING, TASK_2_UNDIRECTED_GRAPH,
               L"2) визначити степені вершин ненапрямленого графа");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTask2,
               L"Завдання 2");
    /* Підменю для завдання 3 */
    HMENU hTask3 = CreateMenu();
    AppendMenu(hTask3, MF_STRING, TASK_3_DIRECTED_GRAPH,
               L"1) визначити висячі та ізольовані вершини напрямленого графа");
    AppendMenu(hTask3, MF_STRING, TASK_3_UNDIRECTED_GRAPH,
               L"2) визначити висячі та ізольовані вершини ненапрямленого графа");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTask3,
               L"Завдання 3");
    /* Підменю для завдання 4 */
    HMENU hTask4 = CreateMenu();
    AppendMenu(hTask4, MF_STRING, TASK_4_1,
               L"1) обчислити змінену матрицю суміжності");
    AppendMenu(hTask4, MF_STRING, TASK_4_2,
               L"2) визначити напівстепені вузлів");
    /* Підменю для завдання 4-3) */
    HMENU hTask4_3 = CreateMenu();
    AppendMenu(hTask4_3, MF_STRING, TASK_4_3_1,
               L" 2 ");
    AppendMenu(hTask4_3, MF_STRING, TASK_4_3_2,
               L" 3 ");
    AppendMenu(hTask4, MF_POPUP, (UINT_PTR)hTask4_3,
               L"3) знайти всі шляхи довжиною ...");
    /*****************************/
    AppendMenu(hTask4, MF_STRING, TASK_4_4,
               L"4) обчислити матрицю досяжності");
    AppendMenu(hTask4, MF_STRING, TASK_4_5,
               L"5) знайти компоненти сильної зв'язності");
    AppendMenu(hTask4, MF_STRING, TASK_4_6,
               L"6) обчислити матрицю зв'язності");
    AppendMenu(hTask4, MF_STRING, TASK_4_7,
               L"7) намалювати граф конденсації");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTask4,
               L"Завдання 4");
    SetMenu(hwnd, hMenu);
}
