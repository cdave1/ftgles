/*
 *  ftOpenGLES.h
 *  iPhonefonts
 *
 *  Created by david on 5/11/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef FTGL_ES_GLUE_H
#define FTGL_ES_GLUE_H

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <stdio.h>
#include <assert.h>

#define GL_QUADS 888

#ifdef __cplusplus
extern "C" {
#endif
	
	extern GLvoid ftglInitImmediateModeGL();
	
	extern GLvoid ftglBegin( GLenum prim );
	
	extern GLvoid ftglVertex3f( float x, float y, float z );
	
	extern GLvoid ftglVertex2f( float x, float y);
	
	extern GLvoid ftglColor4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
		
	extern GLvoid ftglTexCoord2f( GLfloat s, GLfloat t );
	
	extern GLvoid ftglEnd();
	
	extern GLvoid ftglError(const char* source);
	
#ifdef __cplusplus
}
#endif



#endif