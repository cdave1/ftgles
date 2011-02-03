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

const GLchar * vertexftglesShaderProgram = "\
attribute vec4 position;\
attribute vec4 color;\
varying vec4 colorVarying;\
\
uniform mat4 camera;\
void main()\
{\
    colorVarying = color;\
    gl_Position = (camera * position);\
}\n\
";


const GLchar * fragmentftglesShaderProgram = "\
varying lowp vec4 colorVarying;\
void main()\
{\
    gl_FragColor = colorVarying;\
}\n\
";

static float ftglCameraMat[16];




GLuint ftglCameraUniform;


enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};


typedef struct 
{
	float xyz[3];
	float st[2];
	GLubyte rgba[4];
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
bool ftglesShadersInitted = false;
GLuint ftglesShaderProgram = 0;


GLuint ftglCompileShader(const GLchar * source, const GLenum type)
{
    assert(ftglesShaderProgram);
    assert(strlen(source) > 0);
    
    printf("%s\n", source);
    
    GLuint shader;
    GLint status;
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        printf("Shader compile log:\n%s\n", log);
        free(log);
    }
    
    assert(status != 0);
    
    glAttachShader(ftglesShaderProgram, shader);
    
    return shader;
}


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
    
    if (!ftglesShadersInitted)
    {
        ftglSetCamera(-320.0f, 320.0f, -480.0f, 480.0f, -10000.0f, 10000.0f);
        
        // Compile vertex shaders...
        ftglesShaderProgram = glCreateProgram();
        
        GLuint vertexShader = ftglCompileShader(vertexftglesShaderProgram, GL_VERTEX_SHADER);
        GLuint fragmentShader = ftglCompileShader(fragmentftglesShaderProgram, GL_FRAGMENT_SHADER);
        assert(vertexShader && fragmentShader);
        
        glBindAttribLocation(ftglesShaderProgram, ATTRIB_VERTEX, "position");
        glBindAttribLocation(ftglesShaderProgram, ATTRIB_COLOR, "color");
        
        glLinkProgram(ftglesShaderProgram);
        
        // Get uniform locations
        //uniformIndices[ROT_MATRIX] = glGetUniformLocation(ftglesShaderProgram, "rotation");
        //uniformIndices[CAMERA_MATRIX] = glGetUniformLocation(ftglesShaderProgram, "camera");
        //uniformIndices[TRANSLATION_UNIFORM] = glGetUniformLocation(ftglesShaderProgram, "translation");
        
        ftglCameraUniform = glGetUniformLocation(ftglesShaderProgram, "camera");
        
        // Release vertex and fragment shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        ftglesShadersInitted = true;
    }
    
    assert(ftglesQuadIndicesInitted && ftglesShadersInitted);
    
	ftglesGlueArrays.currIndex = 0;
	ftglesCurrentPrimitive = prim;
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glEnableVertexAttribArray(ATTRIB_COLOR);
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
	
	ftglesGlueArrays.currVertex.xyz[0] = x;
	ftglesGlueArrays.currVertex.xyz[1] = y;
	ftglesGlueArrays.currVertex.xyz[2] = z;
	ftglesGlueArrays.vertices[ftglesGlueArrays.currIndex] = ftglesGlueArrays.currVertex;
	ftglesGlueArrays.currIndex++;
}


GLvoid ftglVertex2f(float x, float y) 
{
	if (ftglesGlueArrays.currIndex >= FTGLES_GLUE_MAX_VERTICES)
	{
		return;
	}
	
	ftglesGlueArrays.currVertex.xyz[0] = x;
	ftglesGlueArrays.currVertex.xyz[1] = y;
	ftglesGlueArrays.currVertex.xyz[2] = 0.0f;
	ftglesGlueArrays.vertices[ftglesGlueArrays.currIndex] = ftglesGlueArrays.currVertex;
	ftglesGlueArrays.currIndex++;
}


GLvoid ftglColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) 
{
	ftglesGlueArrays.currVertex.rgba[0] = r;
	ftglesGlueArrays.currVertex.rgba[1] = g;
	ftglesGlueArrays.currVertex.rgba[2] = b;
	ftglesGlueArrays.currVertex.rgba[3] = a;
}


GLvoid ftglColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) 
{
	ftglesGlueArrays.currVertex.rgba[0] = (GLubyte) (r * 255);
	ftglesGlueArrays.currVertex.rgba[1] = (GLubyte) (g * 255);
	ftglesGlueArrays.currVertex.rgba[2] = (GLubyte) (b * 255);
	ftglesGlueArrays.currVertex.rgba[3] = (GLubyte) (a * 255);
}


GLvoid ftglTexCoord2f(GLfloat s, GLfloat t) 
{
	ftglesGlueArrays.currVertex.st[0] = s;
	ftglesGlueArrays.currVertex.st[1] = t;
}



