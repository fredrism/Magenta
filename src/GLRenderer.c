#include "GLRenderer.h"
#include "glad/glad.h"
#include "stdio.h"
#include "MAGElement.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 mat;"
    "void main()\n"
    "{\n"
    "   vec4 pos = mat * vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
    "   gl_Position = pos/pos.w;\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"

    "void main()\n"
    "{\n"
    "   FragColor = color;\n"
    "}\n\0";

void DrawElement(MAGElement* elem)
{
    glClear(GL_COLOR_BUFFER_BIT);

	int color_pos = glGetUniformLocation(elem->program, "color");
	glUniform4f(color_pos, elem->style->border_color.r, elem->style->border_color.g, elem->style->border_color.b, elem->style->border_color.a);

    glBindBuffer(GL_ARRAY_BUFFER, elem->buffers.main_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem->buffers.border_ibo);

    glDrawElements(GL_TRIANGLES , elem->buffers.num_tris * 3, GL_UNSIGNED_INT, NULL);

    glUniform4f(color_pos, elem->style->main_color.r, elem->style->main_color.g, elem->style->main_color.b, elem->style->main_color.a);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem->buffers.main_ibo);

    glDrawElements(GL_TRIANGLES , elem->buffers.num_tris * 3, GL_UNSIGNED_INT, NULL);
}

unsigned int CreateMaterial()
{
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s \n", infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s \n", infoLog);
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s \n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    printf("Successfully compiled shaders\n");
    printf(vertexShaderSource);
    glUseProgram(shaderProgram);

    return shaderProgram;
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