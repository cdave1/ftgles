/*
 
 Copyright (c) 2010 David Petrie
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

#include "ftglesGlue.h"
#include "strings.h"

#define FTGLES_GLUE_MAX_VERTICES 32768


enum {
    FTGLES_ATTRIB_VERTEX,
    FTGLES_ATTRIB_TEXCOORD,
    FTGLES_ATTRIB_COLOR,
    NUM_ATTRIBUTES
};


typedef struct 
{
	float position[4];
	float texCoord[2];
	GLubyte color[4];
} ftglesVertex_t;


typedef struct 
{
	ftglesVertex_t vertices[FTGLES_GLUE_MAX_VERTICES];
	short quadIndices[FTGLES_GLUE_MAX_VERTICES * 3 / 2];
	ftglesVertex_t currVertex;
	unsigned int currIndex;
} ftglesGlueArrays_t;

ftglesGlueArrays_t ftglesGlueArrays;

GLenum ftglesCurrentPrimitive = GL_TRIANGLES;
bool ftglesQuadIndicesInitted = false;



GLvoid ftglBegin(GLenum prim) 
{
	if (!ftglesQuadIndicesInitted)
	{
		for (int i = 0; i < FTGLES_GLUE_MAX_VERTICES * 3 / 2; i += 6) 
		{
			int q = i / 6 * 4;
			ftglesGlueArrays.quadIndices[i + 0] = q + 0;
			ftglesGlueArrays.quadIndices[i + 1] = q + 1;
			ftglesGlueArrays.quadIndices[i + 2] = q + 2;
			
			ftglesGlueArrays.quadIndices[i + 3] = q + 0;
			ftglesGlueArrays.quadIndices[i + 4] = q + 2;
			ftglesGlueArrays.quadIndices[i + 5] = q + 3;
		}
		ftglesQuadIndicesInitted = true;
	}
    
	ftglesGlueArrays.currIndex = 0;
	ftglesCurrentPrimitive = prim;
}


GLvoid ftglTranslatef(float x, float y, float z)
{
    //glTranslatef(x, y, z);
}


GLvoid ftglVertex3f(float x, float y, float z) 
{
	if (ftglesGlueArrays.currIndex >= FTGLES_GLUE_MAX_VERTICES)
	{
		return;
	}
	
	ftglesGlueArrays.currVertex.position[0] = x;
	ftglesGlueArrays.currVertex.position[1] = y;
	ftglesGlueArrays.currVertex.position[2] = z;
	ftglesGlueArrays.currVertex.position[3] = 1.0f;
	ftglesGlueArrays.vertices[ftglesGlueArrays.currIndex] = ftglesGlueArrays.currVertex;
	ftglesGlueArrays.currIndex++;
}


GLvoid ftglVertex2f(float x, float y) 
{
	if (ftglesGlueArrays.currIndex >= FTGLES_GLUE_MAX_VERTICES)
	{
		return;
	}
	
	ftglesGlueArrays.currVertex.position[0] = x;
	ftglesGlueArrays.currVertex.position[1] = y;
	ftglesGlueArrays.currVertex.position[2] = 0.0f;
	ftglesGlueArrays.currVertex.position[3] = 1.0f;
	ftglesGlueArrays.vertices[ftglesGlueArrays.currIndex] = ftglesGlueArrays.currVertex;
	ftglesGlueArrays.currIndex++;
}


GLvoid ftglColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) 
{
	ftglesGlueArrays.currVertex.color[0] = r;
	ftglesGlueArrays.currVertex.color[1] = g;
	ftglesGlueArrays.currVertex.color[2] = b;
	ftglesGlueArrays.currVertex.color[3] = a;
}


GLvoid ftglColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) 
{
	ftglesGlueArrays.currVertex.color[0] = (GLubyte) (r * 255);
	ftglesGlueArrays.currVertex.color[1] = (GLubyte) (g * 255);
	ftglesGlueArrays.currVertex.color[2] = (GLubyte) (b * 255);
	ftglesGlueArrays.currVertex.color[3] = (GLubyte) (a * 255);
}


GLvoid ftglTexCoord2f(GLfloat s, GLfloat t) 
{
	ftglesGlueArrays.currVertex.texCoord[0] = s;
	ftglesGlueArrays.currVertex.texCoord[1] = t;
}



GLvoid bindArrayBuffers()
{
}


GLvoid ftglBindTexture(unsigned int textureId)
{
    ftglError("QQQ");
    GLint activeTextureID;
    //glGetIntegerv(GL_TEXTURE_BINDING_2D, &activeTextureID);
    //if((unsigned int)activeTextureID != textureId)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);


        ftglError("AAA");
    }
}


GLvoid ftglEnd() 
{
    ftglError("A");
    int currentProgram;

    ftglError("B");
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    
    if (currentProgram == 0)
    {
        return;
    }
    ftglError("C");
    
    GLint positionLocation = glGetAttribLocation(currentProgram, "vertexPosition");
    GLint texCoordLocation = glGetAttribLocation(currentProgram, "vertexTexCoord");
    GLint colorLocation = glGetAttribLocation(currentProgram, "vertexColor");

    ftglError("D");
	if (ftglesGlueArrays.currIndex == 0) 
	{
		ftglesCurrentPrimitive = 0;
		return;
	}

    ftglError("E");
    glEnableVertexAttribArray(positionLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glEnableVertexAttribArray(colorLocation);

    ftglError("F");
    glVertexAttribPointer(positionLocation, 4, GL_FLOAT, 0, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].position);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, 0, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].texCoord);
    glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, 0, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].color);

    ftglError("G");
	if (ftglesCurrentPrimitive == GL_QUADS)
	{
		glDrawElements(GL_TRIANGLES, ftglesGlueArrays.currIndex / 4 * 6, GL_UNSIGNED_SHORT, ftglesGlueArrays.quadIndices);
        ftglError("H");
	} 
	else 
	{
		glDrawArrays(ftglesCurrentPrimitive, 0, ftglesGlueArrays.currIndex);
	}

    ftglError("I");
    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(texCoordLocation);
    glDisableVertexAttribArray(colorLocation);

	ftglesGlueArrays.currIndex = 0;
	ftglesCurrentPrimitive = 0;

    ftglError("J");
}


GLvoid ftglError(const char *source)
{
	GLenum error = glGetError();
	 
	switch (error) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("GL Error (%x): GL_INVALID_ENUM. %s\n\n", error, source);
			break;
		case GL_INVALID_VALUE:
			printf("GL Error (%x): GL_INVALID_VALUE. %s\n\n", error, source);
			break;
		case GL_INVALID_OPERATION:
			printf("GL Error (%x): GL_INVALID_OPERATION. %s\n\n", error, source);
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL Error (%x): GL_OUT_OF_MEMORY. %s\n\n", error, source);
			break;
		default:
			printf("GL Error (%x): %s\n\n", error, source);
			break;
	}
}

