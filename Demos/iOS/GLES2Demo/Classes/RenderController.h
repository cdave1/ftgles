/*
 *  RenderController.h
 *  OpenGLTest3
 *
 *  Created by David Petrie on 18/05/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef GLES2_RENDER_CONTROLLER_H
#define GLES2_RENDER_CONTROLLER_H

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <math.h>

#define MAX_VERTEX_COUNT 8192

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec4_t color4_t;

#define vec2Set(__v, __x, __y) __v[0] = __x; __v[1] = __y;
#define vec3Set(__v, __x, __y, __z) __v[0] = __x; __v[1] = __y; __v[2] = __z;
#define vec4Set(__v, __x, __y, __z, __u) __v[0] = __x; __v[1] = __y; __v[2] = __z; __v[3] = __u;

typedef struct vertex 
{
	float xyz[3];
	float st[2];
	float rgba[4];
} vertex_t;

enum {
    RENDER_ATTRIB_VERTEX,
    RENDER_ATTRIB_COLOR,
    RENDER_NUM_ATTRIBUTES
};


#define __11 0
#define __21 1
#define __31 2
#define __41 3
#define __12 4
#define __22 5
#define __32 6
#define __42 7
#define __13 8
#define __23 9
#define __33 10
#define __43 11
#define __14 12
#define __24 13
#define __34 14
#define __44 15

#ifdef __cplusplus
extern "C" {
#endif


void aglCross3(vec3_t vOut, const vec3_t a, const vec3_t b);


void aglNormalize3(vec3_t vOut, const vec3_t vec);


void aglMatrixMultiply(float *mOut,
					   const float *mA,
					   const float *mB);


void aglBegin(GLenum prim);

void aglVertex3f(float x, float y, float z);

void aglColor4f(float r, float g, float b, float a);

void aglTexCoord2f(float s, float t);

void aglEnd();

void aglError(const char *source);

void aglMatrixTranslation(
					   float	*mOut,
					   const float	fX,
					   const float	fY,
					   const float	fZ);

void aglMatrixRotationZ(float	*mOut,
						const float fAngle);

void aglMatrixPerspectiveFovRH(
							float	*mOut,
							const float	fFOVy,
							const float	fAspect,
							const float	fNear,
							const float	fFar);

void aglMatrixLookAtRH(float *mOut, const vec3_t vEye, const vec3_t vAt, const vec3_t vUp);



void aglOrtho(float *mOut, float left, float right, float bottom, float top, float zNear, float zFar);



#ifdef __cplusplus
}
#endif



#endif // GLES2_RENDER_CONTROLLER_H