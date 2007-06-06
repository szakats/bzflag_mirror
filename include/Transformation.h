#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include <stdlib.h>
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
#include "Transformation.h"
#include "defines.h"
#include "ftoa.h"

#include <string>
#include <vector>
#include <memory>

using namespace std;

// transformation widget for MasterconfigurationDialog
class Transformation : public Fl_Group {
public:

	// unused; prevents the build from breaking
	// (Fl_Group's copy constructor is private)
	Transformation(const Transformation& t) : Fl_Group(0, 0, 0, 0) { this->end(); }
	
	// the format for fields is "<type1:|field1|field2|field3|" ... "|fieldN|>"
	Transformation(int x, int y, int WIDTH, int HEIGHT, const char* fields, bool active) : 
		Fl_Group(x, y, WIDTH, 2*(DEFAULT_TEXTSIZE) + 1) {
		
		this->end();
		
		string transforms = string(fields);
		transformTypes = vector<string>();
		
		// initialize data
		format = string(fields);
		this->fields = vector<Fl_Float_Input*>();
		
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
			typeMenu->add( i->c_str(), 0, this->getTransformationCallback, this);
		}
		
		// label it
		typeMenu->label(transformTypes[0].c_str());
		
		// add the widgets
		this->add(typeMenu);
		this->add(activeButton);
		
		// build the fields
		changeFields(transformTypes[0].c_str());
	}
	
	virtual ~Transformation() { }
	
	// read the transformation type from the menu
	static void getTransformationCallback(Fl_Widget* w, void* data) {
		Transformation* t = (Transformation*)(data);
		Fl_Menu_Button* mb = (Fl_Menu_Button*)(w);
		t->getTransformationCallback_real(t, mb);
	}
	
	// getters
	string getTransformationType(void) { return string( typeMenu->label() ); }
	vector<string> getSupportedTransformations(void) { return transformTypes; }
	bool isActive(void) { return activeButton->value(); }
	
	// setters
	// changes the type of transformation and changes the fields
	void setTransformationType(string& s) {
		typeMenu->copy_label(s.c_str());
		changeFields(s.c_str());
	}
	void setTransformationType(const char* s) {
		string str = string(s);
		setTransformationType(str);	
	}
	
	void setFields(vector<string> fieldValues) {
		int index = 0;
		for(vector<Fl_Float_Input*>::iterator i = fields.begin(); i != fields.end(); i++) {
			(*i)->value(fieldValues[index].c_str());
			index++;
		}	
	}
	
	// return string representation in BZW format
	string toString() {
		string ret = string(typeMenu->label()) + " ";
		
		// if this is a rotation, we need to make sure that the vector is normalized
		if(strcmp(typeMenu->label(), "spin") == 0) {
			printf("normalizing...");
			
			float dx = atof( fields[1]->value() );
			float dy = atof( fields[2]->value() );
			float dz = atof( fields[3]->value() );
			
			float divisor = sqrt(dx*dx + dy*dy + dz*dz);
			
			if(divisor != 0.0f) {
				dx /= divisor;
				dy /= divisor;
				dz /= divisor;	
			}
			
			fields[0]->value( ftoa(dx) );
			fields[1]->value( ftoa(dy) );
			fields[2]->value( ftoa(dz) );
		}
		
		// commit the values to the return string
		for(vector<Fl_Float_Input*>::iterator i = fields.begin(); i != fields.end(); i++) {
			ret += string((*i)->value()) + " ";	
		}
		
		return ret + "\n";
	}
	
private:

	// label the menu button with the selection
	void getTransformationCallback_real(Transformation* t, Fl_Menu_Button* mb) {
		const Fl_Menu_Item* menu = mb->menu();
		mb->label(menu[mb->value()].label());
		changeFields(mb->label());
		this->parent()->redraw();
	}
	
	// change callbacks based on a new selection
	void changeFields(const char* name) {
		string transformation = string( name );
		
		// remove previous fields
		if(fields.size() != 0) {
			for(vector<Fl_Float_Input*>::iterator i = fields.begin(); i != fields.end(); i++) {
				this->remove(*i);
				delete *i;
			}	
		}
		fields.clear();
		
		// cut out the list of fields
		string::size_type startIndex = format.find("<" + transformation + ":", 0);
		startIndex = format.find("|", startIndex);
		
		string::size_type endIndex = format.find("|>", startIndex);
		
		string fieldList = format.substr(startIndex, endIndex - startIndex + 1);
		
		int x = this->x() + 150;
		int y = this->y();
				
		string fieldName;
		// find all fields and make them
		while(true) {
			// locate the |'s at the start and end
			string::size_type start = fieldList.find("|", 0);
			string::size_type end = fieldList.find("|", start+1);
			
			// get the text between the |'s
			fieldName = fieldList.substr(start+1, end - (start+1));
			
			// add the float field
			Fl_Float_Input* newfield = new Fl_Float_Input(x, y, 100, DEFAULT_TEXTSIZE + 6);
			newfield->copy_label(fieldName.c_str());
			newfield->align(FL_ALIGN_TOP);
			
			fields.push_back(newfield);
			this->add(newfield);
			
			// advance fieldList
			fieldList = fieldList.substr(end);	
			
			// advance X
			x += 120;
			
			// break when we run out of string
			if(fieldList.length() < 2)
				break;
		}
	}
	
	// data
	vector<string> transformTypes;
	Fl_Menu_Button* typeMenu;
	Fl_Check_Button* activeButton;
	vector<Fl_Float_Input*> fields;
	string format;
};
#endif /*TRANSFORMATION_H_*/
