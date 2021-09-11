#include "WIN32_MAGWindow.h"
#include "GLRenderer.h"
#include "stdio.h"
#include "cglm/cglm.h"

int initialized = 0;
unsigned int shaderProgram;

void OnResize(unsigned int width, unsigned int height)
{
    printf("RESIZED: %d, %d \n", width, height);
    if(initialized)
    {
        mat4 mat;
        //glm_mat4_identity(mat);
        glm_ortho(0,width, height, 0, 0, 1, mat);
        unsigned int loc = glGetUniformLocation(shaderProgram, "mat");
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
        glViewport(0, 0, width, height);
    }
}

float cameraX = 0;
float cameraY = 0;
float cameraZ = 1;

void OnKeyDown(unsigned int keyCode)
{
    switch (keyCode)
    {
        case VK_LEFT:
            cameraX -= 0.01;
            break;
        case VK_RIGHT:
            cameraX += 0.01;
            break;
        case VK_UP:
            cameraY += 0.01;
            break;
        case VK_DOWN:
            cameraY -= 0.01;
            break;
        case VK_SPACE:
            cameraZ += 0.01;
            break;
        case VK_CONTROL:
            cameraZ -= 0.01;
            break;
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
        
        case WM_SIZE:
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            OnResize(width, height);

        case WM_KEYDOWN:
            OnKeyDown(wParam);

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

    //SetWindowLong(hwnd, GWL_STYLE, WS_CHILD);

    MAGWindow* magWnd = malloc(sizeof(MAGWindow));
    magWnd->hwnd = hwnd;

    ShowWindow(hwnd, 1);
    return magWnd;
}

int CreateContext(MAGWindow* wnd)
{
    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1, 
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC deviceContext = GetDC(wnd->hwnd);
    int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
    SetPixelFormat(deviceContext, pixelFormat, &pfd);

    HGLRC renderingContext = wglCreateContext(deviceContext);
    wglMakeCurrent(deviceContext, renderingContext);


    if(!gladLoadGL())
    {
        printf("Failed to load OpenGL");
        return 1;
    }

    wnd->dc = deviceContext;
    
    unsigned int l = 0, t = 0, w = 640, h = 400;
    RECT rect;
    if (GetWindowRect(wnd->hwnd, &rect))
    {
        l = rect.left;
        t = rect.top;
        w = rect.right - rect.left;
        h = rect.bottom - rect.top;
    }

    glViewport(0, 0, w, h);
    glClearColor(0.15, 0.1, 0.1, 0.0);
    initialized = 1;
    shaderProgram = CreateMaterial();
    return 0;
}

int MAGPollEvents(MAGWindow* wnd)
{
    MSG msg;
    int close = GetMessage(&msg, NULL, 0, 0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return close;
}

void MAGSwapBuffers(MAGWindow* wnd)
{
    SwapBuffers(wnd->dc);
}