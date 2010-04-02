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
 
/**
 * Demo showing the keyboard to edit a string rendered in a font via FTGLES.
 * NOT: NOT FINISHED.
 */

#include "ViewController.h"
#include "TextureLoader.h"

static FTFont *fonts[3];

static FTSimpleLayout layouts[3];

static GLuint aTexture;

static int renderMode = FTGL::RENDER_FRONT | FTGL::RENDER_BACK;


ViewController::ViewController(const char* path)
{
	char fontname[256];
	
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	snprintf(fontname, 256, "%s/Diavlo_BLACK_II_37.otf", path);
	fonts[0] = new FTPolygonFont(fontname);
	if(fonts[0]->Error())
    {
        printf("Could not load font `%s'\n", fontname);
    }
	fonts[0]->FaceSize(40);
	fonts[0]->CharMap(ft_encoding_unicode);
	
	layouts[0].SetLineLength(320.0f);
	layouts[0].SetLineSpacing(0.75f);
    layouts[0].SetFont(fonts[0]);
	layouts[0].SetAlignment(FTGL::ALIGN_CENTER);
	
	snprintf(fontname, 256, "%s/RosewoodStd-Regular.otf", path);
	fonts[1] = new FTTextureFont(fontname);
	if (fonts[1]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[1]->FaceSize(48);
	fonts[1]->CharMap(ft_encoding_unicode);
	
	layouts[1].SetLineLength(320.0f);
	layouts[1].SetLineSpacing(0.75f);
    layouts[1].SetFont(fonts[1]);
	layouts[1].SetAlignment(FTGL::ALIGN_CENTER);
	
	snprintf(fontname, 256, "%s/BorisBlackBloxx.ttf", path);
	fonts[2] = new FTTextureFont(fontname);
	if (fonts[2]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[2]->FaceSize(20);
	fonts[2]->CharMap(ft_encoding_unicode);
	
	layouts[2].SetLineLength(320.0f);
	layouts[2].SetLineSpacing(0.75f);
    layouts[2].SetFont(fonts[2]);
	layouts[2].SetAlignment(FTGL::ALIGN_CENTER);
	
	LoadTexture("mousePoint.png", &aTexture);
	
	printf("Loaded texture: %d\n", aTexture);
}


ViewController::~ViewController() 
{
	delete fonts[0];
	delete fonts[1];
	delete fonts[2];
}


static float angle = 0.0f;
void ViewController::Draw()
{
	ftglInitImmediateModeGL();
	
	glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	glPushMatrix();
	glOrthof(0.0f, 320.0f, 
			 0.0f, 480.0f, 
			 1000.0f, -1000.0f);
	
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	
	glTranslatef(160.0f, 240.0f, 0.0f);
	
	glPushMatrix();
	
	glRotatef(angle, 1.0f, 1.0f, 1.0f);
	angle += 0.5f;
	if (angle >= 360.0f) angle = 0.0f;
	
	glTranslatef(-160.0f, -240.0f, 0.0f);
	
	glPushMatrix();
	glTranslatef(0.0f, 420.0f, 0.0f);
	glColor4f(1.0f, 0.6f, 0.3f, 1.0f);
	layouts[0].Render("FTGL ES on an iPhone!", -1, FTPoint(), renderMode);
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, aTexture);
	
	
	glPushMatrix();
	glTranslatef(10.0f, 280.0f, 0.0f);
	glColor4f(0.4f, 0.8f, 0.0f, 1.0f);
	layouts[1].Render("Lorem ipsum dolor sit amet.", -1, FTPoint(), renderMode);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10.0f, 140.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 0.75f, 1.0f);
	layouts[2].Render("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.", -1, FTPoint(), renderMode);
	glPopMatrix();
	
	glPopMatrix();
	
	glPopMatrix();
	
	glDisable(GL_BLEND);
	
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

