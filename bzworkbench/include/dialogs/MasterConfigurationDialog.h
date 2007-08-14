#ifndef MASTERCONFIGURATIONDIALOG_H_
#define MASTERCONFIGURATIONDIALOG_H_

#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scroll.H>
#include "ConfigurationDialog.h"
#include "AdvancedOptionsDialog.h"
#include "../widgets/QuickLabel.h"
#include "../widgets/TransformWidget.h"
#include "../DataEntry.h"
#include "../render/Point3D.h"
#include "../objects/bz2object.h"

#include <string>
#include <vector>

using namespace std;

/**
 * This dialog configures bz2object's general data
 */
 
class MasterConfigurationDialog : public ConfigurationDialog {
	
public:
	
	static const int WIDTH = 650;
	static const int HEIGHT = 415;
	
	// constructor
	MasterConfigurationDialog(DataEntry* data);
	
	// static initializer
	static MasterConfigurationDialog* init( DataEntry* data ) { return new MasterConfigurationDialog( data ); }
	
	// destructor--destroy the "More..." dialog if it exists
	~MasterConfigurationDialog() { if( moreDialog ) Fl::delete_widget( moreDialog ); }
	
	// set the configuration dialog to be opened when "More..." is clicked
	void setAdditionalConfiguration( ConfigurationDialog* conf ) { 
		this->moreDialog = conf;
		if( conf && moreButton )	// enable the "More..." button
			moreButton->activate();
	}
	
	// static callbacks
	// These need to be static so the this variable, when passed to the callback, is initialized already.
	static void OKButtonCallback(Fl_Widget* w, void* d)		{ 
		MasterConfigurationDialog* dialog = (MasterConfigurationDialog*)(d);
		dialog->OKButtonCallback_real(w); 
	}
	
	static void CancelButtonCallback(Fl_Widget* w, void* d)	{
		MasterConfigurationDialog* dialog = (MasterConfigurationDialog*)(d);
		dialog->CancelButtonCallback_real(w); 
	}
	
	static void addTransformCallback(Fl_Widget* w, void* d) {
		MasterConfigurationDialog* dialog = (MasterConfigurationDialog*)(d);
		dialog->addTransformCallback_real(w);
	}
	
	static void removeTransformCallback(Fl_Widget* w, void* d) {
		MasterConfigurationDialog* dialog = (MasterConfigurationDialog*)(d);
		dialog->removeTransformCallback_real(w);	
	}
	
	static void moreCallback(Fl_Widget* w, void* d) {
		MasterConfigurationDialog* dialog = (MasterConfigurationDialog*)(d);
		dialog->moreCallback_real(w);	
	}
	
	static void advancedCallback(Fl_Widget* w, void* d) {
		MasterConfigurationDialog* dialog = (MasterConfigurationDialog*)(d);
		dialog->advancedCallback_real(w);
	}
	
	static void editTextCallback(Fl_Widget* w, void* d) {
		MasterConfigurationDialog* dialog = (MasterConfigurationDialog*)(d);
		dialog->editTextCallback_real(w);	
	}
	
private:
	// object position label
	QuickLabel* positionLabel;
	
	// position x, y, and z fields
	Fl_Float_Input *positionXField, *positionYField, *positionZField;
	
	// rotation (about the Z axis) label
	// (not applicable or all objects)
	QuickLabel* rotationLabel;
	
	// input for rotation
	Fl_Float_Input* rotationField;
	
	// size label
	QuickLabel* sizeLabel;
	
	// size input
	Fl_Float_Input *sizeXField, *sizeYField, *sizeZField;
	
	// spin input (if applicable)
	Fl_Float_Input *spinXField, *spinYField, *spinZField;
	QuickLabel* spinLabel;
	
	// add Transformation button
	Fl_Button* addTransformationButton;
	
	// remove transformation button
	Fl_Button* removeTransformationButton;
	
	// more... button
	Fl_Button* moreButton;
	
	// edit text button
	Fl_Button* editTextButton;
	
	// advanced... button
	Fl_Button* advancedButton;
	
	// scroll area where the transformations are
	Fl_Scroll* transformationScrollArea;
	
	// transformations label
	QuickLabel* transformationLabel;
	
	// the supported transformations of the object we're dealing with
	string supportedTransformations;
	
	// transforms
	vector<TransformWidget*> transformations;
	
	// the object we're dealing with
	bz2object* object;
	
	// the object's transformation string
	string transformationFormat;

	// real callbacks
	void OKButtonCallback_real(Fl_Widget* w);
	void CancelButtonCallback_real(Fl_Widget* w);
	void addTransformCallback_real(Fl_Widget* w);
	void removeTransformCallback_real(Fl_Widget* w);
	void moreCallback_real(Fl_Widget* w);
	void advancedCallback_real(Fl_Widget* w);
	void editTextCallback_real(Fl_Widget* w);
	
	// the configuration dialog to be opened upon clicking "More..."
	ConfigurationDialog* moreDialog;
	
};

#endif /*MASTERCONFIGURATIONDIALOG_H_*/
