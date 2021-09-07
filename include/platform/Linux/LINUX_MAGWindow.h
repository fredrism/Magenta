#include "X11/Xlib.h"
#include "glad/glad.h"
#include "GL/glx.h"
//#include "GL/glu.h"

typedef struct MAGWindow
{
    Display* dpy;
    Window root;
    GLint att[5];
    XVisualInfo* vi;
    Colormap cmap;
    XSetWindowAttributes swa;
    Window win;
    GLXContext glc;
    XWindowAttributes gwa;

} MAGWindow;

MAGWindow* MAGCreateWindow();
int CreateContext(MAGWindow* wnd);
int MAGPollEvents(MAGWindow* wnd);