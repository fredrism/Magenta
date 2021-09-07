#ifndef MAGWindow_H
#define MAGWindow_H

#include "Windows.h"
#include "glad/glad.h"
#include "gl/GL.h"

typedef struct MAGWindow
{
    HWND hwnd;
    HDC dc;
} MAGWindow;

MAGWindow* MAGCreateWindow();
int MAGPollEvents(MAGWindow* wnd);
int CreateContext(MAGWindow* wnd);

#endif