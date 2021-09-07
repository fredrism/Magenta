#include "MAGWindow.h"
#include "stdio.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
        
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

MAGWindow* MAGCreateWindow()
{
    HMODULE hInstance = GetModuleHandle(NULL);

    LPCSTR CLASS_NAME = "Magenta Window Class";
    LPCSTR WINDOW_NAME = "Magenta Window";
    WNDCLASS wc;

    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = NULL;
    wc.hCursor = NULL;
    wc.hIcon = NULL;
    wc.style = CS_OWNDC;
    wc.lpszMenuName = NULL;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        DWORD errorID = GetLastError();
        LPSTR messageBuffer = NULL;

        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
        printf(messageBuffer);
        return NULL;
    }

    MAGWindow* magWnd = malloc(sizeof(MAGWindow));
    magWnd->hwnd = hwnd;

    ShowWindow(hwnd, 1);
    return magWnd;
}

int MAGPollEvents()
{
    MSG msg;
    int close = GetMessage(&msg, NULL, 0, 0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return close;
}