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
#include "RenderController.h"


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
	
	polygonFont = new FTPolygonFont([fontpath UTF8String]);
    assert (!polygonFont->Error());
	polygonFont->FaceSize(screenSize.width * 0.16f);
    
    textureFont = new FTTextureFont([fontpath UTF8String]);
	assert (!textureFont->Error());
	textureFont->FaceSize(screenSize.width * 0.24f);
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


- (void)SetupGL
{
    shaderProgram = glCreateProgram();
    
    GLuint vertexShader = [self compileShader:[[NSBundle mainBundle] pathForResource:@"vertex" ofType:@"vsh"] withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:[[NSBundle mainBundle] pathForResource:@"fragment" ofType:@"fsh"] withType:GL_FRAGMENT_SHADER];
    assert(vertexShader && fragmentShader);
    
    glBindAttribLocation(shaderProgram, RENDER_ATTRIB_VERTEX, "position");
    glBindAttribLocation(shaderProgram, RENDER_ATTRIB_COLOR, "color");
    
    glLinkProgram(shaderProgram);
    
    cameraUniform = glGetUniformLocation(shaderProgram, "camera");
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    aglOrtho(cameraMatrix, -screenSize.width, screenSize.width, -screenSize.height, screenSize.height, -10000.0f, 10000.0f);
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
    
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, cameraMatrix);
    
	//if (polygonFont)
	//	polygonFont->Render("Hello world!");
  
    if (textureFont)
		textureFont->Render("Hello world!");
	
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
	glView = [[GLESView alloc] initWithFrame:rect renderingAPI:kEAGLRenderingAPIOpenGLES2];
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
	delete textureFont;
    delete polygonFont;
	[glView release];
	[window release];
	[super dealloc];
}


@end
