#ifndef WORLDOPTIONSDIALOG_H_
#define WORLDOPTIONSDIALOG_H_

#include <FL/Fl_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "Fl_Dialog.h"
#include "QuickLabel.h"
#include "Model.h"

#include <string>

/** This is the world options dialog box.  It controls:
 *		* The name of the world
 * 		* The options string passed to the game
 *		* The height of the flags in the world
 * 		* The height of the water int he world (if desired)
 */
class WorldOptionsDialog : public Fl_Dialog
{
public:
	
	static const int WIDTH = 305;
	static const int HEIGHT = 160;
	
	// constructor
	WorldOptionsDialog();
	
	// destructor
	virtual ~WorldOptionsDialog();
	
	// static callbacks
	// These need to be static so the this variable, when passed to the callback, is initialized already.
	static void OKButtonCallback(Fl_Widget* w, void* d)		{ 
		WorldOptionsDialog* dialog = (WorldOptionsDialog*)(d);
		dialog->OKButtonCallback_real(w); 
	}
	
	static void CancelButtonCallback(Fl_Widget* w, void* d)	{
		WorldOptionsDialog* dialog = (WorldOptionsDialog*)(d);
		dialog->CancelButtonCallback_real(w); 
	}
	
private:
	
	// the text field for the world name
	Fl_Input* worldNameField;
	
	// label for the world name field
	QuickLabel* worldNameLabel;
	
	// the text field for the world options
	Fl_Input* worldOptionsField;
	
	// label for the world options field
	QuickLabel* worldOptionsLabel;
	
	// the text field for the flag height
	Fl_Counter* flagHeightField;
	
	// label for the flag height field
	QuickLabel* flagHeightLabel;
	
	// the text field for the water level
	Fl_Counter* waterLevelField;
	
	// the checkbox for water
	Fl_Check_Button* waterCheckButton;
	
	// world options data reference from the model
	WorldOptionsData* data;
	
	// callbacks (see WorldOptionsDialog.cpp for specifics)
	void OKButtonCallback_real(Fl_Widget* w);
	void CancelButtonCallback_real(Fl_Widget* w);
};

#endif /*WORLDOPTIONSDIALOG_H_*/
