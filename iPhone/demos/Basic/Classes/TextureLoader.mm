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

#import "TextureLoader.h"
#import <UIKit/UIKit.h>

extern void LoadTexture(const char *filename, GLuint *handle)
{
	CGImageRef spriteImage;
	size_t	width, height;
	CGContextRef spriteContext;
	GLubyte *spriteData;
	
	NSString *fileName = 
		[[NSString alloc] 
		 initWithCString:filename 
		 encoding:NSASCIIStringEncoding];

	spriteImage = [UIImage imageNamed:fileName].CGImage;
	[fileName release];
	
	width = CGImageGetWidth(spriteImage);
	height = CGImageGetHeight(spriteImage);
	
	if(spriteImage) 
	{
		spriteData = (GLubyte *)calloc(1, width * height * 4);
		spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width * 4, 
											  CGImageGetColorSpace(spriteImage), 
											  kCGImageAlphaPremultipliedLast);
		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
		CGContextRelease(spriteContext);
		glGenTextures(1, handle);
		glBindTexture(GL_TEXTURE_2D, *handle);		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
		glGenerateMipmapOES(GL_TEXTURE_2D);
		free(spriteData);		
	}
}