/*
 *  TextureLoader.h
 *  DemoApps
 *
 *  Created by David Petrie on 30/03/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
 */

#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#ifdef __cplusplus
extern "C" {
#endif
	
extern void LoadTexture(const char *filename, GLuint *handle);
	
#ifdef __cplusplus
}
#endif

#endif