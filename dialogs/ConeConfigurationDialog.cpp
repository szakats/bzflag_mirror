#include "../include/dialogs/ConeConfigurationDialog.h"

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
	
}

// OK callback
void ConeConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	
}

// Cancel callback
void ConeConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	
}
