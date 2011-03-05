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

#include "GLES2Surface.h"

@implementation GLES2Surface


@synthesize framebuffer = _framebuffer,
pixelFormat = _format,
depthFormat = _depthFormat;

- (id) init
{
    return [self initWithPixelFormat:GL_RGB565 
                         depthFormat:GL_DEPTH_COMPONENT16 
                  preserveBackbuffer:NO];
}


- (id) initWithPixelFormat:(GLuint)format 
{
    return [self initWithPixelFormat:format 
                         depthFormat:0 
                  preserveBackbuffer:NO];
}


- (id) initWithPixelFormat:(GLuint)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained
{
    if((self = [super init])) 
    {
        _format = format;
        _depthFormat = depth;
        
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        if(!_context || ![EAGLContext setCurrentContext:_context]) 
        {
            [self release];
            return nil;
        }
    }
    
    return self;
}


- (void) dealloc
{
    [self destroySurface];
    
    [_context release];
    _context = nil;
    
    [super dealloc];
}


- (BOOL)createSurface
{
    CAEAGLLayer *eaglLayer;
    CGSize newSize;
    GLuint oldRenderbuffer;
    GLuint oldFramebuffer;
    
    if(![EAGLContext setCurrentContext:_context]) 
    {
        return NO;
    }
    
    if (!_delegate)
    {
        return NO;
    }
    
    eaglLayer = [_delegate getLayer];
    
    newSize = [eaglLayer bounds].size;
#ifdef __IPHONE_4_0
    if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) 
    {
        newSize.width = roundf(newSize.width * MAX(1.0f, [[UIScreen mainScreen] scale]));
        newSize.height = roundf(newSize.height * MAX(1.0f, [[UIScreen mainScreen] scale]));
    } 
    else 
    {
        newSize.width = roundf(newSize.width);
        newSize.height = roundf(newSize.height);
    }
#else 
    newSize.width = roundf(newSize.width);
    newSize.height = roundf(newSize.height);
#endif
    
    glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint *) &oldRenderbuffer);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &oldFramebuffer);
    
    glGenRenderbuffers(1, &_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    
    if(![_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)eaglLayer]) 
    {
        glDeleteRenderbuffers(1, &_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER_BINDING, oldRenderbuffer);
        return NO;
    }
    
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderbuffer);
    if (_depthFormat) 
    {
        glGenRenderbuffers(1, &_depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, _depthFormat, newSize.width, newSize.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    }
    
    if(!_hasBeenCurrent) 
    {
        glViewport(0, 0, newSize.width, newSize.height);
        glScissor(0, 0, newSize.width, newSize.height);
        _hasBeenCurrent = YES;
    }
    else 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, oldFramebuffer);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, oldRenderbuffer);
    
    return YES;
}


- (void) destroySurface
{
    EAGLContext *oldContext = [EAGLContext currentContext];
    
    if (oldContext != _context)
        [EAGLContext setCurrentContext:_context];
    
    if(_depthFormat) 
    {
        glDeleteRenderbuffers(1, &_depthBuffer);
        _depthBuffer = 0;
    }
    
    glDeleteRenderbuffers(1, &_renderbuffer);
    _renderbuffer = 0;
    
    glDeleteFramebuffers(1, &_framebuffer);
    _framebuffer = 0;
    
    if (oldContext != _context)
        [EAGLContext setCurrentContext:oldContext];
}


- (void) bindFrameBuffer
{
    CGRect _bounds = [_delegate getBounds];
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glViewport(0, 0, _bounds.size.width, _bounds.size.height);
}


- (void) bindRenderBufferAndPresent
{
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}


- (void) swapBuffers
{
    EAGLContext *oldContext = [EAGLContext currentContext];
    GLuint oldRenderbuffer;
    
    if(oldContext != _context)
    {
        [EAGLContext setCurrentContext:_context];
    }
    
    // CHECK_GL_ERROR();
    
    glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint *) &oldRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    
    if(![_context presentRenderbuffer:GL_RENDERBUFFER])
    {
        printf("Failed to swap renderbuffer in %s\n", __FUNCTION__);
    }
    
    if(oldContext != _context)
    {
        [EAGLContext setCurrentContext:oldContext];
    }
}

@end
