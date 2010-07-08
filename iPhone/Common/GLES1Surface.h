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

#import "GLESSurface.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface GLES1Surface : GLESSurface <GLESAbstractSurface>
{
@private
	GLuint _format;
	GLuint _depthFormat;
	GLuint _framebuffer;
	GLuint _renderbuffer;
	GLuint _depthBuffer;
}

- (id) init;
- (id) initWithPixelFormat:(GLuint)format;
- (id) initWithPixelFormat:(GLuint)format 
			   depthFormat:(GLuint)depth 
		preserveBackbuffer:(BOOL)retained;

@property(readonly) GLuint framebuffer;
@property(readonly) GLuint pixelFormat;
@property(readonly) GLuint depthFormat;

@end