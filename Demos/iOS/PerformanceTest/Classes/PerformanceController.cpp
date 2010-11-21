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

#include "PerformanceController.h"
#include "TextureLoader.h"

typedef struct
{
	FTSimpleLayout layout;
	float shade;
	short shadeDir;
} coloredLayout_t;


const unsigned int kLayoutCount = 12;

static FTFont *fpsFont;
char fpsText[32];
static FTFont *fonts[3];

static coloredLayout_t layouts[kLayoutCount];

static GLuint aTexture;

static float screenWidth, screenHeight, contentScaleFactor;


const char *polygonFontText = "This is a polygon font";
const char *textureFontText = "Lorem ipsum dolor sit amet.";
const char *longText1 = "Lorem  ipsum  dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
const char *longText2 = "New Text! Lorem  ipsum  dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Lorem  ipsum  dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

const char *txtA = "Lorem  ipsum  dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ";
const char *txtB = "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor.";
const char *txtC = "in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, ";
const char *txtD = "sunt in culpa qui officia deserunt mollit anim id est laborum.";

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)
#define CLAMP(a,min,max) MIN(max, MAX(min,a))

PerformanceController::PerformanceController(const char* path, float width, float height, float scale)
{
	contentScaleFactor = scale;
	screenWidth = contentScaleFactor * width;
	screenHeight = contentScaleFactor * height;
	
	char fontname[256];
	
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	snprintf(fontname, 256, "%s/TOONISH.ttf", path);
	fpsFont = new FTTextureFont(fontname);
	fpsFont->FaceSize(contentScaleFactor * 15);
	fpsFont->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	snprintf(fpsText, 32, "FPS: 0");
	
	snprintf(fontname, 256, "%s/Diavlo_BLACK_II_37.otf", path);
#if 0
	// Still very slow: polygon fonts.  Not sure how to get around this -- could
	// possibly group mesh polygons by type and render them all at the same time
	// (goal is to minimise calls to ftglEnd()).
	fonts[0] = new FTPolygonFont(fontname);
#else
	fonts[0] = new FTTextureFont(fontname);
#endif
	if(fonts[0]->Error())
    {
        printf("Could not load font `%s'\n", fontname);
    }
	fonts[0]->FaceSize(contentScaleFactor * 15);
	fonts[0]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	snprintf(fontname, 256, "%s/TOONISH.ttf", path);
	fonts[1] = new FTTextureFont(fontname);
	if (fonts[1]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[1]->FaceSize(contentScaleFactor * 15);
	fonts[1]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	snprintf(fontname, 256, "%s/Cardo98s.ttf", path);
	fonts[2] = new FTTextureFont(fontname);
	if (fonts[2]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[2]->FaceSize(contentScaleFactor * 15);
	fonts[2]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	for (unsigned i = 0; i < kLayoutCount; ++i)
	{
		layouts[i].shade = (float)i/kLayoutCount;
		layouts[i].shadeDir = layouts[i].shade > 0.5f;
		layouts[i].layout.SetLineLength(screenWidth);
		layouts[i].layout.SetLineSpacing(0.75f);
		layouts[i].layout.SetFont(fonts[rand() % 3]);
		layouts[i].layout.SetAlignment(FTGL::ALIGN_JUSTIFY);
	}
	
	printf("Loaded texture: %d\n", aTexture);
}


PerformanceController::~PerformanceController() 
{
	delete fonts[0];
	delete fonts[1];
	delete fonts[2];
}


void PerformanceController::DrawNonLayoutText(const float shade, const float yLoc)
{
	unsigned fontIdx = rand() % 3; // Jitter fonts :)
	glColor4f(CLAMP(shade, 0.0f, 1.0f), CLAMP(shade, 0.0f, 1.0f), CLAMP(shade, 0.0f, 1.0f), 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, yLoc, 0.0f);
	fonts[fontIdx]->Render(txtA);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f,yLoc-20.0f, 0.0f);
	fonts[fontIdx]->Render(txtB);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f, yLoc-40.0f, 0.0f);
	fonts[fontIdx]->Render(txtC);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f, yLoc - 60.0f, 0.0f);
	fonts[fontIdx]->Render(txtD);
	glPopMatrix();
}



void PerformanceController::DrawLayoutText(FTSimpleLayout *layout, const float shade, const float yLoc)
{
	glColor4f(1.0f, 1.0f, 1.0f, CLAMP(shade, 0.1f, 1.0f));
	glPushMatrix();
	glTranslatef(0.0f, yLoc, 0.0f);
	if (shade < 0.5f)
		layout->Render(longText1, -1, FTPoint(), FTGL::RENDER_FRONT);
	else 
		layout->Render(longText2, -1, FTPoint(), FTGL::RENDER_FRONT);
	glPopMatrix();
}


void PerformanceController::SetFPS(const unsigned int fps)
{
	snprintf(fpsText, 32, "FPS: %d", fps);
}


static float angle = 0.0f;
void PerformanceController::Draw()
{
	float halfScreenWidth = screenWidth * 0.5f;
	float halfScreenHeight = screenHeight * 0.5f;
	
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glPushMatrix();
	glOrthof(0.0f, screenWidth, 
			 0.0f, screenHeight, 
			 -1000000.0f, 1000000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(halfScreenWidth, halfScreenHeight, 0.0f);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	glPushMatrix();
	glTranslatef(-halfScreenWidth, -halfScreenHeight, 0.0f);

#if 0	
	
	DrawNonLayoutText(0.0f, contentScaleFactor * 940.0f);
	DrawNonLayoutText(0.1f, contentScaleFactor * 890.0f);
	DrawNonLayoutText(0.2f, contentScaleFactor * 840.0f);
	DrawNonLayoutText(0.3f, contentScaleFactor * 790.0f);
	DrawNonLayoutText(0.4f, contentScaleFactor * 740.0f);
	DrawNonLayoutText(0.5f, contentScaleFactor * 690.0f);
	DrawNonLayoutText(0.6f, contentScaleFactor * 640.0f);
	DrawNonLayoutText(0.7f, contentScaleFactor * 590.0f);
	DrawNonLayoutText(0.8f, contentScaleFactor * 540.0f);
	DrawNonLayoutText(0.0f, contentScaleFactor * 490.0f);
	DrawNonLayoutText(0.1f, contentScaleFactor * 440.0f);
	DrawNonLayoutText(0.2f, contentScaleFactor * 390.0f);
	DrawNonLayoutText(0.3f, contentScaleFactor * 340.0f);
	DrawNonLayoutText(0.4f, contentScaleFactor * 290.0f);
	DrawNonLayoutText(0.5f, contentScaleFactor * 240.0f);
	DrawNonLayoutText(0.6f, contentScaleFactor * 190.0f);
	DrawNonLayoutText(0.7f, contentScaleFactor * 140.0f);
	DrawNonLayoutText(0.8f, contentScaleFactor * 90.0f);
	
#else
	float yPos = screenHeight - 20.0f;
	for (unsigned i = 0; i < kLayoutCount; ++i)
	{	
		DrawLayoutText(&layouts[i].layout, layouts[i].shade, contentScaleFactor * yPos);
		
		if (layouts[i].shadeDir == 0)
			layouts[i].shade += 0.01f;
		else 
			layouts[i].shade -= 0.01f;
		if (layouts[i].shade >= 1.0f) layouts[i].shadeDir = 1;
		else if (layouts[i].shade <= 0.0f) layouts[i].shadeDir = 0;
		
		yPos -= (screenHeight / kLayoutCount);
	}
	
#endif
	

	
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
}

