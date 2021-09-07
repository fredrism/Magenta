#include "LINUX_MAGWindow.h"
#include "glad/glad.h"
#include "stdio.h"
#include "stdlib.h"

MAGWindow* MAGCreateWindow()
{
    MAGWindow* window = malloc(sizeof(MAGWindow));
    window->att[0] = GLX_RGBA;
    window->att[1] = GLX_DEPTH_SIZE;
    window->att[2] = 24;
    window->att[3] = GLX_DOUBLEBUFFER;
    window->att[4] = None;

    window->dpy = XOpenDisplay(NULL);
    if(window->dpy == NULL) 
    {
        printf("Cannot connect to X server! \n");
        return NULL;
    }

    window->root = DefaultRootWindow(window->dpy);
    window->vi = glXChooseVisual(window->dpy, 0, window->att);
    if(window->vi == NULL)
    {
        printf("No appropriate visual found!");
        return NULL;
    }

    window->cmap = XCreateColormap(window->dpy, window->root, window->vi->visual, AllocNone);

    window->swa.colormap = window->cmap;
    window->swa.event_mask = ExposureMask | KeyPressMask;

    window->win = XCreateWindow(window->dpy, window->root, 0, 0, 640, 400, 0, window->vi->depth, InputOutput, window->vi->visual, CWColormap | CWEventMask, &window->swa);
    XMapWindow(window->dpy, window->win);
    XStoreName(window->dpy, window->win, "Magenta Window");
    
    return window;
}

void MAGSwapBuffers(MAGWindow* wnd)
{
    glXSwapBuffers(wnd->dpy, wnd->win);
}

int CreateContext(MAGWindow* wnd)
{
    wnd->glc = glXCreateContext(wnd->dpy, wnd->vi, NULL, GL_TRUE);
    glXMakeCurrent(wnd->dpy, wnd->win, wnd->glc);
    gladLoadGL();
    XGetWindowAttributes(wnd->dpy, wnd->win, &wnd->gwa);
    glViewport(0, 0, 640, 400);
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
}

int MAGPollEvents(MAGWindow* wnd)
{
    XEvent event;
    XNextEvent(wnd->dpy, &event);
    if(event.type == KeyPress)
    {
        if(event.xkey.keycode == XK_Escape)
        {
            return 0;
        }
    }

    return 1;
}