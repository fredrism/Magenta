#include "MAGElement.h"
#include "glad/glad.h"
#include "math.h"
#include <malloc.h>
#include <stdio.h>

#define PI 3.14159265359

void SetVector(MAGVector* vector, float x, float y, float z)
{
	vector->x = x;
	vector->y = y;
	vector->z = z;
}

void SetTriangle(MAGTriangle* triangle, int a, int b, int c)
{
	triangle->a = a;
	triangle->b = b;
	triangle->c = c;
}

MAGVector* ComputeVertices(MAGElement* elem, ElemVertexArrayDesc* desc)
{
	float max_radius = fmin(elem->width * 0.5, elem->height * 0.5);
	float radius = fmax(0, fmin(max_radius, elem->style->bevel_radius));

	int border = elem->style->border;
	int bevel = (elem->style->bevel_segments >= 2);
	int bevel_verts = (bevel) ? elem->style->bevel_segments : 3;

	float border_width = elem->style->border_radius;

	float width = elem->width;
	float height = elem->height;

	if (!bevel)
	{
		radius = fmin(width * 0.2, height * 0.2);
	}

	float left = elem->left + radius;
	float right = elem->left + width - radius;
	float top = elem->top + radius;
	float bottom = elem->top + height - radius;

	int verts_per_corner = bevel_verts;
	if (elem->style->border)
	{
		verts_per_corner *= 2;
	}

	verts_per_corner++;

	int num_vertices = 4 * verts_per_corner;

	MAGVector* vertices = malloc(sizeof(MAGVector) * num_vertices);

	SetVector(&vertices[0], left, top, 0);
	SetVector(&vertices[verts_per_corner], right, top, 0);
	SetVector(&vertices[2 * verts_per_corner], right, bottom, 0);
	SetVector(&vertices[3 * verts_per_corner], left, bottom, 0);

	float a = -PI;
	float increment = (2.0 * PI) / (4.0 * (bevel_verts - 1));

	for (int i = 0; i < num_vertices; i += verts_per_corner)
	{
		for (int j = i + 1; j <= i + bevel_verts; j++)
		{
			vertices[j].x = cos(a);
			vertices[j].y = sin(a);
			vertices[j].z = 0;

			if (!bevel)
			{
				vertices[j].x = round(vertices[j].x);
				vertices[j].y = round(vertices[j].y);
			}

			if (border)
			{
				vertices[j + bevel_verts].x = vertices[i].x + vertices[j].x * (radius + border_width);
				vertices[j + bevel_verts].y = vertices[i].y + vertices[j].y * (radius + border_width);
				vertices[j + bevel_verts].z = vertices[i].z + vertices[j].z * (radius + border_width);
			}

			vertices[j].x = vertices[i].x + vertices[j].x * radius;
			vertices[j].y = vertices[i].y + vertices[j].y * radius;
			vertices[j].z = vertices[i].z + vertices[j].z * radius;

			a += increment;
		}

		a -= increment;
	}

	desc->bevel_verts = bevel_verts;
	desc->num_vertices = num_vertices;
	desc->verts_per_corner = verts_per_corner;
	desc->border = border;

	/*
	printf("\n\n\nBEGIN VERTEX \n\n");

	for (int i = 0; i < num_vertices; i++)
	{
		printf("(%f, %f) : %d \n", vertices[i].x, vertices[i].y, i);
	}*/

	return vertices;
}

