#ifndef CONFIGURATIONDIALOG_H_
#define CONFIGURATIONDIALOG_H_

#include "Fl_Dialog.h"
#include "../DataEntry.h"

/**
 * This is the base class of all object configuration dialogs.
 */
 
class ConfigurationDialog : public Fl_Dialog {

public:

	// constructor
	ConfigurationDialog( DataEntry* data, const char* title, int width, int height ) :
		Fl_Dialog( title, width, height, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL )
		{ this->data = data; }
	
	// static initializer
	static ConfigurationDialog* init( DataEntry* data, const char* title, int width, int height ) { return new ConfigurationDialog( data, title, width, height ); }
	
	// destructor
	virtual ~ConfigurationDialog() { }
	
protected:

	// the data itself
	DataEntry* data;
};

#endif /*CONFIGURATIONDIALOG_H_*/
