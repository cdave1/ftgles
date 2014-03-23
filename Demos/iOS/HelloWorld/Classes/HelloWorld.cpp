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

#include "HelloWorld.h"
#include <stdio.h>
#include <FTGL/ftgles.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>


void HelloWorldApp::SetupFonts(const char *fontpath) {
    const float HELLO_WORLD_FONT_SIZE = 72.0f;
    
	m_font = new FTTextureFont(fontpath);
	if (m_font->Error()) {
        fprintf(stderr, "Could not load font at `%s'\n", fontpath);
		delete m_font;
		m_font = NULL;
		return;
	}
    
	m_font->FaceSize(HELLO_WORLD_FONT_SIZE);
}


void HelloWorldApp::SetScreenDimensions(float width, float height, float scale) {
    m_screenWidth = width;
    m_screenHeight = height;
    m_scale = scale;
}


void HelloWorldApp::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(0.0f, m_screenWidth * m_scale,
             0.0f, m_screenHeight * m_scale,
             -10000.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0f, m_screenHeight * 0.5f, 0.0f);
	glColor4f(1.0f, 0.6f, 0.3f, 1.0f);
	if (m_font)
		m_font->Render("Hello world!");
}
