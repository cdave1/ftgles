/*
 *  TextureLoader.mm
 *  DemoApps
 *
 *  Created by David Petrie on 30/03/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
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
		spriteData = (GLubyte *) malloc(width * height * 4);
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