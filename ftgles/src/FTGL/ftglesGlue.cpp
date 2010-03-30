/*
 *  ftglesGlue.cpp
 *  iPhonefonts
 *
 *  Created by david on 5/11/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "ftglesGlue.h"

struct Vertex 
{
	float xyz[3];
	float st[2];
	GLubyte c[4];
};

#define MAX_VERTS 16384

typedef struct Vertex Vertex;
Vertex immediate[MAX_VERTS];
Vertex vab;
short quad_indexes[MAX_VERTS * 3 / 2];
int curr_vertex;
GLenum curr_prim;
bool initted = false;

GLvoid ftglInitImmediateModeGL() 
{
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), immediate[0].xyz);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), immediate[0].st);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), immediate[0].c);
	
	if (initted == false)
	{
		for (int i = 0; i < MAX_VERTS * 3 / 2; i+=6) {
			int q = i / 6 * 4;
			quad_indexes[i + 0] = q + 0;
			quad_indexes[i + 1] = q + 1;
			quad_indexes[i + 2] = q + 2;
			
			quad_indexes[i + 3] = q + 0;
			quad_indexes[i + 4] = q + 2;
			quad_indexes[i + 5] = q + 3;
		}
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		initted = true;
	}
}


GLvoid ftglBegin(GLenum prim) 
{
	curr_vertex = 0;
	curr_prim = prim;
}


GLvoid ftglVertex3f(float x, float y, float z) 
{
	assert(curr_vertex < MAX_VERTS);
	vab.xyz[0] = x;
	vab.xyz[1] = y;
	vab.xyz[2] = z;
	immediate[curr_vertex] = vab;
	curr_vertex++;
}


GLvoid ftglVertex2f(float x, float y) 
{
	assert(curr_vertex < MAX_VERTS);
	vab.xyz[0] = x;
	vab.xyz[1] = y;
	vab.xyz[2] = 0.0f;
	immediate[curr_vertex] = vab;
	curr_vertex++;
}


GLvoid ftglColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) 
{
	vab.c[0] = r;
	vab.c[1] = g;
	vab.c[2] = b;
	vab.c[3] = a;
}


GLvoid ftglColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) 
{
	vab.c[0] = (GLubyte) (r * 255);
	vab.c[1] = (GLubyte) (g * 255);
	vab.c[2] = (GLubyte) (b * 255);
	vab.c[3] = (GLubyte) (a * 255);
}


GLvoid ftglTexCoord2f(GLfloat s, GLfloat t) 
{
	vab.st[0] = s;
	vab.st[1] = t;
}


GLvoid ftglEnd() 
{
	if (curr_vertex == 0) 
	{
		curr_prim = 0;
		return;
	}
	
	if (curr_prim == GL_QUADS) 
	{
		glDrawElements(GL_TRIANGLES, curr_vertex / 4 * 6, GL_UNSIGNED_SHORT, quad_indexes);
	} 
	else 
	{
		glDrawArrays(curr_prim, 0, curr_vertex);
	}
	curr_vertex = 0;
	curr_prim = 0;
}


GLvoid ftglError(const char* source)
{
	GLenum error = glGetError();
	 
	switch (error) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("GL Error (%d): GL_INVALID_ENUM. %s\n\n", error, source);
			break;
		case GL_INVALID_VALUE:
			printf("GL Error (%d): GL_INVALID_VALUE. %s\n\n", error, source);
			break;
		case GL_INVALID_OPERATION:
			printf("GL Error (%d): GL_INVALID_OPERATION. %s\n\n", error, source);
			break;
		case GL_STACK_OVERFLOW:
			printf("GL Error (%d): GL_STACK_OVERFLOW. %s\n\n", error, source);
			break;
		case GL_STACK_UNDERFLOW:
			printf("GL Error (%d): GL_STACK_UNDERFLOW. %s\n\n", error, source);
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL Error (%d): GL_OUT_OF_MEMORY. %s\n\n", error, source);
			break;
		default:
			break;
	}
}