void ftglSetCamera(float left, float right, float bottom, float top, float zNear, float zFar)
{
    //memcpy(ftglCameraMat, camera, sizeof(float) * 16);

    
    
    
    if (right != left)
    {
        ftglCameraMat[ 0] = 2 / (right - left);
        ftglCameraMat[ 1] = 0;
        ftglCameraMat[ 2] = 0;
        ftglCameraMat[ 3] = - ((right + left) / (right - left));
    }
    
    if (top != bottom)
    {
        ftglCameraMat[ 4] = 0;
        ftglCameraMat[ 5] = 2 / (top - bottom);
        ftglCameraMat[ 6] = 0;
        ftglCameraMat[ 7] = - ((top + bottom) / (top - bottom));
    }
	
    if (zFar != zNear)
    {
        ftglCameraMat[ 8] = 0;
        ftglCameraMat[ 9] = 0;
        ftglCameraMat[10] = -2 / (zFar - zNear);
        ftglCameraMat[11] = - ((zFar + zNear) / (zFar - zNear));
    }
	
	ftglCameraMat[12] = 0;
	ftglCameraMat[13] = 0;
	ftglCameraMat[14] = 0;
	ftglCameraMat[15] = 1;
}



GLvoid bindArrayBuffers()
{
}


GLvoid ftglBindTexture()
{
	
}


GLvoid ftglEnd() 
{
    /*
	GLboolean vertexArrayEnabled;
	GLboolean texCoordArrayEnabled;
	GLboolean colorArrayEnabled;
	
	GLvoid * vertexArrayPointer;
	GLvoid * texCoordArrayPointer;
	GLvoid * colorArrayPointer;
	
	GLint vertexArrayType, texCoordArrayType, colorArrayType;
	GLint vertexArraySize, texCoordArraySize, colorArraySize;
	GLsizei vertexArrayStride, texCoordArrayStride, colorArrayStride;
	
	bool resetPointers = false;
	
	glGetPointerv(GL_VERTEX_ARRAY_POINTER, &vertexArrayPointer);
	glGetPointerv(GL_TEXTURE_COORD_ARRAY_POINTER, &texCoordArrayPointer);
	glGetPointerv(GL_COLOR_ARRAY_POINTER, &colorArrayPointer);

	glGetBooleanv(GL_VERTEX_ARRAY, &vertexArrayEnabled);
	glGetBooleanv(GL_TEXTURE_COORD_ARRAY, &texCoordArrayEnabled);
	glGetBooleanv(GL_COLOR_ARRAY, &colorArrayEnabled);

	if (!vertexArrayEnabled)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
	}
	
	if (vertexArrayPointer != &ftglesGlueArrays.vertices[0].xyz)
	{
		glGetIntegerv(GL_VERTEX_ARRAY_TYPE, &vertexArrayType);
		glGetIntegerv(GL_VERTEX_ARRAY_SIZE, &vertexArraySize);
		glGetIntegerv(GL_VERTEX_ARRAY_STRIDE, &vertexArrayStride);
		if (texCoordArrayEnabled)
		{
			glGetIntegerv(GL_TEXTURE_COORD_ARRAY_TYPE, &texCoordArrayType);
			glGetIntegerv(GL_TEXTURE_COORD_ARRAY_SIZE, &texCoordArraySize);
			glGetIntegerv(GL_TEXTURE_COORD_ARRAY_STRIDE, &texCoordArrayStride);
		}	
		if (colorArrayEnabled)
		{
			glGetIntegerv(GL_COLOR_ARRAY_TYPE, &colorArrayType);
			glGetIntegerv(GL_COLOR_ARRAY_SIZE, &colorArraySize);
			glGetIntegerv(GL_COLOR_ARRAY_STRIDE, &colorArrayStride);
		}
        

        
		//glVertexPointer(3, GL_FLOAT, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].xyz);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].st);
		//glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].rgba);
		
		resetPointers = true;
	}
	
	if (!texCoordArrayEnabled)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (!colorArrayEnabled)
		glEnableClientState(GL_COLOR_ARRAY);
	*/
    
    glUseProgram(ftglesShaderProgram);
    
    
    
    glUniformMatrix4fv(ftglCameraUniform, 1, GL_FALSE, ftglCameraMat);
    
	if (ftglesGlueArrays.currIndex == 0) 
	{
		ftglesCurrentPrimitive = 0;
		return;
	}
    ftglError("a");
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].xyz);
    glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, 0, sizeof(ftglesVertex_t), ftglesGlueArrays.vertices[0].rgba);
	
	if (ftglesCurrentPrimitive == GL_QUADS) 
	{
		glDrawElements(GL_TRIANGLES, ftglesGlueArrays.currIndex / 4 * 6, GL_UNSIGNED_SHORT, ftglesGlueArrays.quadIndices);
	} 
	else 
	{
		glDrawArrays(ftglesCurrentPrimitive, 0, ftglesGlueArrays.currIndex);
	}
	ftglesGlueArrays.currIndex = 0;
	ftglesCurrentPrimitive = 0;
    
    
	
    /*
	if (resetPointers)
	{
		if (vertexArrayEnabled)
		{
			glVertexPointer(vertexArraySize, vertexArrayType, 
							vertexArrayStride, vertexArrayPointer);	
		}
		if (texCoordArrayEnabled)
		{
			glTexCoordPointer(texCoordArraySize, texCoordArrayType, 
							  texCoordArrayStride, texCoordArrayPointer);
		}
		if (colorArrayEnabled)
		{
			glColorPointer(colorArraySize, colorArrayType, 
						   colorArrayStride, colorArrayPointer);
		}
	}
	
	if (!vertexArrayEnabled)
		glDisableClientState(GL_VERTEX_ARRAY);
	
	if (!texCoordArrayEnabled)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (!colorArrayEnabled)
		glDisableClientState(GL_COLOR_ARRAY);
     */
    ftglError("z");
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

