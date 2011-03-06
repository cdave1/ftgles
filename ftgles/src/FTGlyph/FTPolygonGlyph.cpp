/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "config.h"

#include "FTGL/ftgles.h"

#include "FTInternals.h"
#include "FTPolygonGlyphImpl.h"
#include "FTVectoriser.h"


//
//  FTGLPolyGlyph
//


FTPolygonGlyph::FTPolygonGlyph(FT_GlyphSlot glyph, float outset,
                               bool useDisplayList) :
    FTGlyph(new FTPolygonGlyphImpl(glyph, outset, useDisplayList))
{}


FTPolygonGlyph::~FTPolygonGlyph()
{}


const FTPoint& FTPolygonGlyph::Render(const FTPoint& pen, int renderMode)
{
    FTPolygonGlyphImpl *myimpl = dynamic_cast<FTPolygonGlyphImpl *>(impl);
    return myimpl->RenderImpl(pen, renderMode);
}


//
//  FTGLPolyGlyphImpl
//


FTPolygonGlyphImpl::FTPolygonGlyphImpl(FT_GlyphSlot glyph, float _outset,
                                       bool useDisplayList)
:   FTGlyphImpl(glyph)
{
    if(ft_glyph_format_outline != glyph->format)
    {
        err = 0x14; // Invalid_Outline
        return;
    }
    pgCurrIndex = 0;
    pgVertices = NULL;
    vectoriser = new FTVectoriser(glyph);

    if((vectoriser->ContourCount() < 1) || (vectoriser->PointCount() < 3))
    {
        delete vectoriser;
        vectoriser = NULL;
        return;
    }
    
    hscale = glyph->face->size->metrics.x_ppem * 64;
    vscale = glyph->face->size->metrics.y_ppem * 64;
    outset = _outset;
    
    if (vectoriser)
        vectoriser->MakeMesh(1.0, 1, outset);
}


FTPolygonGlyphImpl::~FTPolygonGlyphImpl()
{
    if (vectoriser)
    {
        delete vectoriser;
    }
    glDeleteBuffers(1, &bufferHnd);
}






GLvoid FTPolygonGlyphImpl::pgVertex3f(float x, float y, float z) 
{
    pgCurrVertex.xyz[0] = x;
    pgCurrVertex.xyz[1] = y;
    pgCurrVertex.xyz[2] = z;
    pgVertices[pgCurrIndex] = pgCurrVertex;
    pgCurrIndex++;
}


GLvoid FTPolygonGlyphImpl::pgTexCoord2f(GLfloat s, GLfloat t) 
{
    pgCurrVertex.st[0] = s;
    pgCurrVertex.st[1] = t;
}


const FTPoint& FTPolygonGlyphImpl::RenderImpl(const FTPoint& pen,
                                              int renderMode)
{
    if (pgCurrIndex == 0)
    {
        if (vectoriser)
        {
            const FTMesh *mesh = vectoriser->GetMesh();
            
            int vertexCount = 1;
            for(unsigned int t = 0; t < vectoriser->GetMesh()->TesselationCount(); ++t)
            {
                vertexCount += vectoriser->GetMesh()->Tesselation(t)->PointCount();
            }
            
            if (!(pgVertices = (polygonGlyphVertex_t *)calloc(1, sizeof(polygonGlyphVertex_t) * vertexCount)))
            {
                return advance;
            }
            
            DoRender(pen);
            
            glGenBuffers(1, &bufferHnd);
            glBindBuffer(GL_ARRAY_BUFFER, bufferHnd);
            glBufferData(GL_ARRAY_BUFFER, sizeof(polygonGlyphVertex_t) * pgCurrIndex, pgVertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            printf("%d %d\n", vertexCount, bufferHnd);
            
            free(pgVertices);
        }
    }
    
    if (bufferHnd > 0)
    {
        glTranslatef(pen.Xf(), pen.Yf(), pen.Zf());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, bufferHnd);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(polygonGlyphVertex_t), 0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(polygonGlyphVertex_t), ((char *)NULL + (12)));
        glDrawArrays(GL_TRIANGLES, 0, pgCurrIndex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);    
        glTranslatef(-pen.Xf(), -pen.Yf(), -pen.Zf());
    }
    
    return advance;
}


void FTPolygonGlyphImpl::DoRender(const FTPoint& pen)
{
    const FTMesh *mesh = vectoriser->GetMesh();

    pgCurrIndex = 0;
    for(unsigned int t = 0; t < mesh->TesselationCount(); ++t)
    {
        const FTTesselation* subMesh = mesh->Tesselation(t);
        
        for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
        {
            FTPoint point = subMesh->Point(i);
            pgTexCoord2f(point.Xf() / hscale, point.Yf() / vscale);
            pgVertex3f(point.Xf() / 64.0f, point.Yf() / 64.0f, 0.0f);
        }
    }
}
