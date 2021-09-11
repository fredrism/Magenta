#ifndef MAGElement_H
#define MAGElement_H
#include "MAGVector.h"

typedef struct MAGElementBuffer
{
    unsigned int main_vbo;
    unsigned int main_ibo;
    unsigned int border_ibo;
    int num_tris;
    int border_begin;

} MAGElementBuffer;

typedef struct MAGStyle
{
    MAGColor main_color;
    MAGColor border_color;

    float bevel_radius;
    unsigned int bevel_segments;
    
    float border_radius;
    unsigned int border;

} MAGStyle;

typedef struct ElemVertexArrayDesc
{
    int num_vertices;
    int verts_per_corner;
    int bevel_verts;
    int border;
    int num_tris;
    int border_begin;
} ElemVertexArrayDesc;

typedef struct MAGElement
{
    unsigned int top;
    unsigned int left;
    unsigned int width;
    unsigned int height;
    unsigned int program;

    MAGStyle* style;
    MAGElementBuffer buffers;
} MAGElement;

void CreateElementBuffers(MAGElement* element);

#endif