/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef FL_TWEAK_H_
#define FL_TWEAK_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>

#include "../widgets/QuickLabel.h"
#include "Fl_Dialog.h"
#include "../objects/bz2object.h"
#include "../defines.h"

class Fl_Tweak : public Fl_Dialog {

public:

	// tweak operations
	enum TweakOp {
		TRANSLATE_X,
		TRANSLATE_Y,
		TRANSLATE_Z,
		ROTATE_X,
		ROTATE_Y,
		ROTATE_Z,
		SCALE_X,
		SCALE_Y,
		SCALE_Z
	};

	// dimensions
	static const int DEFAULT_WIDTH = 180;
	static const int DEFAULT_HEIGHT = 60;

	// constructor
	Fl_Tweak( bz2object* obj, TweakOp op );

	// static tweak callback
	static void tweak_callback(Fl_Widget* w, void* data ) {
		Fl_Tweak* fltw = (Fl_Tweak*)(data);
		fltw->tweak_callback_real( w );
	}

private:

	// object to tweak
	bz2object* obj;

	// data field
	Fl_Float_Input* inputField;

	// data field label
	QuickLabel* fieldLabel;

	// real tweak callback
	void tweak_callback_real( Fl_Widget* w );

	// the operation we're going to do
	TweakOp op;
};

#endif /*FL_TWEAK_H_*/
