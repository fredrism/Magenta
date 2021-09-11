#include "Magenta.h"
#include "MAGElement.h"
#include "stdio.h"
#include <malloc.h>

int main()
{
    MAGWindow* window = MAGCreateWindow();

    if(window == NULL)
    {
        return 0;
    }

    CreateContext(window);
    MAGElement elem;
    elem.style = malloc(sizeof(MAGStyle));

    elem.left = 10;
    elem.top = 10;
    elem.width = 500;
    elem.height = 500;
    
    SetColor(&elem.style->main_color, 0.2, 0.2, 0.2, 1);
    SetColor(&elem.style->border_color, 1, 0, 0, 1);

    elem.style->bevel_segments = 16;
    elem.style->bevel_radius = 100;
    elem.style->border = 1;
    elem.style->border_radius = 10;
    CreateElementBuffers(&elem);
    elem.program = CreateMaterial();

    while(MAGPollEvents(window) > 0)
    {
        DrawElement(&elem);
        MAGSwapBuffers(window);
    }

    return 0;
}