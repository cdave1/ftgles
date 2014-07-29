#include "RenderController.h"

static vertex_t vertices[MAX_VERTEX_COUNT];
static vertex_t vertex;
static GLenum currentPrimitive = GL_TRIANGLES;
static int vertexCount = 0;

void aglBindPositionAttribute(GLint attributeHandle) {
    glVertexAttribPointer(attributeHandle, 4, GL_FLOAT, 0, sizeof(vertex_t), vertices[0].xyz);
    glEnableVertexAttribArray(attributeHandle);
}


void aglBindColorAttribute(GLint attributeHandle) {
    glVertexAttribPointer(attributeHandle, 4, GL_FLOAT, 0, sizeof(vertex_t), vertices[0].rgba);
    glEnableVertexAttribArray(attributeHandle);
}


void aglBindTextureAttribute(GLint attributeHandle) {
    glVertexAttribPointer(attributeHandle, 2, GL_FLOAT, 0, sizeof(vertex_t), vertices[0].st);
    glEnableVertexAttribArray(attributeHandle);
}


void aglBegin(GLenum prim) {
    currentPrimitive = prim;
    vertexCount = 0;
}


void aglVertex3f(float x, float y, float z) {
    if (vertexCount > MAX_VERTEX_COUNT) return;
    vec4Set(vertex.xyz, x, y, z, 1.0f);
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


float aglDot(const vec3_t a, const vec3_t b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
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
                               const float	fov_y,
                               const float	aspect,
                               const float	near,
                               const float	far) {
    float f, n, realAspect;

    realAspect = aspect;

    // cotangent(a) == 1.0f / tan(a);
    f = 1.0f / (float)tan(fov_y * 0.5f);
    n = 1.0f / (near - far);

    mOut[ 0] = f / realAspect; mOut[ 4] = 0;   mOut[ 8] = 0;                 mOut[12] = 0;
    mOut[ 1] = 0;              mOut[ 5] = f;   mOut[ 9] = 0;                 mOut[13] = 0;
    mOut[ 2] = 0;              mOut[ 6] = 0;   mOut[10] = (far + near) * n;  mOut[14] = (2 * far * near) * n;
    mOut[ 3] = 0;              mOut[ 7] = 0;   mOut[11] = -1;                mOut[15] = 0;
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


void aglMatrixLookAtRH(float *mOut, const vec3_t pos, const vec3_t target, const vec3_t up) {
    vec3_t f, vUpActual, s, u;
    float t[16];
    vec3_t x_axis, y_axis, z_axis;

    f[0] = pos[0] - target[0];
    f[1] = pos[1] - target[1];
    f[2] = pos[2] - target[2];

    aglNormalize3(z_axis, f);
    aglCross3(x_axis, up, z_axis);
    aglNormalize3(x_axis, x_axis);
    aglCross3(y_axis, z_axis, x_axis);

    mOut[ 0] = x_axis[0];   mOut[ 4] = x_axis[1];   mOut[ 8] = x_axis[2];   mOut[12] = -aglDot(x_axis, pos);
    mOut[ 1] = y_axis[0];   mOut[ 5] = y_axis[1];   mOut[ 9] = y_axis[2];   mOut[13] = -aglDot(y_axis, pos);
    mOut[ 2] = z_axis[0];   mOut[ 6] = z_axis[1];   mOut[10] = z_axis[2];   mOut[14] = -aglDot(z_axis, pos);
    mOut[ 3] = 0;           mOut[ 7] = 0;           mOut[11] = 0;           mOut[15] = 1;
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


void aglOrtho(float *m, float l, float r, float b, float t, float n, float f) {
    m[ 0] = 2.0f / (r - l); m[ 4] = 0;              m[ 8] = 0;              m[12] = -(r + l) / (r - l);
    m[ 1] = 0;              m[ 5] = 2.0f / (t - b); m[ 9] = 0;              m[13] = -(t + b) / (t - b);
    m[ 2] = 0;              m[ 6] = 0;              m[10] = -1.0f/ (f - n); m[14] = -n / (f - n);
    m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;              m[15] = 1;
}
