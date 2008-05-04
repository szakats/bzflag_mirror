/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
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

#include "FTGL/ftgl.h"

#include "FTInternals.h"
#include "FTPixmapFontImpl.h"


//
//  FTPixmapFont
//


FTPixmapFont::FTPixmapFont(char const *fontFilePath)
{
    impl = new FTPixmapFontImpl(fontFilePath);
}


FTPixmapFont::FTPixmapFont(const unsigned char *pBufferBytes,
                           size_t bufferSizeInBytes)
{
    impl = new FTPixmapFontImpl(pBufferBytes, bufferSizeInBytes);
}


FTPixmapFont::~FTPixmapFont()
{
    ;
}


//
//  FTPixmapFontImpl
//


FTGlyph* FTPixmapFontImpl::MakeGlyph(unsigned int g)
{
    FT_GlyphSlot ftGlyph = face.Glyph(g, FT_LOAD_NO_HINTING
                                          | FT_LOAD_NO_BITMAP);

    if(ftGlyph)
    {
        FTPixmapGlyph* tempGlyph = new FTPixmapGlyph(ftGlyph);
        return tempGlyph;
    }

    err = face.Error();
    return NULL;
}


template <typename T>
inline void FTPixmapFontImpl::RenderI(const T* string)
{
    glPushAttrib(GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_TEXTURE_2D);

    GLfloat ftglColour[4];
    glGetFloatv(GL_CURRENT_RASTER_COLOR, ftglColour);

    glPixelTransferf(GL_RED_SCALE, ftglColour[0]);
    glPixelTransferf(GL_GREEN_SCALE, ftglColour[1]);
    glPixelTransferf(GL_BLUE_SCALE, ftglColour[2]);
    glPixelTransferf(GL_ALPHA_SCALE, ftglColour[3]);

    FTFontImpl::Render(string);

    glPopClientAttrib();
    glPopAttrib();
}


void FTPixmapFontImpl::Render(const char* string)
{
    RenderI(string);
}


void FTPixmapFontImpl::Render(const char* string, int renderMode)
{
    RenderI(string);
}


void FTPixmapFontImpl::Render(const wchar_t* string)
{
    RenderI(string);
}


void FTPixmapFontImpl::Render(const wchar_t* string, int renderMode)
{
    RenderI(string);
}

