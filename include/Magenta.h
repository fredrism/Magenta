typedef void MAGWindow;

MAGWindow* MAGCreateWindow();
int MAGPollEvents(MAGWindow* wnd);
void MAGSwapBuffers(MAGWindow* wnd);
int CreateContext(MAGWindow* wnd);
void DrawScene();