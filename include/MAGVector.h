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

typedef struct MAGColor
{
    float r;
    float g;
    float b;
    float a;
} MAGColor;

void SetColor(MAGColor* color, float r, float g, float b, float a);