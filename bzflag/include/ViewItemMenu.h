/* bzflag
 * Copyright (c) 1993 - 2001 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef BZF_VIEWITEMMENU_H
#define BZF_VIEWITEMMENU_H

#include "View.h"

class ViewItemMenu : public View {
public:
	ViewItemMenu();

protected:
	virtual ~ViewItemMenu();

	// View overrides
	virtual bool		onPreRender(float x, float y, float w, float h);
	virtual void		onPostRender(float x, float y, float w, float h);
};

class ViewItemMenuReader : public ViewTagReader {
public:
	ViewItemMenuReader();
	virtual ~ViewItemMenuReader();

	// ViewItemReader overrides
	virtual ViewTagReader* clone() const;
	virtual View*		open(const ConfigReader::Values&);
	virtual void		close();

private:
	ViewItemMenu*		item;
};

#endif
