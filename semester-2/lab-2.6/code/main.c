#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "GraphPainter.h"
#include "Prim'sAlgorithm.h"

#define GO_TO_START 0
#define NEXT_STEP   1

#define TRAVERSAL_START         1
#define TRAVERSAL_CONTINUATION  2
#define TRAVERSAL_END           3
int current_traversal_state = TRAVERSAL_START;

graph *G;
graph *Gt;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void CreateGoToStartButton(HWND hWnd);

void CreateNextStepButton(HWND hWnd, int traversal_state);
HWND hNextStepButton;

void CreateTip(HWND hWnd, int traversal_state);
HWND hTip;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASS wndClass;
    wndClass.lpszClassName = L"Лабораторна робота 2.6";
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
    hWnd = CreateWindowExW(0, L"Лабораторна робота 2.6",
                           L"Лабораторна робота 2.6, виконав М.М.Кушнір",
                           WS_OVERLAPPEDWINDOW,
                           0, 0,
                           (graph_full_size[x] + 400),
                           (graph_full_size[y] + 50),
                           (HWND) NULL,
                           (HMENU) NULL,
                           (HINSTANCE) hInstance,
                           (HINSTANCE) NULL);
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
    static COLORREF graph_w_text_color;
    static HBRUSH   graph_v_brush;
    static HPEN     graph_v_pen;
    static COLORREF graph_v_text_color;
    static HPEN     tree_e_pen;
    static COLORREF tree_w_text_color;
    static HBRUSH   tree_v_brush;
    static HPEN     tree_v_pen;
    static COLORREF tree_v_text_color;

    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (wParam)
        {
        case GO_TO_START:
        {
            current_traversal_state = TRAVERSAL_START;
            RedrawWindow(hWnd, NULL, NULL,
                         RDW_ERASE | RDW_INVALIDATE);
            if (hNextStepButton != NULL)
                DestroyWindow(hNextStepButton);
            CreateNextStepButton(hWnd, TRAVERSAL_START);
            DestroyWindow(hTip);
            CreateTip(hWnd, TRAVERSAL_START);
            break;
        }
        case NEXT_STEP:
        {
            switch (current_traversal_state)
            {
            case TRAVERSAL_START:
                DestroyWindow(hNextStepButton);
                CreateNextStepButton(hWnd, TRAVERSAL_CONTINUATION);
                DestroyWindow(hTip);
                CreateTip(hWnd, TRAVERSAL_CONTINUATION);
                current_traversal_state = TRAVERSAL_CONTINUATION;
                RefreshGraph(Gt);
            case TRAVERSAL_CONTINUATION:
                PerformPrimAlgStep(Gt, G);
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                if (IsVerticesSetFull(N, Gt))
                {
                    DestroyWindow(hNextStepButton);
                    CreateNextStepButton(hWnd, TRAVERSAL_END);
                    current_traversal_state = TRAVERSAL_END;
                }
                break;
            case TRAVERSAL_END:
                DestroyWindow(hTip);
                CreateTip(hWnd, TRAVERSAL_END);
                RedrawWindow(hWnd, NULL, NULL,
                             RDW_ERASE | RDW_INVALIDATE);
                DestroyWindow(hNextStepButton);
                hNextStepButton = NULL;
            }
        }
        }
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        switch (current_traversal_state)
        {
        case TRAVERSAL_START:
            system("cls");
            printf("Weighted graph [ G ]:\n\n");
            PrintGraph(G);
            DrawGraph(G,
                      graph_e_pen, graph_w_text_color,
                      graph_v_brush, graph_v_pen, graph_v_text_color,
                      hdc);
            break;
        case TRAVERSAL_CONTINUATION:
            system("cls");
            printf("Current spanning tree [ Gt ]:\n\n");
            PrintGraph(Gt);
            DrawPAStep(Gt, tree_e_pen, tree_w_text_color,
                       tree_v_brush, tree_v_pen, tree_v_text_color,
                       hdc);
            break;
        case TRAVERSAL_END:
            system("cls");
            printf("Minimum spanning tree [ Gt ] of the weighted graph [ G ]:\n\n");
            PrintGraph(Gt);
            DrawGraph(Gt, tree_e_pen, tree_w_text_color,
                      tree_v_brush, tree_v_pen, tree_v_text_color,
                      hdc);
            break;
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
        CreateGoToStartButton(hWnd);
        CreateNextStepButton(hWnd, TRAVERSAL_START);
        CreateTip(hWnd, TRAVERSAL_START);

        graph_e_pen = CreatePen(PS_SOLID, 1, RGB(0, 38, 0));
        graph_w_text_color = RGB(0, 0, 0);
        graph_v_brush = CreateSolidBrush(RGB(37, 255, 127));
        graph_v_pen = CreatePen(PS_SOLID, 3, RGB(3, 104, 65));
        graph_v_text_color = RGB(3, 104, 65);
        tree_e_pen = CreatePen(PS_SOLID, 3, RGB(242, 0, 0));
        tree_w_text_color = RGB(242, 0, 0);
        tree_v_brush = CreateSolidBrush(RGB(233, 99, 98));
        tree_v_pen = CreatePen(PS_SOLID, 3, RGB(143, 1, 24));
        tree_v_text_color = RGB(255, 255, 255);

        int **vertices_coords = SetVerticesCoords(N);

        srand(N1 * 1000 + N2 * 100 + N3 * 10 + N4);
        double **T = randm(N, N);
        int **A = mulmr(N, N, T, (1.0 - N3 * 0.01 - N4 * 0.005 - 0.05));
        int **uA = SymmetrizeMatrix(N, A);
        int **W = GetMatrixOfWeights(N, A);

        G = ConvertDataToGraphList(N, vertices_names, vertices_coords, uA, W);
        Gt = InitGraph();

        FreeDouble2dArr(N, T);
        FreeInt2dArr(N, A);
        FreeInt2dArr(N, uA);
        FreeInt2dArr(N, W);
        break;
    }
    case WM_DESTROY:
    {
        DeleteObject(graph_e_pen);
        DeleteObject(graph_v_pen);
        DeleteObject(graph_v_brush);
        DeleteObject(tree_e_pen);
        DeleteObject(tree_v_brush);
        DeleteObject(tree_v_pen);
        FreeGraph(G);
        FreeGraph(Gt);
        PostQuitMessage(0);
        break;
    }
    default :
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void CreateGoToStartButton(HWND hWnd)
{
    CreateWindowW(L"BUTTON", L"На початок",
                  WS_VISIBLE | WS_CHILD | WS_BORDER |
                  BS_FLAT | BS_MULTILINE | BS_CENTER,
                  (graph_full_size[x] + 75), 450, 250, 60,
                  hWnd,
                  (HMENU) GO_TO_START,
                  NULL, NULL);
}