MAGTriangle* ComputeIndices(MAGVector* vertices, ElemVertexArrayDesc* desc)
{
	unsigned int bevel_verts = desc->bevel_verts;
	unsigned int verts_per_corner = desc->verts_per_corner;
	unsigned int border = desc->border;

#define GetInnerVertex(x) (((x) % 4) * verts_per_corner)
#define GetOuterVertex(x, y) (GetInnerVertex(x) + 1 + (y % bevel_verts))
#define GetBorderVertex(x, y) (GetOuterVertex(x, y) + bevel_verts)

	unsigned int num_tris_per_corner = bevel_verts + 1;
	unsigned int num_tris_per_border = 0;

	if (border)
	{
		num_tris_per_border = 2 + 2 * (bevel_verts - 1);
	}

	unsigned int num_tris = 2 + (num_tris_per_corner + num_tris_per_border) * 4;
	MAGTriangle* indices = malloc(num_tris * sizeof(MAGTriangle));

	indices[0].a = GetInnerVertex(0);
	indices[0].b = GetInnerVertex(2);
	indices[0].c = GetInnerVertex(1);

	indices[1].a = GetInnerVertex(2);
	indices[1].b = GetInnerVertex(0);
	indices[1].c = GetInnerVertex(3);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < bevel_verts - 1; j++)
		{
			int index = 2 + num_tris_per_corner * i + j;
			indices[index].a = GetInnerVertex(i);
			indices[index].b = GetOuterVertex(i, j + 1);
			indices[index].c = GetOuterVertex(i, j);
		}

		int index = 2 + num_tris_per_corner * i + bevel_verts - 1;
		indices[index].a = GetInnerVertex(i);
		indices[index].b = GetInnerVertex(i + 1);
		indices[index].c = GetOuterVertex(i, bevel_verts - 1);

		indices[index + 1].a = GetOuterVertex(i, bevel_verts - 1);
		indices[index + 1].b = GetInnerVertex(i + 1);
		indices[index + 1].c = GetOuterVertex(i + 1, bevel_verts);

	}

	int begin_border = 2 + num_tris_per_corner * 3 + bevel_verts + 1;
	int indx = begin_border;

	if (border)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < bevel_verts - 1; j++)
			{
				indices[indx].a = GetOuterVertex(i, j);
				indices[indx].b = GetOuterVertex(i, j + 1);
				indices[indx].c = GetBorderVertex(i, j);

				indx++;

				indices[indx].a = GetOuterVertex(i, j + 1);
				indices[indx].b = GetBorderVertex(i, j + 1);
				indices[indx].c = GetBorderVertex(i, j);

				indx++;
			}

			indices[indx].a = GetOuterVertex(i, bevel_verts - 1);
			indices[indx].b = GetOuterVertex(i + 1, 0);
			indices[indx].c = GetBorderVertex(i, bevel_verts - 1);

			indx++;

			indices[indx].a = GetOuterVertex(i + 1, 0);
			indices[indx].b = GetBorderVertex(i + 1, 0);
			indices[indx].c = GetBorderVertex(i, bevel_verts - 1);

			indx++;
		}
	}

	unsigned int* actual_indices = malloc(sizeof(unsigned int) * num_tris * 3);

	

	for (int i = 0; i < num_tris; i++)
	{
		actual_indices[3 * i] = indices[i].a;
		actual_indices[3 * i + 1] = indices[i].b;
		actual_indices[3 * i + 2] = indices[i].c;
	}

	/*
	printf("\n\n\n\nBEGIN INDICES : %d\n\n", num_tris * 3);

	for (int i = 0; i < num_tris * 3; i += 3)
	{
		printf("%d, %d, %d \n", actual_indices[i], actual_indices[i + 1], actual_indices[i + 2]);
	}*/

	desc->num_tris = num_tris;
	desc->border_begin = begin_border;

	return indices;
}

void CreateElementBuffers(MAGElement* element)
{
	unsigned int vbo;
	unsigned int main_ibo;
	unsigned int border_ibo;

	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &main_ibo);
	glCreateBuffers(1, &border_ibo);

	ElemVertexArrayDesc desc;

	MAGVector* vertices = ComputeVertices(element, &desc);
	MAGTriangle* indices = ComputeIndices(vertices, &desc);
    
	printf("\n\nNUM Vertices: %d \n\n", desc.num_vertices);

	for (int i = 0; i < desc.num_vertices; i++)
	{
		//printf("(%f, %f, %f) \n", vertices[i].x, vertices[i].y, vertices[i].z);
	}
	printf("\n\n\nNUM TRIS: %d \n\n", desc.num_tris);

	for (int i = 0; i < desc.num_tris; i++)
	{
		//printf("(%d, %d, %d) \n", indices[3 * i].a, indices[3 * i + 1].b, indices[3 * i + 2].c);
	}
    
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * desc.num_vertices * 3, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, main_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * desc.border_begin * 3, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (desc.num_tris - desc.border_begin) * 3, &indices[desc.border_begin], GL_STATIC_DRAW);

	element->buffers.main_ibo = main_ibo;
	element->buffers.border_ibo = border_ibo;
	element->buffers.main_vbo = vbo;
	element->buffers.border_begin = desc.border_begin;
	element->buffers.num_tris = desc.num_tris;

	free(vertices);
	free(indices);
}

void SetColor(MAGColor* color, float r, float g, float b, float a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}