#include "MAGWindow.h"
#include "stdio.h"

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

    const char* version = glGetString(GL_VERSION);
    printf("OpenGL Version: %s", version);
    glViewport(0, 0, 640, 400);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    return 0;
}

void DrawScene(MAGWindow* wnd)
{
	glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(wnd->dc);
}

int glCheckError()
{
    GLenum error = glGetError();
    int result = 0;
    while(error != GL_NO_ERROR)
    {
        result = 1;
        switch(error)
        {
            case GL_INVALID_ENUM:                  printf("INVALID_ENUM \n"); break;
            case GL_INVALID_VALUE:                 printf("INVALID_VALUE \n"); break;
            case GL_INVALID_OPERATION:             printf("INVALID_OPERATION \n"); break;
            case GL_STACK_OVERFLOW:                printf("STACK_OVERFLOW \n"); break;
            case GL_STACK_UNDERFLOW:               printf("STACK_UNDERFLOW \n"); break;
            case GL_OUT_OF_MEMORY:                 printf("OUT_OF_MEMORY \n"); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: printf("INVALID_FRAMEBUFFER_OPERATION \n"); break;
        }

        error = glGetError();
    }

    return result;
}