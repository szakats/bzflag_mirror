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

#include "dialogs/ConeConfigurationDialog.h"

// constructor
ConeConfigurationDialog::ConeConfigurationDialog( cone* theCone ) :
	ConfigurationDialog( theCone, "Cone Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	this->begin();
	
	this->theCone = theCone;
	
	// set up the dialog
	this->nameLabel = new QuickLabel( "Name: ", 5, 5);
	this->nameField = new Fl_Input( 60, 5, DEFAULT_WIDTH - 65, DEFAULT_TEXTSIZE + 6);
	this->nameField->value( theCone->getName().c_str() );
	
	this->subdivisionLabel = new QuickLabel("Subdivisions: ", 5, 30);
	this->subdivisionCounter = new Fl_Counter( 120, 30, 120, DEFAULT_TEXTSIZE + 6 );
	this->subdivisionCounter->type( FL_SIMPLE_COUNTER );
	this->subdivisionCounter->value( theCone->getDivisions() );
	this->subdivisionCounter->step( 1.0 );
	
	this->sweepAngleLabel = new QuickLabel("Sweep Angle: ", 5, 55 );
	this->sweepAngleCounter = new Fl_Counter( 120, 55, 120, DEFAULT_TEXTSIZE + 6 );
	this->sweepAngleCounter->type( FL_NORMAL_COUNTER );
	this->sweepAngleCounter->value( theCone->getSweepAngle() );
	
	this->flatShadingButton = new Fl_Check_Button(5, 80, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Flat Shading");
	this->flatShadingButton->value( theCone->hasFlatShading() == true ? 1 : 0 );
	
	this->smoothBounceButton = new Fl_Check_Button(5, 105, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Smooth Bounce");
	this->smoothBounceButton->value( theCone->hasSmoothBounce() == true ? 1 : 0 );
	
	this->end();
	
	// add the callbacks
	this->setOKEventHandler( OKCallback, this );
	this->setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void ConeConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theCone->setSmoothBounce( smoothBounceButton->value() == 1 ? true : false );
	theCone->setSweepAngle( sweepAngleCounter->value() );
	theCone->setDivisions( (int)subdivisionCounter->value() );
	theCone->setFlatShading( flatShadingButton->value() == 1 ? true : false );
	
	// don't delete this dialog box just yet...just hide it
	this->hide();
}

// Cancel callback
void ConeConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	this->hide();
}
