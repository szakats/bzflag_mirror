/* bzflag
 * Copyright (c) 1993 - 2009 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * HUDuiElement:
 *	User interface class and functions for a visual noninteractive
 *	UI element.
 */

#ifndef	__HUDUIELEMENT_H__
#define	__HUDUIELEMENT_H__

/* bzflag common header */
#include "common.h"

/* system headers */
#include <string>

/* common headers */
#include "bzfgl.h"
#include "vectors.h"

class LocalFontFace;

class HUDuiElement {
  friend class HUDui;
  public:
			HUDuiElement();
    virtual		~HUDuiElement();

    float		getX() const;
    float		getY() const;
    float		getWidth() const;
    float		getHeight() const;
    float		getLabelWidth() const;
    std::string		getLabel() const;
    const LocalFontFace*	getFontFace() const;
    float		getFontSize() const;

    virtual void		setPosition(float x, float y);
    virtual void		setSize(float width, float height);
    void		setLabelWidth(float width);
    void		setLabel(const std::string& label);
    virtual void		setFontFace(const LocalFontFace* face);
    virtual void		setFontSize(float size);

    virtual void	render();

  protected:
    virtual void	onSetFont();
    virtual void	doRender() = 0;

    void		renderLabel();

    static const fvec4	dimTextColor;
    static const fvec4	moreDimTextColor;
    static const fvec4	textColor;

    bool		skipRenderLabel;
    float		fontHeight;

  private:
    const LocalFontFace*	elementFontFace;
    float		elementFontSize;
    float		elementX, elementY;
    float		elementWidth, elementHeight;
    float		desiredLabelWidth, trueLabelWidth;
    std::string		label;
};

//
// inline functions
//

inline
float			HUDuiElement::getX() const
{
  return elementX;
}

inline
float			HUDuiElement::getY() const
{
  return elementY;
}

inline
float			HUDuiElement::getWidth() const
{
  return elementWidth;
}

inline
float			HUDuiElement::getHeight() const
{
  return elementHeight;
}

#endif // __HUDUICONTROL_H__

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
