#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "BFS_DFS.h"
#include "GraphPainter.h"

#define NO_ALGORITHM    0
#define BFS_ALGORITHM   1
#define DFS_ALGORITHM   2
#define NEXT_STEP       3
int current_algorithm = NO_ALGORITHM;

#define TRAVERSAL_START         1
#define TRAVERSAL_CONTINUATION  2
#define TRAVERSAL_END           3
int current_traversal_state = TRAVERSAL_END;

int **vertices_coords;
int **A;
int **tree_matrix;
queue *Q;
stack *S;
int sequence[N];
int closed_vertex = -1;

HWND hNextStepButton = NULL;
HWND hGreenLabel = NULL;
HWND hBlueLabel = NULL;
HWND hRedLabel = NULL;
HWND hYellowLabel = NULL;
HWND hProcessNameLabel;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void CreateNextStepButton(HWND hWnd, int traversal_state);
void CreateColorTips(HWND hWnd);
void CreateTip(HWND hWnd, int algorithm, int traversal_state);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASS wndClass;
    wndClass.lpszClassName = L"Лабораторна робота 2.5";
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = 0;
    wndClass.lpszMenuName = 0;
    wndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    if (!RegisterClass(&wndClass)) return 0;

    HWND hWnd;
    MSG lpMsg;

    hWnd = CreateWindowExW(0, L"Лабораторна робота 2.5",
                        L"Лабораторна робота 2.5, виконав М.М.Кушнір",
                        WS_OVERLAPPEDWINDOW,
                        0, 0,
                        (graph_full_size[x] + 400),
                        (graph_full_size[y] + 50),
                        (HWND) NULL,
                        (HMENU) NULL,
                        (HINSTANCE) hInstance,
                        (HINSTANCE) NULL);

    int start_pos[] = { (graph_full_size[x] + 95), 500 };
    CreateWindowW(L"BUTTON", L"Повернутися на початок",
                  WS_VISIBLE | WS_CHILD | WS_BORDER |
                  BS_FLAT | BS_MULTILINE | BS_CENTER,
                  start_pos[x], start_pos[y], 210, 50,
                  hWnd,
                  (HMENU) NO_ALGORITHM,
                  NULL, NULL);

    CreateWindowW(L"BUTTON", L"Обійти граф у ширину",
                  WS_VISIBLE | WS_CHILD | WS_BORDER |
                  BS_FLAT | BS_MULTILINE | BS_CENTER,
                  start_pos[x], (start_pos[y] + 70), 210, 50,
                  hWnd,
                  (HMENU) BFS_ALGORITHM,
                  NULL, NULL);

    CreateWindowW(L"BUTTON", L"Обійти граф у глибину",
                  WS_VISIBLE | WS_CHILD | WS_BORDER |
                  BS_FLAT | BS_MULTILINE | BS_CENTER,
                  start_pos[x], (start_pos[y] + 140), 210, 50,
                  hWnd,
                  (HMENU) DFS_ALGORITHM,
                  NULL, NULL);

    CreateTip(hWnd, NO_ALGORITHM, TRAVERSAL_END);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    int GetMessageRes;
    while ((GetMessageRes = GetMessage(&lpMsg, hWnd, 0, 0)) != 0)
    {
        if (GetMessageRes == -1)
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
    static HPEN     graph_e_pen;
    static HBRUSH   graph_v_brush;
    static HPEN     graph_v_pen;
    static HBRUSH   active_v_brush;
    static HPEN     active_v_pen;
    static HBRUSH   visited_v_brush;
    static HPEN     visited_v_pen;
    static HBRUSH   tree_v_brush;
    static HPEN     tree_v_pen;
    static HPEN     tree_e_pen;

    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (wParam)
        {
        case NO_ALGORITHM:
        {
            current_algorithm = NO_ALGORITHM;
            current_traversal_state = TRAVERSAL_END;
            RedrawWindow(hWnd, NULL, NULL,
                         RDW_ERASE | RDW_INVALIDATE);
            if (hNextStepButton != NULL)
            {
                DestroyWindow(hNextStepButton);
                hNextStepButton = NULL;
            }
            if (hGreenLabel != NULL)
            {
                DestroyWindow(hNextStepButton);
                hNextStepButton = NULL;
                DestroyWindow(hGreenLabel);
                hGreenLabel = NULL;
                DestroyWindow(hBlueLabel);
                hBlueLabel = NULL;
                DestroyWindow(hRedLabel);
                hRedLabel = NULL;
                DestroyWindow(hYellowLabel);
                hYellowLabel = NULL;
            }
            DestroyWindow(hProcessNameLabel);
            CreateTip(hWnd, NO_ALGORITHM, TRAVERSAL_END);
            break;
        }
        case BFS_ALGORITHM:
        case DFS_ALGORITHM:
        {
            if (wParam == BFS_ALGORITHM)
            {
                current_algorithm = BFS_ALGORITHM;
                FreeQueue(Q);
                DestroyWindow(hProcessNameLabel);
                CreateTip(hWnd, BFS_ALGORITHM, TRAVERSAL_START);
            }
            else
            {
                current_algorithm = DFS_ALGORITHM;
                FreeStack(S);
                DestroyWindow(hProcessNameLabel);
                CreateTip(hWnd, DFS_ALGORITHM, TRAVERSAL_START);
            }
            current_traversal_state = TRAVERSAL_START;
            RedrawWindow(hWnd, NULL, NULL,
                         RDW_ERASE | RDW_INVALIDATE);
            if (hNextStepButton != NULL)
                DestroyWindow(hNextStepButton);
            CreateNextStepButton(hWnd, TRAVERSAL_START);
            if (hGreenLabel == NULL)
                CreateColorTips(hWnd);
            int i, j;
            for (i = 0; i < N; i++)
            {
                sequence[i] = 0;
                for (j = 0; j < N; j++)
                    tree_matrix[i][j] = 0;
            }
            system("cls");
            PrintSequence(N, sequence);
            break;
        }
        case NEXT_STEP:
        {
            switch (current_traversal_state)
            {
            case TRAVERSAL_START:
                DestroyWindow(hNextStepButton);
                CreateNextStepButton(hWnd, TRAVERSAL_CONTINUATION);
                current_traversal_state = TRAVERSAL_CONTINUATION;
            case TRAVERSAL_CONTINUATION:
            {
                int is_empty;
                if (current_algorithm == BFS_ALGORITHM)
                {
                    closed_vertex = PerformBFSStep(N, A, sequence, Q, tree_matrix);
                    is_empty = IsQueueEmpty(Q);
                }
                else
                {
                    closed_vertex = PerformDFSStep(N, A, sequence, S, tree_matrix);
                    is_empty = IsStackEmpty(S);
                }
                system("cls");
                PrintSequence(N, sequence);
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                if (CheckTraversalState(N, sequence) == TRAVERSAL_END && is_empty)
                {
                    DestroyWindow(hNextStepButton);
                    CreateNextStepButton(hWnd, TRAVERSAL_END);
                    current_traversal_state = TRAVERSAL_END;
                }
                break;
            }
            case TRAVERSAL_END:
                system("cls");
                printf("Traversal tree matrix\n\n");
                PrintBooleanMatrix(N, tree_matrix);
                DestroyWindow(hProcessNameLabel);
                if (current_algorithm == BFS_ALGORITHM)
                    CreateTip(hWnd, BFS_ALGORITHM, TRAVERSAL_END);
                else
                    CreateTip(hWnd, DFS_ALGORITHM, TRAVERSAL_END);
                DestroyWindow(hNextStepButton);
                hNextStepButton = NULL;
                DestroyWindow(hGreenLabel);
                hGreenLabel = NULL;
                DestroyWindow(hBlueLabel);
                hBlueLabel = NULL;
                DestroyWindow(hRedLabel);
                hRedLabel = NULL;
                DestroyWindow(hYellowLabel);
                hYellowLabel = NULL;
                RedrawWindow(hWnd, NULL, NULL,
                             RDW_ERASE | RDW_INVALIDATE);
            }
        }
        }
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        switch (current_algorithm)
        {
        case NO_ALGORITHM:
        {
            system("cls");
            printf("Graph adjacency matrix\n\n");
            PrintBooleanMatrix(N, A);
            DrawGraph(N, A, vertices_coords,
                      graph_e_pen, graph_v_brush, graph_v_pen, hdc);
            break;
        }
        case BFS_ALGORITHM:
        {
            switch (current_traversal_state)
            {
            case TRAVERSAL_START:
                DrawGraph(N, A, vertices_coords,
                          graph_e_pen, graph_v_brush, graph_v_pen, hdc);
                break;
            case TRAVERSAL_CONTINUATION:
                if (closed_vertex != -1)
                {
                    /* Виділяємо останню закриту вершину */
                    DrawVertex(closed_vertex,
                               vertices_coords,
                               tree_v_brush, tree_v_pen, hdc);
                    /* Позначаємо першу у черзі відвідану вершину як активну */
                    if (!IsQueueEmpty(Q))
                        DrawVertex(Q->tail->data,
                                   vertices_coords,
                                   active_v_brush, active_v_pen, hdc);
                }
                else
                    DrawBFSStep(A, vertices_coords, Q,
                             tree_e_pen, active_v_brush, active_v_pen,
                             visited_v_brush, visited_v_pen,
                             hdc);
                break;
            case TRAVERSAL_END:
                DrawGraph(N, tree_matrix, vertices_coords,
                          tree_e_pen, tree_v_brush, tree_v_pen, hdc);
                break;
            }
            break;
        }
        case DFS_ALGORITHM:
        {
            switch (current_traversal_state)
            {
            case TRAVERSAL_START:
                DrawGraph(N, A, vertices_coords,
                          graph_e_pen, graph_v_brush, graph_v_pen, hdc);
                break;
            case TRAVERSAL_CONTINUATION:
                if (closed_vertex != -1)
                {
                    /* Виділяємо останню закриту вершину */
                    DrawVertex(closed_vertex,
                               vertices_coords,
                               tree_v_brush, tree_v_pen, hdc);
                    /* Позначаємо першу у стеку відвідану вершину як активну */
                    if (!IsStackEmpty(S))
                        DrawVertex(S->head->data,
                                   vertices_coords,
                                   active_v_brush, active_v_pen, hdc);
                }
                else
                    DrawDFSStep(A, vertices_coords, S,
                                tree_e_pen,
                                active_v_brush, active_v_pen,
                                visited_v_brush, visited_v_pen,
                                hdc);
                break;
            case TRAVERSAL_END:
                DrawGraph(N, tree_matrix, vertices_coords,
                          tree_e_pen, tree_v_brush, tree_v_pen, hdc);
                break;
            }
            break;
        }
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_ERASEBKGND:
    {
        if (current_traversal_state == TRAVERSAL_CONTINUATION)
            return 1;
        else
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    case WM_CREATE:
    {
        graph_e_pen =       CreatePen(PS_SOLID, 1, RGB(0, 38, 0));
        graph_v_brush =     CreateSolidBrush(RGB(37, 255, 127));
        graph_v_pen =       CreatePen(PS_SOLID, 3, RGB(3, 104, 65));
        active_v_brush =    CreateSolidBrush(RGB(233, 210, 39));
        active_v_pen =      CreatePen(PS_SOLID, 3, RGB(228, 114, 0));
        visited_v_brush =   CreateSolidBrush(RGB(95, 141, 225));
        visited_v_pen =     CreatePen(PS_SOLID, 3, RGB(7, 0, 186));
        tree_e_pen =        CreatePen(PS_SOLID, 2, RGB(242, 0, 0));
        tree_v_brush =      CreateSolidBrush(RGB(233, 99, 98));
        tree_v_pen =        CreatePen(PS_SOLID, 3, RGB(143, 1, 24));

        vertices_coords = SetVerticesCoords(N);
        srand(N1 * 1000 + N2 * 100 + N3 * 10 + N4);
        double **T = randm(N, N);
        A = mulmr(N, N, T, (1.0 - N3 * 0.01 - N4 * 0.005 - 0.15));
        FreeDouble2dArr(N, T);
        tree_matrix = Create2dIntArr(N, N);
        Q = InitQueue();
        S = InitStack();
        break;
    }
    case WM_DESTROY:
    {
        DeleteObject(graph_e_pen);
        DeleteObject(graph_v_pen);
        DeleteObject(graph_v_brush);
        DeleteObject(active_v_brush);
        DeleteObject(active_v_pen);
        DeleteObject(visited_v_brush);
        DeleteObject(visited_v_pen);
        DeleteObject(tree_e_pen);
        DeleteObject(tree_v_brush);
        DeleteObject(tree_v_pen);

        FreeInt2dArr(N, vertices_coords);
        FreeInt2dArr(N, A);
        FreeInt2dArr(N, tree_matrix);
        free(Q);
        free(S);
        PostQuitMessage(0);
        break;
    }
    default :
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void CreateNextStepButton(HWND hWnd, int traversal_state)
{
    wchar_t *start_traversal = L"Почати обхід";
    wchar_t *next_step = L"Наступний крок";
    wchar_t *draw_tree = L"Намалювати дерево обходу";
    wchar_t *button_text;
    switch (traversal_state)
    {
    case TRAVERSAL_START:
        button_text = start_traversal;
        break;
    case TRAVERSAL_CONTINUATION:
        button_text = next_step;
        break;
    case TRAVERSAL_END:
        button_text = draw_tree;
    }
    hNextStepButton = CreateWindowW(L"BUTTON", button_text,
                               WS_VISIBLE | WS_CHILD | WS_BORDER |
                               BS_FLAT | BS_MULTILINE | BS_CENTER,
                               (graph_full_size[x] + 75), 400, 250, 60,
                               hWnd,
                               (HMENU) NEXT_STEP,
                               NULL, NULL);
}

void CreateColorTips(HWND hWnd)
{
    int start_pos[] = {  (graph_full_size[x] + 75), 100 };
    hGreenLabel = CreateWindowW(L"STATIC", L"     Зелений - нові вершини",
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                start_pos[x], start_pos[y], 250, 20,
                                hWnd,
                                NULL, NULL, NULL);
    hBlueLabel = CreateWindowW(L"STATIC", L"     Синій - відвідані вершини",
                               WS_VISIBLE | WS_CHILD | WS_BORDER,
                               start_pos[x], (start_pos[y] + 30), 250, 20,
                               hWnd,
                               NULL, NULL, NULL);
    hRedLabel = CreateWindowW(L"STATIC", L"     Червоний - закриті вершини",
                              WS_VISIBLE | WS_CHILD | WS_BORDER,
                              start_pos[x], (start_pos[y] + 60), 250, 20,
                              hWnd,
                              NULL, NULL, NULL);
    hYellowLabel = CreateWindowW(L"STATIC", L"     Жовтий - активна вершина",
                                 WS_VISIBLE | WS_CHILD | WS_BORDER,
                                 start_pos[x], (start_pos[y] + 90), 250, 20,
                                 hWnd,
                                 NULL, NULL, NULL);
}

void CreateTip(HWND hWnd, int algorithm, int traversal_state)
{
    wchar_t *no_algorithm = L"Граф, заданий за варінтом";
    wchar_t *bfs_traversal = L"Обхід графа в ширину";
    wchar_t *dfs_traversal = L"Обхід графа в глибину";
    wchar_t *bfs_tree = L"Дерево обходу в ширину";
    wchar_t *dfs_tree = L"Дерево обходу в глибину";
    wchar_t *tip_text;
    switch (algorithm)
    {
    case NO_ALGORITHM:
        tip_text = no_algorithm;
        break;
    case BFS_ALGORITHM:
        if (traversal_state == TRAVERSAL_END)
            tip_text = bfs_tree;
        else
            tip_text = bfs_traversal;
        break;
    case DFS_ALGORITHM:
        if (traversal_state == TRAVERSAL_END)
            tip_text = dfs_tree;
        else
            tip_text = dfs_traversal;
    }
    hProcessNameLabel = CreateWindowW(L"STATIC", tip_text,
                                      WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                                      (graph_full_size[x] + 75), 290, 250, 20,
                                      hWnd,
                                      NULL, NULL, NULL);
}
