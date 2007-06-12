#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include <stdlib.h>
#include <math.h>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scroll.H>
#include "QuickLabel.h"
#include "../defines.h"
#include "../ftoa.h"
#include "../Transform.h"

#include <string>
#include <vector>
#include <memory>

using namespace std;

// transformation widget for MasterconfigurationDialog
class TransformWidget : public Fl_Group {
public:

	// unused; prevents the build from breaking
	// (Fl_Group's copy constructor is private)
	TransformWidget(const TransformWidget& t);
	
	// the format for fields is "<type1:|field1|field2|field3|" ... "|fieldN|>"
	TransformWidget(int x, int y, int WIDTH, int HEIGHT, const char* fields, bool active);
	
	virtual ~TransformWidget();
	
	// read the transformation type from the menu
	static void getTransformationCallback(Fl_Widget* w, void* data);
	
	// getters
	string getTransformationType(void);
	vector<string> getSupportedTransformations(void);
	bool isActive(void);
	
	// setters
	// changes the type of transformation and changes the fields
	void setTransformationType(string& s);
	void setTransformationType(const char* s);
	
	// set the fields with string represented values
	void setFields(vector<string> fieldValues);
	
	// set the fields with float values
	void setFields(vector<float> fieldValues);
	
	
	// return string representation in BZW format
	string toString();
	
private:

	// label the menu button with the selection
	void getTransformationCallback_real(TransformWidget* t, Fl_Menu_Button* mb);
	
	// change callbacks based on a new selection
	void changeFields(const char* name);
	
	// data
	vector<string> transformTypes;
	Fl_Menu_Button* typeMenu;
	Fl_Check_Button* activeButton;
	vector<Fl_Float_Input*> fields;
	string format;
};
#endif /*TRANSFORMATION_H_*/
