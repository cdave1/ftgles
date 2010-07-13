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


#include "HelloWorldDemo.h"

#define HELLO_WORLD_FONT_NAME "Diavlo_BLACK_II_37.otf"

HelloWorldDemo::HelloWorldDemo(const char * path, float width, float height)
{
	screenWidth = width;
	screenHeight = height;
	
	char fontname[256];
	snprintf(fontname, 256, "%s/%s", path, HELLO_WORLD_FONT_NAME);
	font = new FTTextureFont(fontname);
	if (font->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	font->FaceSize(48);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


HelloWorldDemo::~HelloWorldDemo() 
{
	delete font;
}


void HelloWorldDemo::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(0.0f, screenWidth, 0.0f, screenHeight, 10.0f, -10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0f, screenHeight * 0.5f, 0.0f);
	glColor4f(1.0f, 0.6f, 0.3f, 1.0f);
	font->Render("Hello world!");
}

