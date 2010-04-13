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
 * Demo iPhone app showing ftgles in action.
 */

#include "ViewController.h"
#include "TextureLoader.h"

static FTFont *fonts[3];

static FTSimpleLayout layouts[3];

static GLuint aTexture;

static int renderMode = FTGL::RENDER_FRONT | FTGL::RENDER_BACK;

#define MAX_VERTEX_COUNT 16

typedef struct DemoVertex 
{
	float xyz[3];
	float st[2];
	float rgba[4];
} DemoVertex;

static DemoVertex vertices[MAX_VERTEX_COUNT];
static DemoVertex vertex;
static GLenum currentPrimitive = GL_TRIANGLES;
static int vertexCount = 0;

void demoGlBegin(GLenum prim)
{
	currentPrimitive = prim;
	vertexCount = 0;
	glVertexPointer(3, GL_FLOAT, sizeof(DemoVertex), vertices[0].xyz);
	glTexCoordPointer(2, GL_FLOAT, sizeof(DemoVertex), vertices[0].st);
	glColorPointer(4, GL_FLOAT, sizeof(DemoVertex), vertices[0].rgba);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}


void demoGlVertex3f(float x, float y, float z)
{
	if (vertexCount > MAX_VERTEX_COUNT) return;
	vertex.xyz[0] = x;
	vertex.xyz[1] = y;
	vertex.xyz[2] = z;
	vertices[vertexCount] = vertex;
	vertexCount++;
}


void demoGlColor4f(float r, float g, float b, float a)
{
	vertex.rgba[0] = r;
	vertex.rgba[1] = g;
	vertex.rgba[2] = b;
	vertex.rgba[3] = a;
}


void demoGlTexCoord2f(float s, float t)
{
	vertex.st[0] = s;
	vertex.st[1] = t;
}


void demoGlEnd()
{
	if (vertexCount == 0) 
	{
		currentPrimitive = 0;
		return;
	}
	glDrawArrays(currentPrimitive, 0, vertexCount);
	vertexCount = 0;
	currentPrimitive = 0;
}


void demoGlError(const char *source)
{
}


const char *polygonFontText = "This is a polygon font";
const char *textureFontText = "Lorem ipsum dolor sit amet.";
const char *outlineFontText = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, \
sed do eiusmod tempor incididunt. Lorem ipsum dolor sit amet, consectetur adipisicing elit.\
sed do eiusmod tempor incididunt. Lorem ipsum dolor sit amet, consectetur adipisicing elit.";


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
	fonts[0]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	layouts[0].SetLineLength(320.0f);
	layouts[0].SetLineSpacing(0.75f);
    layouts[0].SetFont(fonts[0]);
	layouts[0].SetAlignment(FTGL::ALIGN_LEFT);
	
	snprintf(fontname, 256, "%s/RosewoodStd-Regular.otf", path);
	fonts[1] = new FTTextureFont(fontname);
	if (fonts[1]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[1]->FaceSize(48);
	fonts[1]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	layouts[1].SetLineLength(320.0f);
	layouts[1].SetLineSpacing(0.75f);
    layouts[1].SetFont(fonts[1]);
	layouts[1].SetAlignment(FTGL::ALIGN_CENTER);
	
	snprintf(fontname, 256, "%s/Cardo98s.ttf", path);
	fonts[2] = new FTTextureFont(fontname);
	if (fonts[2]->Error())
	{
        printf("Could not load font `%s'\n", fontname);	
	}
	fonts[2]->FaceSize(16);
	fonts[2]->CharMap(FT_ENCODING_ADOBE_LATIN_1);
	
	layouts[2].SetLineLength(300.0f);
	layouts[2].SetLineSpacing(0.75f);
    layouts[2].SetFont(fonts[2]);
	layouts[2].SetAlignment(FTGL::ALIGN_RIGHT);
	
	LoadTexture("mousePoint.png", &aTexture);
	
	printf("Loaded texture: %d\n", aTexture);
}


ViewController::~ViewController() 
{
	delete fonts[0];
	delete fonts[1];
	delete fonts[2];
}


void ViewController::DrawTexturedQuad()
{
	demoGlBegin(GL_TRIANGLE_STRIP);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, aTexture);
	
	demoGlVertex3f(-40.0f, -40.0f, 0.0f);
	demoGlColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	demoGlTexCoord2f(0, 1);
	
	demoGlVertex3f(-40.0f, 40.0f, 0.0f);
	demoGlColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	demoGlTexCoord2f(1, 0);
	
	demoGlVertex3f(40.0f, -40.0f, 0.0f);
	demoGlColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	demoGlTexCoord2f(0, 0);
	
	demoGlVertex3f(40.0f, 40.0f, 0.0f);
	demoGlColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	demoGlTexCoord2f(1, 1);
	
	demoGlEnd();
}


static float angle = 0.0f;
static float faceSize = 40.0f;
void ViewController::Draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glPushMatrix();
	glOrthof(0.0f, 320.0f, 
			 0.0f, 480.0f, 
			 1000.0f, -1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(160.0f, 240.0f, 0.0f);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	
	glPushMatrix();	
	glRotatef(angle, 1.0f, 1.0f, 1.0f);
	glTranslatef(-160.0f, -240.0f, 0.0f);
	
	glPushMatrix();
	glTranslatef(0.0f, 420.0f, 0.0f);
	glColor4f(6.0f, 0.6f, 0.3f, 1.0f);
	layouts[0].Render(polygonFontText, -1, FTPoint(), renderMode);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10.0f, 280.0f, 0.0f);
	glColor4f(0.4f, 0.4f, 0.0f, 1.0f);
	layouts[1].Render(textureFontText, -1, FTPoint(), renderMode);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10.0f, 140.0f, 0.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	layouts[2].Render(outlineFontText, -1, FTPoint(), renderMode);
	glPopMatrix();
	
	glPopMatrix();
	
	//DrawTexturedQuad();
	
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	angle += 0.5f;
	if (angle >= 360.0f) angle = 0.0f;
	
	//fonts[1]->FaceSize(faceSize);
	//faceSize -= 1.0f;
	//if (faceSize < 10) faceSize = 120;
}