void CreateNextStepButton(HWND hWnd, int traversal_state)
{
    wchar_t *start_traversal = L"Почати обхід";
    wchar_t *next_step = L"Наступний крок";
    wchar_t *draw_tree = L"Намалювати дерево кістяка";
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
                                    (graph_full_size[x] + 75), 350, 250, 60,
                                    hWnd,
                                    (HMENU) NEXT_STEP,
                                    NULL, NULL);
}

void CreateTip(HWND hWnd, int traversal_state)
{
    wchar_t *graph_text = L"Зважений ненапрямлений граф G";
    wchar_t *traversal_text = L"Пошук мінімального кістяка за алгоритмом Пріма";
    wchar_t *tree_text = L"Дерево мінімального кістяка Gt зваженого графа G";
    wchar_t *tip_text;
    int lines_quantity;
    switch (traversal_state)
    {
    case TRAVERSAL_START:
        tip_text = graph_text;
        lines_quantity = 1;
        break;
    case TRAVERSAL_CONTINUATION:
        tip_text = traversal_text;
        lines_quantity = 2;
        break;
    case TRAVERSAL_END:
        tip_text = tree_text;
        lines_quantity = 2;
    }
    hTip = CreateWindowW(L"STATIC", tip_text,
                  WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                  (graph_full_size[x] + 50), 250, 300, (18 * lines_quantity),
                  hWnd,
                  NULL, NULL, NULL);
}
