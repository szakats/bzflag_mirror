#ifndef ADVANCEDOPTIONSDIALOG_H_
#define ADVANCEDOPTIONSDIALOG_H_

#include "Fl_Dialog.h"
#include "ConfigurationDialog.h"

#include "../widgets/QuickLabel.h"
#include "../widgets/MaterialWidget.h"
#include "../defines.h"

#include "../model/Model.h"

#include "../objects/material.h"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scroll.H>

#include <vector>
#include <string>
#include <map>

using namespace std;

/**
 * This is the dialog that opens when the user clicks "Advanced..." on the Master Configuration Dialog
 */
 
class AdvancedOptionsDialog : public ConfigurationDialog {

public:

	static const int DEFAULT_WIDTH = 400;
	static const int DEFAULT_HEIGHT = 250;
	
	// constructor
	AdvancedOptionsDialog( bz2object* obj );
	
	// destructor
	virtual ~AdvancedOptionsDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		AdvancedOptionsDialog* ccd = (AdvancedOptionsDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		AdvancedOptionsDialog* ccd = (AdvancedOptionsDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
	// add material callback
	static void addMaterialCallback( Fl_Widget* w, void* data ) {
		AdvancedOptionsDialog* ccd = (AdvancedOptionsDialog*)data;
		ccd->addMaterialCallback_real( w );
	}
	
private:

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	
	void addMaterialCallback_real( Fl_Widget* w, MaterialWidget* newMaterial = NULL );
	
	// material widgets
	QuickLabel* materialLabel;
	Fl_Button* materialNew;
	Fl_Button* materialEdit;
	Fl_Button* materialDelete;
	Fl_Scroll* materialList;
	vector< MaterialWidget* > materialWidgets;
	
	// physics driver widgets
	QuickLabel* phydrvLabel;
	Fl_Menu_Button* phydrvMenu;
	Fl_Button* phydrvNew;
	Fl_Button* phydrvEdit;
	Fl_Button* phydrvDelete;
	
	// bz2object reference
	// (duplicated from ConfigurationDialog::object for type safety)
	bz2object* obj;
	
};

#endif /*ADVANCEDOPTIONSDIALOG_H_*/
