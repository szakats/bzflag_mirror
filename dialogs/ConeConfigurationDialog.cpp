#include "../include/dialogs/ConeConfigurationDialog.h"

// constructor
ConeConfigurationDialog::ConeConfigurationDialog( cone* theCone ) :
	ConfigurationDialog( theCone, "Cone Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {

	this->theCone = theCone;
	
	// set up the dialog
	
}

// OK callback
