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

#ifndef CONECONFIGURATIONDIALOG_H_
#define CONECONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "ConfigurationDialog.h"
#include "../widgets/QuickLabel.h"
#include "../defines.h"

#include "../objects/cone.h"

class ConeConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 150;

	// constructor
	ConeConfigurationDialog( cone* theCone );
	
	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) { 
		cone* c = dynamic_cast< cone* > (d);
		if( c )
			return new ConeConfigurationDialog( dynamic_cast< cone* >( d ) );
		else
			return NULL;
	}
	
	// destructor
	virtual ~ConeConfigurationDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		ConeConfigurationDialog* ccd = (ConeConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		ConeConfigurationDialog* ccd = (ConeConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
private:
	
	// the cone
	cone* theCone;
	
	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	
	// name label
	QuickLabel* nameLabel;
	
	// name field
	Fl_Input* nameField;
	
	// subdivision label
	QuickLabel* subdivisionLabel;
	
	// subdivision counter
	Fl_Counter* subdivisionCounter;
	
	// angle sweep label
	QuickLabel* sweepAngleLabel;
	
	// angle sweep counter
	Fl_Counter* sweepAngleCounter;
	
	// flat shading check-button
	Fl_Check_Button* flatShadingButton;
	
	// smooth bounce check-button
	Fl_Check_Button* smoothBounceButton;
	
	
};

#endif /*CONECONFIGURATIONDIALOG_H_*/
