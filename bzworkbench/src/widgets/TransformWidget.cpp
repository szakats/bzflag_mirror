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

#include "widgets/TransformWidget.h"
#include "defines.h"

// unused; prevents the build from breaking
// (Fl_Group's copy constructor is private)
TransformWidget::TransformWidget(const TransformWidget& t) : Fl_Group(0, 0, 0, 0) { end(); }

// the format for fields is "<type1:|field1|field2|field3|" ... "|fieldN|>"
TransformWidget::TransformWidget(int x, int y, int WIDTH, int HEIGHT, const char* inputFields, bool active) : 
	Fl_Group(x, y, WIDTH, HEIGHT) {
	
	end();
	
	string transforms = string(inputFields);
	transformTypes = vector<string>();
	
	// initialize data
	format = string(inputFields);
	fields = vector<Fl_Float_Input*>();
	
	// initialize the members
	typeMenu = new Fl_Menu_Button(x + 30, y, 100, DEFAULT_TEXTSIZE + 6);
	activeButton = new Fl_Check_Button(x, y, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6);
	activeButton->value(active);
	
	
	// find all the transformation types
	while(true) {
		// find the < and : to get the types
		string::size_type start = transforms.find("<", 0);
		string::size_type end = transforms.find(":", 0);
		
		if(start == string::npos || end == string::npos)
			break;
			
		// get the type
		string key = transforms.substr( start+1, end - (start+1) );
		
		// add the type
		transformTypes.push_back( key );
		
		// find the end of the line
		end = transforms.find(">", end);
		
		// break when we run out of string 
		if(transforms.length() <= 2)
			break;
		
		// advance transforms
		transforms = transforms.substr( end+1 );
		
	}
	
	// build the transformation types menu
	for(vector<string>::iterator i = transformTypes.begin(); i != transformTypes.end(); i++) {
		typeMenu->add( i->c_str(), 0, getTransformationCallback, this);
	}
	
	// label it
	typeMenu->label(transformTypes[0].c_str());
	
	// add the widgets
	add(typeMenu);
	add(activeButton);
	
	// build the fields
	changeFields(transformTypes[0].c_str());
}

TransformWidget::~TransformWidget() { }

// read the transformation type from the menu
void TransformWidget::getTransformationCallback(Fl_Widget* w, void* data) {
	TransformWidget* t = (TransformWidget*)(data);
	Fl_Menu_Button* mb = (Fl_Menu_Button*)(w);
	t->getTransformationCallback_real(t, mb);
}

// getters
string TransformWidget::getTransformationType(void) { return string( typeMenu->label() ); }
vector<string> TransformWidget::getSupportedTransformations(void) { return transformTypes; }
bool TransformWidget::isActive(void) { return (activeButton->value() == 1 ? true : false); }

// setters
// changes the type of transformation and changes the fields
void TransformWidget::setTransformationType(string& s) {
	typeMenu->copy_label(s.c_str());
	changeFields(s.c_str());
}
void TransformWidget::setTransformationType(const char* s) {
	string str = string(s);
	setTransformationType(str);	
}

// set the fields with string represented values
void TransformWidget::setFields(vector<string> fieldValues) {
	int index = 0;
	for(vector<Fl_Float_Input*>::iterator i = fields.begin(); i != fields.end(); i++) {
		(*i)->value(fieldValues[index].c_str());
		index++;
	}	
}

// set the fields with float values
void TransformWidget::setFields(vector<float> fieldValues) {
	int index = 0;
	for(vector<Fl_Float_Input*>::iterator i = fields.begin(); i != fields.end(); i++) {
		(*i)->value(ftoa(fieldValues[index]).c_str());
		index++;
	}	
}


// return string representation in BZW format
string TransformWidget::toString() {
	string ret = string(typeMenu->label()) + " ";
	
	// if this is a rotation, we need to make sure that the vector is normalized
	if(strcmp(typeMenu->label(), "spin") == 0) {
		
		float dx = atof( fields[1]->value() );
		float dy = atof( fields[2]->value() );
		float dz = atof( fields[3]->value() );
		
		float divisor = sqrt(dx*dx + dy*dy + dz*dz);
		
		if(divisor != 0.0f) {
			dx /= divisor;
			dy /= divisor;
			dz /= divisor;	
		}
		
		fields[1]->value( ftoa(dx).c_str() );
		fields[2]->value( ftoa(dy).c_str() );
		fields[3]->value( ftoa(dz).c_str() );
	}
	
	// commit the values to the return string
	for(vector<Fl_Float_Input*>::iterator i = fields.begin(); i != fields.end(); i++) {
		string value = string((*i)->value());
		if(value.length() == 0)
			ret += string( "0.0 " );
		else
			ret += string((*i)->value()) + " ";	
	}
	
	return ret + "\n";
}

/////////////////////////
// Private methods
/////////////////////////

// label the menu button with the selection
void TransformWidget::getTransformationCallback_real(TransformWidget* t, Fl_Menu_Button* mb) {
	const Fl_Menu_Item* menu = mb->menu();
	mb->label(menu[mb->value()].label());
	changeFields(mb->label());
	parent()->redraw();
}

// change callbacks based on a new selection
void TransformWidget::changeFields(const char* name) {
	string transformation = string( name );
	
	// remove previous fields
	if(fields.size() != 0) {
		for(vector<Fl_Float_Input*>::iterator i = fields.begin(); i != fields.end(); i++) {
			remove(*i);
			delete *i;
		}	
	}
	fields.clear();
	
	// cut out the list of fields
	string::size_type startIndex = format.find("<" + transformation + ":", 0);
	startIndex = format.find("|", startIndex);
	
	string::size_type endIndex = format.find("|>", startIndex);
	
	string fieldList = format.substr(startIndex, endIndex - startIndex + 1);
	
	int tempX = x() + 150;
	int tempY = y();
			
	string fieldName;
	// find all fields and make them
	while(true) {
		// locate the |'s at the start and end
		string::size_type start = fieldList.find("|", 0);
		string::size_type end = fieldList.find("|", start+1);
		
		// get the text between the |'s
		fieldName = fieldList.substr(start+1, end - (start+1));
		
		// add the float field
		Fl_Float_Input* newfield = new Fl_Float_Input(tempX, tempY, 100, DEFAULT_TEXTSIZE + 6);
		newfield->copy_label(fieldName.c_str());
		newfield->align(FL_ALIGN_TOP);
		
		fields.push_back(newfield);
		add(newfield);
		newfield->value("0.000000");
		
		// advance fieldList
		fieldList = fieldList.substr(end);	
		
		// advance X
		tempX += 120;
		
		// break when we run out of string
		if(fieldList.length() < 2)
			break;
	}
}
