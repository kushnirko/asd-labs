#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "DrawingFunctions.h"

#define DRAW_OTHER_GRAPH 0

int **A;
int **undirected_A;
int **current_matrix;
int **vertices_coords;

HWND hButton;
HWND hLabel1;
HWND hLabel2;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void CreateButton(HWND);
void CreateTips(HWND);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASS wndClass;
    wndClass.lpszClassName = L"Лабораторна робота 2.3";
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
    hWnd = CreateWindow(L"Лабораторна робота 2.3",
                        L"Лабораторна робота 2.3, виконав М.М.Кушнір",
                        WS_OVERLAPPEDWINDOW,
                        10, 10, (graph_full_size[x] + 340), graph_full_size[y],
                        (HWND)NULL,
                        (HMENU)NULL,
                        (HINSTANCE)hInstance,
                        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    int GetMessageRes;
    while ((GetMessageRes = GetMessage(&lpMsg, hWnd, 0, 0)) != 0) {
        if (GetMessageRes == -1) return lpMsg.wParam;
        else {
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
        if (wParam == DRAW_OTHER_GRAPH)
        {
            if (current_matrix == undirected_A)
                current_matrix = A;
            else if (current_matrix == A)
                current_matrix = undirected_A;
            RedrawWindow(hWnd, NULL, NULL,
                         RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
            /*
             * Видалення старих та створення
             * нових контролерів зі
             * зміненими відповідно до
             * поточного стану програми
             * текстовими назвами
             */
            DestroyWindow(hButton);
            DestroyWindow(hLabel1);
            DestroyWindow(hLabel2);
            CreateButton(hWnd);
            CreateTips(hWnd);
            break;
        }
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        HPEN ePen = CreatePen(PS_SOLID, 1, RGB(0, 38, 0));
        HBRUSH vFillBrush = CreateSolidBrush(RGB(37, 255, 127));
        HPEN vOutlinePen = CreatePen(PS_SOLID, 3, RGB(3, 104, 65));
        DrawGraph(current_matrix,
                  vertices_coords,
                  ePen,
                  vFillBrush,
                  vOutlinePen,
                  hdc);
        int matrix_print_pos[2] = {(graph_full_size[x] + 55), 250};
        ShowInt2dArr(N, N, current_matrix, matrix_print_pos, hdc);
        DeleteObject(ePen);
        DeleteObject(vOutlinePen);
        DeleteObject(vFillBrush);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_CREATE:
    {
        vertices_coords = SetVerticesCoords(N);
        srand(N1 * 1000 + 100 * N2 + 10 * N3 + N4);
        double **T = randm(N, N);
        A = mulmr(N, N, T,
                  (1.0 - N3 * 0.02 - N4 * 0.005 - 0.25));
        undirected_A = SymmetrizeMatrix(N, A);
        /* Логування матриці Т до консолі для перевірки коректності роботи функції randm() */
        printf("Matrix T\n");
        PrintDouble2dArr(N, N, T);
        /* На початку роботи програми завжди буде виводитися ненапрямлений граф */
        current_matrix = undirected_A;
        FreeDouble2dArr(N, T);
        CreateButton(hWnd);
        CreateTips(hWnd);
        break;
    }
    case WM_DESTROY:
    {
        FreeInt2dArr(N, A);
        FreeInt2dArr(N, undirected_A);
        FreeInt2dArr(N, vertices_coords);
        PostQuitMessage(0);
        break;
    }
    default :
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void CreateButton(HWND hWnd)
{
    wchar_t button_directed_graph[] = L"Намалювати напрямлений граф";
    wchar_t button_undirected_graph[] = L"Намалювати ненапрямлений граф";
    wchar_t *buttonText;
    if (current_matrix == A)
        buttonText = button_undirected_graph;
    else
        buttonText = button_directed_graph;
    hButton = CreateWindowW(L"Button", buttonText,
                            WS_VISIBLE | WS_CHILD | WS_BORDER |
                            BS_FLAT | BS_MULTILINE | BS_CENTER,
                            (graph_full_size[x] + 55), 550, 210, 50,
                            hWnd,
                            (HMENU)DRAW_OTHER_GRAPH,
                            NULL, NULL);
}

void CreateTips(HWND hWnd)
{
    wchar_t label1_directed_graph[] = L"Матриця A";
    wchar_t label1_undirected_graph[] = L"Матриця undirected_A";
    wchar_t *label1_text;
    wchar_t label2_directed_graph[] = L"( напрямлений граф )";
    wchar_t label2_undirected_graph[] = L"( ненапрямлений граф )";
    wchar_t *label2_text;
    if (current_matrix == A)
    {
        label1_text = label1_directed_graph;
        label2_text = label2_directed_graph;
    } else
    {
        label1_text = label1_undirected_graph;
        label2_text = label2_undirected_graph;
    }
    hLabel1 = CreateWindowW(L"Static", label1_text,
                            WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                            (graph_full_size[x] + 60), 175, 200, 20,
                            hWnd,
                            NULL, NULL, NULL);
    hLabel2 = CreateWindowW(L"Static", label2_text,
                            WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                            (graph_full_size[x] + 60), 200, 200, 20,
                            hWnd,
                            NULL, NULL, NULL);
}
