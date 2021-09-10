#ifndef MAGElement_H
#define MAGElement_H

typedef struct MAGElementBuffer
{
    unsigned int main_vbo;
    unsigned int main_ibo;
    unsigned int border_ibo;
    int num_tris;
    int border_begin;

} MAGElementBuffer;

typedef struct MAGColor
{
    float r;
    float g;
    float b;
    float a;
} MAGColor;

void SetColor(MAGColor* color, float r, float g, float b, float a);

typedef struct MAGStyle
{
    MAGColor main_color;
    MAGColor border_color;

    float bevel_radius;
    unsigned int bevel_segments;
    
    float border_radius;
    unsigned int border;

} MAGStyle;

typedef struct MAGVector
{
    float x;
    float y;
    float z;
} MAGVector;

typedef struct MAGTriangle
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
} MAGTriangle;

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
    float top;
    float left;
    float width;
    float height;
    unsigned int program;

    MAGStyle* style;
    MAGElementBuffer buffers;
} MAGElement;

void CreateElementBuffers(MAGElement* element);

#endif