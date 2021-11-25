#include "framework.h"
#include "Two-dimensionalArray-BasicsOfProgramming-lab9.h"
#include <stdio.h>
#include <math.h>

#define MAX_LOADSTRING 100
#define e 2.71828f
HINSTANCE hInst;   
WCHAR szTitle[MAX_LOADSTRING];      
WCHAR szWindowClass[MAX_LOADSTRING];           

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TWODIMENSIONALARRAYBASICSOFPROGRAMMINGLAB9, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TWODIMENSIONALARRAYBASICSOFPROGRAMMINGLAB9));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TWODIMENSIONALARRAYBASICSOFPROGRAMMINGLAB9));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TWODIMENSIONALARRAYBASICSOFPROGRAMMINGLAB9);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int columnsCount = -1;
int rowsCount = -1;
int** table;

int columnsCount2 = -1;
int rowsCount2 = -1;
int** table2;



int** GetTwoDimensionalArray(const char* filePath, int* columnsCount, int* rowsCount)
{
    int** localTable;
    FILE* filePointer;
    fopen_s(&filePointer, filePath, "r");
    
    fscanf_s(filePointer, "%d", columnsCount);
    fscanf_s(filePointer, "%d", rowsCount);

    localTable = new int* [(*columnsCount)];
    for (int i = 0; i < (*columnsCount); i++)
        localTable[i] = new int[(*rowsCount)];

    for(int i = 0; i < *rowsCount; i++)
        for (int j = 0; j < (*columnsCount); j++)
            fscanf_s(filePointer, "%d", &(localTable[j][i]));

    return localTable;
}

float Bell(float x, float offset, float height, float width)
{
    float point = (((1 / (1 + pow(e, -width * (x - offset)))) * (1 / (1 + pow(e, width * (x - offset)))))) * height/0.25f;
    return (((1 / (1 + pow(e, -width * (x - offset)))) * (1 / (1 + pow(e, width * (x - offset)))))) * height / 0.25f;
}

float OneSidedLine(float x)
{
    return (fabs(x) + x) / 2;
}

float Polybell(float x, float offset, float height, float k, float a, float b)
{
    return -OneSidedLine((x - offset - a) * k) - OneSidedLine(-(x - offset - b) * k) + height;
}

COLORREF Rainbow(float x)
{
    int point = Bell(x, 0, 1, 5) * 255;
    float a = 0.1666666666f;
    float test = Polybell(x, a + a * 2, 1, 6, a, -a);

    int r = Polybell(x, a, 1, 6, a, -a) * 255;
    int g = Polybell(x, a + a * 2, 1, 6, a, -a) * 255;
    int b = Polybell(x, a + 4 * a, 1, 6, a, -a) * 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
        
    return RGB(r, g, b);
}

void VisualizeTwoDimensionalArray(HDC hdc, int** array, int xElementsCount, int yElementsCount, int left, int top, int right, int bottom)
{
    int rectangleWidth = (right - left) / (float)xElementsCount;
    int rectangleHeight = (bottom - top) / (float)yElementsCount;
    for (int y = 0; y < yElementsCount; y++)
    {
        for (int x = 0; x < xElementsCount; x++)
        {
            HBRUSH brush = CreateSolidBrush(Rainbow((array[x][y] / 10.0f)));
            SelectObject(hdc, brush);
            Rectangle(hdc, left + rectangleWidth * x, top + rectangleHeight * y, left + rectangleWidth * (1 + x), top + rectangleHeight * (1 + y));
            
        }
    }
}

void DeleteMinSeq(int*** array, int* columnsCount, int* rowsCount)
{

    int min = (*array)[0][0];
    int minColumn = 0;
    int minRow = 0;
    for (int x = 0; x < *columnsCount; x++)
        for (int y = 0; y < *rowsCount; y++)
            if (min > (*array)[x][y])
            {
                min = (*array)[x][y];
                minColumn = x;
                minRow = y;
            }



    int newColumnsCount = *columnsCount - 1;
    int newRowsCount = *rowsCount - 1;

    int** tempArray = new int* [(*columnsCount)];
    for (int i = 0; i < (*columnsCount); i++)
        tempArray[i] = new int[newRowsCount];


    int xID = 0;
    int yID = 0;
    for (int y = 0; y < *rowsCount; y++)
    {
        xID = 0;
        for (int x = 0; x < *columnsCount; x++)
        {
            tempArray[xID][yID] = (*array)[x][y];
            if (x != minColumn)
                xID++;
        }
        if (y != minRow)
            yID++;
    }

    for (int i = 0; i < *columnsCount; i++)
    {
        delete[(*rowsCount)](*array)[i];
    }
    delete[(*columnsCount)](*array);

    *array = tempArray;
    *columnsCount = newColumnsCount;
    *rowsCount = newRowsCount;
}

void RewriteMinSeq(int*** array, int* columnsCount, int* rowsCount)
{

    int min = (*array)[0][0];
    int minColumn = 0;
    int minRow = 0;
    for (int x = 0; x < *columnsCount; x++)
        for (int y = 0; y < *rowsCount; y++)
            if (min > (*array)[x][y])
            {
                min = (*array)[x][y];
                minColumn = x;
                minRow = y;
            }

    int xID = 0;
    int yID = 0;

    int newColumnsCount = *columnsCount - 1;
    int newRowsCount = *rowsCount - 1;

    for (int y = 0; y < *rowsCount; y++)
    {
        xID = 0;

        for (int x = 0; x < *columnsCount; x++)
        {
            (*array)[xID][yID] = (*array)[x][y];
            if (x != minColumn)
                xID++;
        }
        if (y != minRow)
            yID++;
    }

    *columnsCount = newColumnsCount;
    *rowsCount = newRowsCount;
}

//rainbow from left top to right bottom
/*
5 5
1 2 3 4 5
2 3 4 5 6
3 4 5 6 7
4 5 6 7 8
5 6 7 8 9
*/
/*
10 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
1 2 3 4 5 6 7 8 9 10
*/


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        //table = new int* [columnsCount];
        //for (int i = 0; i < columnsCount; i++)
        //    table[i] = new int[rowsCount];

        table = GetTwoDimensionalArray("data.txt", &columnsCount, &rowsCount);
        table2 = GetTwoDimensionalArray("data.txt", &columnsCount2, &rowsCount2);

        RewriteMinSeq(&table2, &columnsCount2, &rowsCount2);

    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            VisualizeTwoDimensionalArray(hdc, table, columnsCount, rowsCount, 100, 100, 500, 500);
            VisualizeTwoDimensionalArray(hdc, table2, columnsCount2, rowsCount2, 100 + 500, 100, 500 + 500, 500);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
