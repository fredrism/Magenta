#include "Magenta.h"
#include "stdio.h"

int main()
{
    MAGWindow* window = MAGCreateWindow();

    if(window == NULL)
    {
        return 0;
    }

    CreateContext(window);
    InitFont();

    while(MAGPollEvents(window) > 0)
    {
        DrawScene();
        DrawText(-100, 10, "Well well, well, hello there chap! we're going hard");
        MAGSwapBuffers(window);
    }

    return 0;
}