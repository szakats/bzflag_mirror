#ifndef CONECONFIGURATIONDIALOG_H_
#define CONECONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "ConfigurationDialog.h"
#include "../widgets/QuickLabel.h"

#include "../objects/cone.h"

class ConeConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 200;
	static const int DEFAULT_HEIGHT = 100;

	// constructor
	ConeConfigurationDialog( cone* theCone );
	
	// destructor
	virtual ~ConeConfigurationDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		ConeConfigurationDialog* ccd = (ConeConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CanelCallback( Fl_Widget* w, void* data ) {
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
	
	// rotation label
	
	
};

#endif /*CONECONFIGURATIONDIALOG_H_*/
