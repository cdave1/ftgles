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
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "QuartzCore/QuartzCore.h"


@interface AppController ()
- (void) SetupFonts;
- (void) Render;
- (void) ShowFPS;
@end


@implementation AppController

- (void) SetupFonts
{
	NSString *fontpath = [NSString stringWithFormat:@"%@/Diavlo_BLACK_II_37.otf", 
						  [[NSBundle mainBundle] resourcePath]];
	
	font = new FTTextureFont([fontpath UTF8String]);
	if (font->Error())
	{
        NSLog(@"Could not load font `%@'\n", fontpath);	
		delete font;
		font = NULL;
		return;
	}
	font->FaceSize(screenSize.width * 0.16f);
}


- (void) Render
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(0.0f, screenSize.width, 0.0f, screenSize.height, -10000.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0f, screenSize.height * 0.5f, 0.0f);
	glColor4f(1.0f, 0.6f, 0.3f, 1.0f);
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
