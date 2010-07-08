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


#import "EAGLView.h"
#import "GLES1Surface.h"
#import "GLES2Surface.h"

@implementation EAGLView

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
		
		_surface = [[GLES1Surface alloc] init];
		if(!_surface) 
		{
			_surface = [[GLES2Surface alloc] init];
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
