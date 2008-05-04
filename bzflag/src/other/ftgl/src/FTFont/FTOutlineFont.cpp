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
#include "FTOutlineFontImpl.h"


//
//  FTOutlineFont
//


FTOutlineFont::FTOutlineFont(char const *fontFilePath)
{
    impl = new FTOutlineFontImpl(fontFilePath);
}


FTOutlineFont::FTOutlineFont(const unsigned char *pBufferBytes,
                             size_t bufferSizeInBytes)
{
    impl = new FTOutlineFontImpl(pBufferBytes, bufferSizeInBytes);
}


FTOutlineFont::~FTOutlineFont()
{
    ;
}


//
//  FTOutlineFontImpl
//


FTGlyph* FTOutlineFontImpl::MakeGlyph(unsigned int g)
{
    FT_GlyphSlot ftGlyph = face.Glyph(g, FT_LOAD_NO_HINTING);

    if(ftGlyph)
    {
        FTOutlineGlyph* tempGlyph = new FTOutlineGlyph(ftGlyph, outset, useDisplayLists);
        return tempGlyph;
    }

    err = face.Error();
    return NULL;
}


template <typename T>
inline void FTOutlineFontImpl::RenderI(const T* string)
{
    glPushAttrib(GL_ENABLE_BIT | GL_HINT_BIT | GL_LINE_BIT
                  | GL_COLOR_BUFFER_BIT);

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE

    FTFontImpl::Render(string);

    glPopAttrib();
}


void FTOutlineFontImpl::Render(const char* string)
{
    RenderI(string);
}


void FTOutlineFontImpl::Render(const char* string, int renderMode)
{
    RenderI(string);
}


void FTOutlineFontImpl::Render(const wchar_t* string)
{
    RenderI(string);
}


void FTOutlineFontImpl::Render(const wchar_t* string, int renderMode)
{
    RenderI(string);
}

