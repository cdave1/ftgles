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

#import "TextureLoader.h"
#import <UIKit/UIKit.h>

extern void LoadTexture(const char *filename, GLuint *handle) {
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
	
	if(spriteImage) {
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