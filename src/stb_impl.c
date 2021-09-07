#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stdio.h"
#include "glad/glad.h"

stbtt_bakedchar cdata[96];
GLuint ftex;

unsigned char ttf_buffer[1 << 20];
unsigned char temp_bitmap[512 * 512];

void InitFont()
{
    fread(ttf_buffer, 1, 1<<20, fopen("/media/fredrik/E278FC5878FC2D43/dev/Magenta/build/fonts/Official.ttf", "rb"));
    stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);
    glGenTextures(1, &ftex);
    glBindTexture(GL_TEXTURE_2D, ftex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void DrawText(float x, float y, char* text)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ftex);
    glBegin(GL_QUADS);
    while(*text)
    {
        if(*text >= 32 && *text < 128)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 512, 512, *text-32, &x, &y, &q, 1);
            glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0 * 0.01, -q.y0 * 0.01);
            glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1 * 0.01, -q.y0 * 0.01);
            glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1 * 0.01, -q.y1 * 0.01);
            glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0 * 0.01, -q.y1 * 0.01);
        }
        ++text;
    }
    glEnd();
}