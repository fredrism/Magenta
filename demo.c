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

    while(MAGPollEvents() > 0)
    {
        DrawScene(window);
    }

    return 0;
}