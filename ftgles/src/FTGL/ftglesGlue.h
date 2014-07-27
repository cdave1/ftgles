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

#ifndef FTGL_ES_GLUE_H
#define FTGL_ES_GLUE_H

#include <stdio.h>
#include <assert.h>

#ifndef GL_QUADS
#define GL_QUADS 888
#endif

#ifdef FTGLES2
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif



#ifdef __cplusplus
extern "C" {
#endif

#ifdef FTGLES2
    extern void ftglBindPositionAttribute(GLint attributeHandle);

    extern void ftglBindColorAttribute(GLint attributeHandle);

    extern void ftglBindTextureAttribute(GLint attributeHandle);

#endif

	extern void ftglBegin(unsigned int prim);
    
    extern void ftglTranslatef(float x, float y, float z);
	
	extern void ftglVertex3f(float x, float y, float z);
	
	extern void ftglVertex2f(float x, float y);
	
	extern void ftglColor4f(float r, float g, float b, float a );
		
	extern void ftglTexCoord2f(float s, float t );
	
    extern void ftglBindTexture(unsigned int textureId);
    
	extern void ftglEnd();
	
	extern void ftglError(const char *source);
    
    
	
#ifdef __cplusplus
}
#endif



#endif