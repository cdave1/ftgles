/*
 *  RenderController.c
 *  OpenGLTest3
 *
 *  Created by David Petrie on 18/05/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "RenderController.h"

static vertex_t vertices[MAX_VERTEX_COUNT];
static vertex_t vertex;
static GLenum currentPrimitive = GL_TRIANGLES;
static int vertexCount = 0;


void aglBegin(GLenum prim) {
	currentPrimitive = prim;
	vertexCount = 0;

	glVertexAttribPointer(RENDER_ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(vertex_t), vertices[0].xyz);
	glVertexAttribPointer(RENDER_ATTRIB_COLOR, 4, GL_FLOAT, 0, sizeof(vertex_t), vertices[0].rgba);
	
	glEnableVertexAttribArray(RENDER_ATTRIB_VERTEX);
	glEnableVertexAttribArray(RENDER_ATTRIB_COLOR);
}


void aglVertex3f(float x, float y, float z) {
	if (vertexCount > MAX_VERTEX_COUNT) return;
    vec3Set(vertex.xyz, x, y, z);
	vertices[vertexCount] = vertex;
	vertexCount++;
}


void aglColor4f(float r, float g, float b, float a) {
    vec4Set(vertex.rgba, r, g, b, a);
}


void aglTexCoord2f(float s, float t) {
    vec2Set(vertex.st, s, t);
}


void aglEnd() {
	if (vertexCount == 0) {
		currentPrimitive = 0;
		return;
	}
	glDrawArrays(currentPrimitive, 0, vertexCount);
	vertexCount = 0;
	currentPrimitive = 0;
}


void aglMatrixIdentity(float *mOut) {
	mOut[ 0] = 1;	mOut[ 4] = 0;	mOut[ 8] = 0;	mOut[12] = 0;
	mOut[ 1] = 0;	mOut[ 5] = 1;	mOut[ 9] = 0;	mOut[13] = 0;
	mOut[ 2] = 0;	mOut[ 6] = 0;	mOut[10] = 1;	mOut[14] = 0;
	mOut[ 3] = 0;	mOut[ 7] = 0;	mOut[11] = 0;	mOut[15] = 1;
}


void aglMatrixTranslation(float *mOut, const float fX, const float fY, const float fZ) {
	mOut[ 0] = 1;	mOut[ 4] = 0;	mOut[ 8] = 0;	mOut[12] = fX;
	mOut[ 1] = 0;	mOut[ 5] = 1;	mOut[ 9] = 0;	mOut[13] = fY;
	mOut[ 2] = 0;	mOut[ 6] = 0;	mOut[10] = 1;	mOut[14] = fZ;
	mOut[ 3] = 0;	mOut[ 7] = 0;	mOut[11] = 0;	mOut[15] = 1;
}


void aglMatrixRotationZ(float *mOut, const float fAngle) {
	float fsin = sinf(fAngle);
	float fcos = cosf(fAngle);
    
    mOut[ 0] = fcos;	mOut[ 4] = fsin;	mOut[ 8] = 0;	mOut[12] = 0;
	mOut[ 1] = -fsin;	mOut[ 5] = fcos;	mOut[ 9] = 0;	mOut[13] = 0;
	mOut[ 2] = 0;       mOut[ 6] = 0;       mOut[10] = 1;	mOut[14] = 0;
	mOut[ 3] = 0;       mOut[ 7] = 0;       mOut[11] = 0;	mOut[15] = 1;
}


void aglMatrixPerspectiveFovRH(float *mOut,
							const float	fFOVy,
							const float	fAspect,
							const float	fNear,
							const float	fFar) {
	float f, n, fRealAspect;
	
	fRealAspect = fAspect;
	
	// cotangent(a) == 1.0f / tan(a);
	f = 1.0f / (float)tan(fFOVy * 0.5f);
	n = 1.0f / (fNear - fFar);
	
	mOut[ 0] = f / fRealAspect;
	mOut[ 1] = 0;
	mOut[ 2] = 0;
	mOut[ 3] = 0;
	
	mOut[ 4] = 0;
	mOut[ 5] = f;
	mOut[ 6] = 0;
	mOut[ 7] = 0;
	
	mOut[ 8] = 0;
	mOut[ 9] = 0;
	mOut[10] = (fFar + fNear) * n;
	mOut[11] = -1;
	
	mOut[12] = 0;
	mOut[13] = 0;
	mOut[14] = (2 * fFar * fNear) * n;
	mOut[15] = 0;
}


void aglCross3(vec3_t vOut, const vec3_t a, const vec3_t b) {
	vec3Set(vOut,
			(a[1] * b[2]) - (a[2] * b[1]),
			(a[2] * b[0]) - (a[0] * b[2]),
			(a[0] * b[1]) - (a[1] * b[0]));
}


void aglNormalize3(vec3_t vOut, const vec3_t vec) {
	float	f;
	double temp;
	
	temp = (double)(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	temp = 1.0 / sqrt(temp);
	f = (float)temp;
	vec3Set(vOut, vec[0] * f, vec[1] * f, vec[2] * f);
}


void aglMatrixLookAtRH(float *mOut, const vec3_t vEye, const vec3_t vAt, const vec3_t vUp) {
	vec3_t f, vUpActual, s, u;
	float	t[16];
	
	f[0] = vAt[0] - vEye[0];
	f[1] = vAt[1] - vEye[1];
	f[2] = vAt[2] - vEye[2];
	
	aglNormalize3(f, f);
	aglNormalize3(vUpActual, vUp);
	aglCross3(s, f, vUpActual);
	aglCross3(u, s, f);
	
	mOut[ 0] = s[0];
	mOut[ 1] = u[0];
	mOut[ 2] = -f[0];
	mOut[ 3] = 0;
	
	mOut[ 4] = s[1];
	mOut[ 5] = u[1];
	mOut[ 6] = -f[1];
	mOut[ 7] = 0;
	
	mOut[ 8] = s[2];
	mOut[ 9] = u[2];
	mOut[10] = -f[2];
	mOut[11] = 0;
	
	mOut[12] = 0;
	mOut[13] = 0;
	mOut[14] = 0;
	mOut[15] = 1;
	
	aglMatrixTranslation(t, -vEye[0], -vEye[1], -vEye[2]);
	aglMatrixMultiply(mOut, t, mOut);
}


void aglMatrixMultiply(float *mOut,
					  const float *mA,
					  const float *mB) {
    for(int i = 0; i < 4; i++) {
        mOut[i*4] =   mA[i*4] * mB[ 0] + mA[i*4+1] * mB[ 4] + mA[i*4+2] * mB[ 8] + mA[i*4+3] * mB[12];
        mOut[i*4+1] = mA[i*4] * mB[ 1] + mA[i*4+1] * mB[ 5] + mA[i*4+2] * mB[ 9] + mA[i*4+3] * mB[13];
        mOut[i*4+2] = mA[i*4] * mB[ 2] + mA[i*4+1] * mB[ 6] + mA[i*4+2] * mB[10] + mA[i*4+3] * mB[14];
        mOut[i*4+3] = mA[i*4] * mB[ 3] + mA[i*4+1] * mB[ 7] + mA[i*4+2] * mB[11] + mA[i*4+3] * mB[15];
    }
}


void aglOrtho(float *mOut, float left, float right, float bottom, float top, float near, float far) {
    aglMatrixIdentity(mOut);
    
    float w = (right - left);
    float h = (top - bottom);
    float d = (far - near);
    
    mOut[ 0] = 2.0f / w;    mOut[ 4] = 0;           mOut[ 8] = 0;       mOut[12] = -1;
	mOut[ 1] = 0;           mOut[ 5] = 2.0f / h;    mOut[ 9] = 0;       mOut[13] = 1;
	mOut[ 2] = 0;           mOut[ 6] = 0;           mOut[10] = -2.0f/d;  mOut[14] = 0;
	mOut[ 3] = 0;           mOut[ 7] = 0;           mOut[11] = 0;       mOut[15] = 1;
}
