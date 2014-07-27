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

typedef struct vertex {
	GLfloat xyz[4];
	GLfloat st[2];
	GLfloat rgba[4];
} vertex_t;

/* We want to store things in column major order for OpenGL

 m[0]  m[4]  m[ 8]  m[12]
 m[1]  m[5]  m[ 9]  m[13]
 m[2]  m[6]  m[10]  m[14]
 m[3]  m[7]  m[11]  m[15]
 
*/

#ifdef __cplusplus
extern "C" {
#endif

    void aglBindPositionAttribute(GLint attributeHandle);

    void aglBindColorAttribute(GLint attributeHandle);

    void aglBindTextureAttribute(GLint attributeHandle);

    void aglBegin(GLenum prim);

    void aglCross3(vec3_t vOut, const vec3_t a, const vec3_t b);

    void aglNormalize3(vec3_t vOut, const vec3_t vec);

    void aglMatrixMultiply(float *mOut,
                           const float *mA,
                           const float *mB);

    void aglVertex3f(float x, float y, float z);

    void aglColor4f(float r, float g, float b, float a);

    void aglTexCoord2f(float s, float t);

    void aglEnd();

    void aglError(const char *source);

    void aglMatrixIdentity(float *mOut);

    void aglMatrixTranslation(float	*mOut,
                              const float	fX,
                              const float	fY,
                              const float	fZ);

    void aglMatrixRotationZ(float	*mOut,
                            const float fAngle);

    void aglMatrixPerspectiveFovRH(float	*mOut,
                                   const float	fFOVy,
                                   const float	fAspect,
                                   const float	fNear,
                                   const float	fFar);
    
    void aglMatrixLookAtRH(float *mOut, const vec3_t vEye, const vec3_t vAt, const vec3_t vUp);
    
    void aglOrtho(float *m, float l, float r, float b, float t, float n, float f);
    
#ifdef __cplusplus
}
#endif

#endif