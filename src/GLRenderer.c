#include "GLRenderer.h"
#include "glad/glad.h"
#include "stdio.h"

void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

/*
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
*/