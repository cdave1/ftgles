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


#import "Delegate.h"
#include "KeyboardDemoController.h"
#include <sys/time.h> 
#include "QuartzCore/QuartzCore.h"

static KeyboardDemoController *keyboardDemoController = NULL;

static int frames;
static CFTimeInterval	CurrentTime;
static CFTimeInterval	LastFPSUpdate;

@implementation AppController

- (void) update
{
	if(keyboardDemoController)
		keyboardDemoController->Draw();
	
	[glView swapBuffers];
	
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
	glView = [[EAGLView alloc] initWithFrame:rect];
	
	[window addSubview:glView];
	[window makeKeyAndVisible];
	
	const char *bundleResourcePath = 
	[[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSASCIIStringEncoding];
	
	float scale = 1.0f;
	
#ifdef __IPHONE_4_0
	if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) 
	{
		scale = [[UIScreen mainScreen] scale];
	}
#endif
	
	keyboardDemoController = new KeyboardDemoController(bundleResourcePath, rect.size.width, rect.size.height, scale);
	
	NSString *reqSysVer = @"3.1";
	NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
	BOOL displayLinkSupported = NO;
	if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
		displayLinkSupported = TRUE;
	
	if (displayLinkSupported)
	{
		id displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(update)];
		[displayLink setFrameInterval:1];
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	}
	else
	{
		CurrentTime = CACurrentMediaTime();
		LastFPSUpdate = CurrentTime;
		[NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0) 
										 target:self 
									   selector:@selector(update) 
									   userInfo:nil 
										repeats:YES];
	}
}


- (void) dealloc
{
	delete keyboardDemoController;
	[glView release];
	[window release];
	[super dealloc];
}


@end
