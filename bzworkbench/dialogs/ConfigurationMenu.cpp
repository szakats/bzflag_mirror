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

#include "dialogs/ConfigurationMenu.h"

// build the menu
void ConfigurationMenu::buildMenu(void) {
	
	if( obj == NULL )
		return;
		
	this->clear();
	
	if( obj->isKey("position") ) {
		this->add("Translate", 0, 0, 0, FL_SUBMENU);
			this->add("Translate/Along X", 0, this->translate_x, this);
			this->add("Translate/Along Y", 0, this->translate_y, this);
			this->add("Translate/Along Z", 0, this->translate_z, this);
	}
	this->add("Rotate", 0, 0, 0, FL_SUBMENU);
		if( obj->isKey("spin") ) {
			this->add("Rotate/Around X", 0, this->rotate_x, this);
			this->add("Rotate/Around Y", 0, this->rotate_y, this);
		}
		if( obj->isKey( "spin" ) || obj->isKey( "rotation" ) )
			this->add("Rotate/Around Z", 0, this->rotate_z, this);
	
	if( obj->isKey("size") || obj->isKey("scale") ) {
		this->add("Scale", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
			this->add("Scale/Along X", 0, this->scale_x, this);
			this->add("Scale/Along Y", 0, this->scale_y, this);
			this->add("Scale/Along Z", 0, this->scale_z, this);
	}
	
	this->add("Tweak...", 0, this->tweak, this);
}

// constructor...nothing atypical here
ConfigurationMenu::ConfigurationMenu( MainWindow* mw, int x, int y, int width, int height ) :
	Fl_Menu_Button( x, y, width, height ) {
	
	this->type(Fl_Menu_Button::POPUP3);
	
	this->parent = mw;
	
	this->obj = NULL;
	
}

// instantiated callbacks
void ConfigurationMenu::translate_x_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::TRANSLATE_X );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::translate_y_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::TRANSLATE_Y );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::translate_z_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::TRANSLATE_Z );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::rotate_x_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::ROTATE_X );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::rotate_y_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::ROTATE_Y );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::rotate_z_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::ROTATE_Z );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::scale_x_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::SCALE_X );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::scale_y_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::SCALE_Y );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::scale_z_real( Fl_Widget* w ) {
	Fl_Tweak* fltw = new Fl_Tweak( this->obj, Fl_Tweak::SCALE_Z );
	fltw->show();
	
	// wait for the operation to complete
	while( fltw->shown() ) { Fl::wait(); }
	
	// update the model and scene
	this->parent->getModel()->notifyObservers(NULL);
}

void ConfigurationMenu::tweak_real( Fl_Widget* w ) {
	MasterConfigurationDialog* mcd = new MasterConfigurationDialog( obj );
	mcd->show();
}
