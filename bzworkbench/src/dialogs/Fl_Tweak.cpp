/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "dialogs/Fl_Tweak.h"

// constructor function
Fl_Tweak::Fl_Tweak( bz2object* obj, TweakOp op ) : Fl_Dialog( "Configure", DEFAULT_WIDTH, DEFAULT_HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL ) {
	obj = obj;
	setOKEventHandler( Fl_Tweak::tweak_callback, this );
	
	op = op;
	
	// set up the dialog box
	
	// make the label
	switch( op ) {
		case TRANSLATE_X:
			fieldLabel = new QuickLabel( "Translate along X", 10, 10 );
			break;
		case TRANSLATE_Y:
			fieldLabel = new QuickLabel( "Translate along Y", 10, 10 );
			break;
		case TRANSLATE_Z:
			fieldLabel = new QuickLabel( "Translate along Z", 10, 10 );
			break;
		case ROTATE_X:
			fieldLabel = new QuickLabel( "Rotate around X", 10, 10 );
			break;
		case ROTATE_Y:
			fieldLabel = new QuickLabel( "Rotate around Y", 10, 10 );
			break;
		case ROTATE_Z:
			fieldLabel = new QuickLabel( "Rotate around Z", 10, 10 );
			break;
		case SCALE_X:
			fieldLabel = new QuickLabel( "Scale along X", 10, 10 );
			break;
		case SCALE_Y:
			fieldLabel = new QuickLabel( "Scale along Y", 10, 10 );
			break;
		case SCALE_Z:
			fieldLabel = new QuickLabel( "Scale along Z", 10, 10 );
			break;
		default:
			fieldLabel = new QuickLabel( "(unknown tweak operator)", 10, 10 );
	}
	
	// make the field
	inputField = new Fl_Float_Input(10, 16 + DEFAULT_TEXTSIZE, DEFAULT_WIDTH - 20, DEFAULT_TEXTSIZE + 6);
	
	add( fieldLabel );
	add( inputField );
}

// callback
// apply the desired tweak operation
void Fl_Tweak::tweak_callback_real( Fl_Widget* w ) {
	float value = atof( inputField->value() );
	
	switch( op ) {
		case TRANSLATE_X:
			obj->setPos( obj->getPos() + osg::Vec3( value, 0.0, 0.0 ) );
			break;	
		case TRANSLATE_Y:
			obj->setPos( obj->getPos() + osg::Vec3( 0.0, value, 0.0 ) );
			break;
		case TRANSLATE_Z:
			obj->setPos( obj->getPos() + osg::Vec3( 0.0, 0.0, value ) );
			break;
		case ROTATE_X:
			obj->setRotationX( obj->getRotation().x() + value );
			break;
		case ROTATE_Y:
			obj->setRotationY( obj->getRotation().y() + value );
			break;
		case ROTATE_Z:
			obj->setRotationZ( obj->getRotation().z() + value );
			break;
		case SCALE_X:
			obj->setSize( obj->getSize() + osg::Vec3( value, 0.0, 0.0 ) );
			break;
		case SCALE_Y:
			obj->setSize( obj->getSize() + osg::Vec3( 0.0, value, 0.0 ) );
			break;
		case SCALE_Z:
			obj->setSize( obj->getSize() + osg::Vec3( 0.0, 0.0, value ) );
			break;
		default:
			break;
	}
	
	// close the window
	Fl::delete_widget( this );
}
