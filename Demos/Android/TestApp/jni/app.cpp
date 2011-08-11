/* San Angeles Observation OpenGL ES version example
 * Copyright 2009 The Android Open Source Project
 * All rights reserved.
 *
 * This source is free software; you can redistribute it and/or
 * modify it under the terms of EITHER:
 *   (1) The GNU Lesser General Public License as published by the Free
 *       Software Foundation; either version 2.1 of the License, or (at
 *       your option) any later version. The text of the GNU Lesser
 *       General Public License is included with this source in the
 *       file LICENSE-LGPL.txt.
 *   (2) The BSD-style license that is included with this source in
 *       the file LICENSE-BSD.txt.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
 */

#include <iostream>
#include <fstream>
using namespace std;

#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <stdio.h>
#include "FTGL/ftgles.h"
#include "FTGL/ftglesGlue.h"
#include <GLES/gl.h>
#include "test.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestApp", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TestApp", __VA_ARGS__))


#define WINDOW_DEFAULT_WIDTH    640
#define WINDOW_DEFAULT_HEIGHT   480

#define WINDOW_BPP              16

extern "C" {

static long
_getTime(void);

/* Call to initialize the graphics state */

JNIEXPORT void JNICALL
Java_com_example_TestApp_TestAppActivity_nativeSetup( JNIEnv*  env, jlong offset, jlong len, jstring source );

JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeInit( JNIEnv*  env );

JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h );

/* Call to finalize the graphics state */
JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeDone( JNIEnv*  env );

/* This is called to indicate to the render loop that it should
 * stop as soon as possible.
 */
JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoGLSurfaceView_nativePause( JNIEnv*  env );

/* Call to render the next GL frame */
JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeRender( JNIEnv*  env );



  
// The simple framework expects the application code to define these functions.
extern void appInit();
extern void appDeinit();
extern void appRender(long tick, int width, int height);

/* Value is non-zero when application is alive, and 0 when it is closing.
 * Defined by the application framework.
 */
extern int gAppAlive;


};

	int frames;
	long CurrentTime;
	long LastFPSUpdate;
	FTFont *font;
 unsigned char * buffer = NULL;
int bufferLen = 0;

  myTest * _myTest;

int   gAppAlive   = 1;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;
static long sStartTick = 0;
static long sTick = 0;

static long
_getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
    }

JNIEXPORT void JNICALL
Java_com_example_TestApp_TestAppActivity_nativeSetup( JNIEnv*  env, jlong offset, jlong len, jstring source )
{

  bufferLen = len;

  const char* sourcePath = env->GetStringUTFChars(source, 0);
 
  //printf("%s", sourcePath);
  LOGI("%s", sourcePath);

    ifstream ifs(sourcePath);
    if (!ifs.is_open())
    {
      LOGW("Could not open file");
    }

    if (!ifs.good())
      {
	LOGW("no");
	ifs.close();
      }

    ifs.seekg(offset);
    if (NULL == (buffer = (unsigned char *)calloc(1, sizeof(unsigned char) * len)))
      {
	ifs.close();
	LOGW("calloc");
	return;
      }

    ifs.read((char *)buffer, len);

    ifs.close();

  

}


/* Call to initialize the graphics state */
JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeInit( JNIEnv*  env )
{
   appInit();
     gAppAlive    = 1;
    sDemoStopped = 0;
   sTimeOffsetInit = 0;
}

JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
     sWindowWidth  = w;
    sWindowHeight = h;
     __android_log_print(ANDROID_LOG_INFO, "TestApp", "my resize w=%d h=%d", w, h);
}

/* Call to finalize the graphics state */
JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeDone( JNIEnv*  env )
{
    appDeinit();
}

/* This is called to indicate to the render loop that it should
 * stop as soon as possible.
 */

JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoGLSurfaceView_nativePause( JNIEnv*  env )
{
   sDemoStopped = !sDemoStopped;
   if (sDemoStopped) {
        /* we paused the animation, so store the current
         * time in sTimeStopped for future nativeRender calls */
       sTimeStopped = _getTime();
   } else {
        /* we resumed the animation, so adjust the time offset
         * to take care of the pause interval. */
      sTimeOffset -= _getTime() - sTimeStopped;
   }
}

/* Call to render the next GL frame */
JNIEXPORT void JNICALL
Java_com_example_TestApp_DemoRenderer_nativeRender( JNIEnv*  env )
{
  
    long   curTime;

    if (sDemoStopped) {
        curTime = sTimeStopped + sTimeOffset;
    } else {
        curTime = _getTime() + sTimeOffset;
        if (sTimeOffsetInit == 0) {
            sTimeOffsetInit = 1;
            sTimeOffset     = -curTime;
            curTime         = 0;
        }
	}

   appRender(curTime, sWindowWidth, sWindowHeight);
}



// Called from the app framework.
void appInit()
{ glEnable(GL_TEXTURE_2D);

    font = new FTOutlineFont(buffer, bufferLen);
				 //[fontpath UTF8String]);

    free(buffer);

    if (font->Error())
      {
	LOGW("Font problem.");
		delete font;
		font = NULL;
		return;
	}
	font->FaceSize(sWindowWidth * 0.16f);

  glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
}


// Called from the app framework.
void appDeinit()
{
  // delete font;
}



void _glError(const char *source)
{
	GLenum error = glGetError();
	 
	switch (error) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
		  LOGW("GL Error (%x): GL_INVALID_ENUM. %s\n\n", error, source);
			break;
		case GL_INVALID_VALUE:
		 LOGW("GL Error (%x): GL_INVALID_VALUE. %s\n\n", error, source);
			break;
		case GL_INVALID_OPERATION:
		 LOGW("GL Error (%x): GL_INVALID_OPERATION. %s\n\n", error, source);
			break;
		case GL_STACK_OVERFLOW:
		 LOGW("GL Error (%x): GL_STACK_OVERFLOW. %s\n\n", error, source);
			break;
		case GL_STACK_UNDERFLOW:
		 LOGW("GL Error (%x): GL_STACK_UNDERFLOW. %s\n\n", error, source);
			break;
		case GL_OUT_OF_MEMORY:
		 LOGW("GL Error (%x): GL_OUT_OF_MEMORY. %s\n\n", error, source);
			break;
		default:
		 LOGW("GL Error (%x): %s\n\n", error, source);
			break;
	}
}

void appRender(long tick, int width, int height)
{
    if (sStartTick == 0)
        sStartTick = tick;
    if (!gAppAlive)
        return;

    // Actual tick value is "blurred" a little bit.
    sTick = (sTick + tick - sStartTick) >> 1;

    // Terminate application after running through the demonstration once.
    // if (sTick >= RUN_LENGTH)
    //{
    //  gAppAlive = 0;
    //  return;
    //}
	_glError("appRender");    
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0.0f, sWindowWidth, 
	     0.0f, sWindowHeight, 
	     -100.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	glTranslatef(0.0f,sWindowHeight * 0.5f, 0.0f);
	glColor4f(1.0f, 0.6f, 0.3f, 1.0f);

	

		if (font)
		  font->Render("Hello world!");

	_glError("appRender");
	glPopMatrix();
}
