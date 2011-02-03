/*
 
 Copyright (c) 2010 David Petrie david@davidpetrie.com
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the 
 use of this software. Permission is granted to anyone to use this software for
 any purpose, including commercial applications, and to alter it and 
 redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim 
 that you wrote the original software. If you use this software in a product, an 
 acknowledgment in the product documentation would be appreciated but is not 
 required.
 2. Altered source versions must be plainly marked as such, and must not be 
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 
 */


#import "Delegate.h"
#include <sys/time.h> 
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "QuartzCore/QuartzCore.h"


@interface AppController ()
- (void) SetupFonts;
- (void) Render;
- (void) ShowFPS;
- (GLuint)compileShader:(NSString *)filePath withType:(GLenum)type;
- (void) SetupGL;
@end


static GLuint shaderProgram = 0;
static GLuint cameraUniform;

static float cameraMatrix[16];



@implementation AppController

- (void) SetupFonts
{
	NSString *fontpath = [NSString stringWithFormat:@"%@/Diavlo_BLACK_II_37.otf", 
						  [[NSBundle mainBundle] resourcePath]];
	
	font = new FTPolygonFont([fontpath UTF8String]); //FTTextureFont([fontpath UTF8String]);
	if (font->Error())
	{
        NSLog(@"Could not load font `%@'\n", fontpath);	
		delete font;
		font = NULL;
		return;
	}
	font->FaceSize(screenSize.width * 0.16f);
}


- (GLuint)compileShader:(NSString *)filePath withType:(GLenum)type
{
    assert(shaderProgram);
    
    GLuint shader;
    GLint status;
    const GLchar *source;
	
    if (!(source = (GLchar *)[[NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil] UTF8String]))
    {
        NSLog(@"Failed to load vertex shader");
        return FALSE;
    }
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    assert(status != 0);

    glAttachShader(shaderProgram, shader);
    return shader;
}


void CameraOrtho(float *mOut, float left, float right, float bottom, float top, float zNear, float zFar)
{
    bzero(mOut, sizeof(float) * 16);
    
    if (right != left)
    {
        mOut[ 0] = 2 / (right - left);
        mOut[ 1] = 0;
        mOut[ 2] = 0;
        mOut[ 3] = - ((right + left) / (right - left));
    }
    
    if (top != bottom)
    {
        mOut[ 4] = 0;
        mOut[ 5] = 2 / (top - bottom);
        mOut[ 6] = 0;
        mOut[ 7] = - ((top + bottom) / (top - bottom));
    }
	
    if (zFar != zNear)
    {
        mOut[ 8] = 0;
        mOut[ 9] = 0;
        mOut[10] = -2 / (zFar - zNear);
        mOut[11] = - ((zFar + zNear) / (zFar - zNear));
    }
	
	mOut[12] = 0;
	mOut[13] = 0;
	mOut[14] = 0;
	mOut[15] = 1;
}


- (void)SetupGL
{
    shaderProgram = glCreateProgram();
    
    GLuint vertexShader = [self compileShader:[[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"] withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:[[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"] withType:GL_FRAGMENT_SHADER];
    assert(vertexShader && fragmentShader);
    
    glLinkProgram(shaderProgram);
    
    // Get uniform locations
    //uniformIndices[ROT_MATRIX] = glGetUniformLocation(shaderProgram, "rotation");
    //uniformIndices[CAMERA_MATRIX] = glGetUniformLocation(shaderProgram, "camera");
    //uniformIndices[TRANSLATION_UNIFORM] = glGetUniformLocation(shaderProgram, "translation");
    
    cameraUniform = glGetUniformLocation(shaderProgram, "camera");
    
    
    // Release vertex and fragment shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    CameraOrtho(cameraMatrix, -screenSize.width, screenSize.width, -screenSize.height, screenSize.height, -10000.0f, 10000.0f);
    
   // ftglSetCamera();
    
    printf("\n");
}


- (void) Render
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    GLenum error = glGetError();
    
	switch (error) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("GL Error (%x): GL_INVALID_ENUM\n\n", error);
			break;
		case GL_INVALID_VALUE:
			printf("GL Error (%x): GL_INVALID_VALUE.\n\n", error);
			break;
		case GL_INVALID_OPERATION:
			printf("GL Error (%x): GL_INVALID_OPERATION.\n\n", error);
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL Error (%x): GL_OUT_OF_MEMORY.\n\n", error);
			break;
		default:
			printf("GL Error (%x):\n\n", error);
			break;
	}
    glUseProgram(shaderProgram);
    
   // glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, cameraMatrix);
    
    

	//glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
	//glOrthof(0.0f, screenSize.width, 0.0f, screenSize.height, -10000.0f, 10000.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	
	//glTranslatef(0.0f, screenSize.height * 0.5f, 0.0f);
	//glColor4f(1.0f, 0.6f, 0.3f, 1.0f);
	if (font)
		font->Render("Hello world!");
	
	[glView swapBuffers];
	[self ShowFPS];
}


- (void) ShowFPS
{
	++frames;
	CurrentTime = CACurrentMediaTime();
	
	if ((CurrentTime - LastFPSUpdate) > 1.0f)
	{ 
		printf("fps: %d\n", frames);		
		frames = 0;
		LastFPSUpdate = CurrentTime;
	} 
}


- (void) applicationDidFinishLaunching:(UIApplication*)application
{
	CGRect	rect = [[UIScreen mainScreen] bounds];
	window = [[UIWindow alloc] initWithFrame:rect];
	glView = [[GLESView alloc] initWithFrame:rect];
	screenSize = rect.size;
	
	[window addSubview:glView];
	[window makeKeyAndVisible];
	
	float scale = 1.0f;
	
#ifdef __IPHONE_4_0
	if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) 
	{
		scale = [[UIScreen mainScreen] scale];
	}
#endif
	
	screenSize.width *= scale;
	screenSize.height *= scale;
	
	[self SetupFonts];
    [self SetupGL];
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	NSString *reqSysVer = @"3.1";
	NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
	BOOL displayLinkSupported = NO;
	if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
		displayLinkSupported = TRUE;
	
	if (displayLinkSupported)
	{
		id displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(Render)];
		[displayLink setFrameInterval:1];
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	}
	else
	{
		CurrentTime = CACurrentMediaTime();
		LastFPSUpdate = CurrentTime;
		[NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0) 
										 target:self 
									   selector:@selector(Render) 
									   userInfo:nil 
										repeats:YES];
	}
}


- (void) dealloc
{
	delete font;
	[glView release];
	[window release];
	[super dealloc];
}


@end
