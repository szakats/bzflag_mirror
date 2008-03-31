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

#ifndef CONFIGURATIONMENU_H_
#define CONFIGURATIONMENU_H_

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Menu_Button.H>

#include "Fl_Tweak.h"

class MainWindow;
class bz2object;

/**
 * This is the menu that pops up when you right-click a selected object.
 */
 
class ConfigurationMenu : public Fl_Menu_Button {

public:

	// constructor and destructor
	ConfigurationMenu(MainWindow* parent, int x, int y, int width, int height);
	virtual ~ConfigurationMenu() { }
	
	// update the object
	void setObject( bz2object* _obj ) {this->obj = _obj; this->buildMenu(); }
	
	// static callbacks
	static void translate_x(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->translate_x_real( w );
	}
	
	static void translate_y(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->translate_y_real( w );
	}
	
	static void translate_z(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->translate_z_real( w );
	}
	
	static void rotate_x(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->rotate_x_real( w );
	}
	
	static void rotate_y(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->rotate_y_real( w );
	}
	
	static void rotate_z(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->rotate_z_real( w );
	}
	
	static void scale_x(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->scale_x_real( w );
	}
	
	static void scale_y(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->scale_y_real( w );
	}
	
	static void scale_z(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->scale_z_real( w );
	}
	
	static void tweak(Fl_Widget* w, void* data) {
		ConfigurationMenu* cm = (ConfigurationMenu*)(data);
		cm->tweak_real( w );
	}
	
private:

	// instantiated callbacks
	void translate_x_real( Fl_Widget* w );
	void translate_y_real( Fl_Widget* w );
	void translate_z_real( Fl_Widget* w );
	
	void rotate_x_real( Fl_Widget* w );
	void rotate_y_real( Fl_Widget* w );
	void rotate_z_real( Fl_Widget* w );
	
	void scale_x_real( Fl_Widget* w );
	void scale_y_real( Fl_Widget* w );
	void scale_z_real( Fl_Widget* w );
	
	void tweak_real( Fl_Widget* w );
	
	// parent
	MainWindow* parent;
	
	// the object to configure
	bz2object* obj;
	
	// build the menu (helper method)
	void buildMenu(void);
	
};

#include "../windows/MainWindow.h"

#endif /*CONFIGURATIONMENU_H_*/
