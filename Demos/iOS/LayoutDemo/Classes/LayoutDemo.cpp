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

/**
 * Demo iPhone app showing ftgles in action.
 */

#include "LayoutDemo.h"
#include "TextureLoader.h"
#include <string>

static FTFont *fpsFont;
static char fpsText[32];
static FTFont *fonts[4];
static FTSimpleLayout layouts[4];
static GLuint aTexture;
static float screenWidth, screenHeight, contentScaleFactor;

LayoutDemo::LayoutDemo(const char* path, float width, float height, float scale) {
	contentScaleFactor = scale;
	screenWidth = contentScaleFactor * width;
	screenHeight = contentScaleFactor * height;
	
	char fontname[256];

     const char * fontFiles[] = {
        "Brandon_thin.otf",
        "Brandon_light.otf",
        "Brandon_reg.otf",
        "Brandon_med.otf",
        "Brandon_bld.otf",
        "Brandon_blk.otf"
    };
	
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	snprintf(fontname, 256, "%s/TOONISH.ttf", path);
	fpsFont = new FTTextureFont(fontname);
	fpsFont->FaceSize(contentScaleFactor * 15);
	fpsFont->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	snprintf(fpsText, 32, "FPS: 0");
	
	snprintf(fontname, 256, "%s/%s", path, fontFiles[5]);
	fonts[0] = new FTTextureFont(fontname);
	if(fonts[0]->Error())
    {
        printf("Could not load font `%s'\n", fontname);
    }
	fonts[0]->FaceSize(contentScaleFactor * 84.0);
	fonts[0]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	layouts[0].SetLineLength(screenWidth);
	layouts[0].SetLineSpacing(0.75f);
    layouts[0].SetFont(fonts[0]);
	layouts[0].SetAlignment(FTGL::ALIGN_CENTER);
	
	snprintf(fontname, 256, "%s/%s", path, fontFiles[1]);
	fonts[1] = new FTTextureFont(fontname);
	if (fonts[1]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[1]->FaceSize(contentScaleFactor * 36.0);
	fonts[1]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	layouts[1].SetLineLength((screenWidth * 0.5f) - 50.0f);
	layouts[1].SetLineSpacing(0.75f);
    layouts[1].SetFont(fonts[1]);
	layouts[1].SetAlignment(FTGL::ALIGN_LEFT);

    snprintf(fontname, 256, "%s/%s", path, fontFiles[3]);
    fonts[2] = new FTTextureFont(fontname);
    if (fonts[2]->Error())
    {
        printf("Could not load font `%s'\n", fontname);
    }
    fonts[2]->FaceSize(contentScaleFactor * 36.0);
    fonts[2]->CharMap(FT_ENCODING_ADOBE_LATIN_1);

    layouts[2].SetLineLength((screenWidth * 0.5f) - 50.0f);
    layouts[2].SetLineSpacing(0.75f);
    layouts[2].SetFont(fonts[2]);
    layouts[2].SetAlignment(FTGL::ALIGN_RIGHT);
	
	snprintf(fontname, 256, "%s/%s", path, fontFiles[2]);
	fonts[3] = new FTTextureFont(fontname);
	if (fonts[3]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[3]->FaceSize(contentScaleFactor * 28);
	fonts[3]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	layouts[3].SetLineLength(screenWidth - 50.0f);
	layouts[3].SetLineSpacing(0.75f);
    layouts[3].SetFont(fonts[3]);
	layouts[3].SetAlignment(FTGL::ALIGN_JUSTIFY);
	
	printf("Loaded texture: %d\n", aTexture);
}


LayoutDemo::~LayoutDemo() {
	delete fonts[0];
	delete fonts[1];
	delete fonts[2];
}


void LayoutDemo::SetFPS(const unsigned int fps) {
	snprintf(fpsText, 32, "FPS: %d", fps);
}


static float color = 0.0f;
void LayoutDemo::Draw() {
	float halfScreenWidth = screenWidth * 0.5f;
	float halfScreenHeight = screenHeight * 0.5f;
	
	glClearColor(0.92f, 0.92f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glPushMatrix();
	glOrthof(0.0f, screenWidth, 
			 0.0f, screenHeight, 
			 -1000.0f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(halfScreenWidth, halfScreenHeight, 0.0f);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	glPushMatrix();
	glTranslatef(-halfScreenWidth, -halfScreenHeight, 0.0f);

    std::string polygonFontText = "FTGLES Layout Demo";
    std::string leftParagraph = "(Left aligned) Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
    std::string rightParagraph = "(Right aligned) Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
    std::string outlineFontText = "(Justify aligned) Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	glPushMatrix();
	glTranslatef(0.0f, contentScaleFactor * 620.0f, 0.0f);
	glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
	layouts[0].Render(polygonFontText.c_str(), -1, FTPoint(), FTGL::RENDER_FRONT);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(25.0f, contentScaleFactor * 500.0f, 0.0f);
	glColor4f(0.2f, 0.4f, 0.75f, 1.0f);
	layouts[1].Render(leftParagraph.c_str(), -1, FTPoint(), FTGL::RENDER_FRONT);
	glPopMatrix();

    glPushMatrix();
    glTranslatef((screenWidth * 0.5f) + 25.0f, contentScaleFactor * 500.0f, 0.0f);
    glColor4f(0.2f, 0.4f, 0.75f, 1.0f);
    layouts[2].Render(rightParagraph.c_str(), -1, FTPoint(), FTGL::RENDER_FRONT);
    glPopMatrix();
	
	glPushMatrix();
	glTranslatef(25.0f, contentScaleFactor * 240.0f, 0.0f);
	glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
	layouts[3].Render(outlineFontText.c_str(), -1, FTPoint(), FTGL::RENDER_FRONT);
	glPopMatrix();
	
	glPopMatrix();
	
	
	// Show the fps
	glPushMatrix();
	glColor4f(1.0f, 0.8f, 0.0f, 1.0f);
	glTranslatef(-halfScreenWidth, -halfScreenHeight, 0.0f);
	fpsFont->Render(fpsText);
	glPopMatrix();
	
	
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	color += 0.01f;
	if (color > 1.0f) color = 0.0f;
}

