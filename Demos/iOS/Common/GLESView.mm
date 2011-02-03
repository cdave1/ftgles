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


#import "GLESView.h"
#import "GLES1Surface.h"
#import "GLES2Surface.h"

@implementation GLESView

@synthesize autoresizesSurface = _autoresize,
					surfaceSize=_size;


+ (Class)layerClass
{
    return [CAEAGLLayer class];
}


- (id) initWithFrame:(CGRect)frame
{
	if((self = [super initWithFrame:frame])) 
	{
		CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
		
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:YES], 
										kEAGLDrawablePropertyRetainedBacking, 
										kEAGLColorFormatRGBA8, 
										kEAGLDrawablePropertyColorFormat, 
										nil];
        
		_surface = [[GLES2Surface alloc] init];
		if(!_surface) 
		{
            _surface = [[GLES1Surface alloc] init];			
			if (!_surface)
			{
				[self release];
				return nil;
			}
		}
		
		_surface.delegate = self;
		
#ifdef __IPHONE_4_0
		if([[UIScreen mainScreen] respondsToSelector:@selector(scale)] &&
		   [self respondsToSelector:@selector(contentScaleFactor)]) 
		{
			self.contentScaleFactor = MAX(1.0f, [[UIScreen mainScreen] scale]);
		}
#endif
		
		if (![_surface createSurface])
		{
			[self release];
			return nil;
		}
		
		_size = frame.size;
	}

	return self;
}


- (void) dealloc
{
	[_surface release];
	[super dealloc];
}


- (void) layoutSubviews
{
	CGRect bounds = [self bounds];
	
	if(_autoresize && ((roundf(bounds.size.width) != _size.width) || (roundf(bounds.size.height) != _size.height))) 
	{
		[_surface destroySurface];
		[_surface createSurface];
		_size = bounds.size;
	}
}


- (void) setAutoresizesEAGLSurface:(BOOL)autoresizesEAGLSurface;
{
	_autoresize = autoresizesEAGLSurface;
	if(_autoresize)
		[self layoutSubviews];
}


- (void) swapBuffers
{
	[_surface swapBuffers];
}


- (CAEAGLLayer *) getLayer
{
	return (CAEAGLLayer*)[self layer];
}


- (CGRect) getBounds
{
	return [self bounds];
}



@end
