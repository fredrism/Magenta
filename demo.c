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
    printf("Hello");
    while(MAGPollEvents(window) > 0)
    {
        DrawScene();
        MAGSwapBuffers(window);
    }

    return 0;
}