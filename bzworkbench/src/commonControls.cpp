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

#include "commonControls.h"
#include <FL/Fl_File_Chooser.H>

std::map<bzwb_eCommonControlType,controlHanderList>	commonControlHandlers;

controlHanderList getControlHandlers ( bzwb_eCommonControlType type )
{
	controlHanderList list;
	if ( commonControlHandlers.find(type) == commonControlHandlers.end())
		return list;
	return commonControlHandlers.find(type)->second;
}

void addControlHandler ( bzwb_eCommonControlType type, bzwb_BaseCommonControlHandler* handler )
{
	if (!handler )
		return;

	if ( commonControlHandlers.find(type) == commonControlHandlers.end())
	{
		controlHanderList list;
		commonControlHandlers[type] = list;
	}
	commonControlHandlers[type].push_back(handler);
}

void removeControlHandler ( bzwb_eCommonControlType type, bzwb_BaseCommonControlHandler* handler )
{
	if (!handler)
		return;

	if ( commonControlHandlers.find(type) == commonControlHandlers.end())
		return;
	
	std::map<bzwb_eCommonControlType,controlHanderList>::iterator itr = commonControlHandlers.find(type);
	for ( int i = 0; i < (int) itr->second.size(); i++ )
	{
		if (handler == itr->second[i])
		{
			itr->second.erase(itr->second.begin()+i);
			return;
		}
	}
}

bool callOpenFileDialog ( std::string  &result, const char* file, const char *dir, const char *extension, const char* description  )
{
	// see if anyone wants to handle it.
	controlHanderList handlers = getControlHandlers(eOpenFileDialog);
	for (int i = 0; i < (int)handlers.size(); i++)
	{
		bzw_OpenDialogControlHandler *handler = (bzw_OpenDialogControlHandler*)handlers[i];

		handler->file = file;
		handler->extension = extension;
		handler->directory = dir;
		handler->sucsessful = false;
		handler->description = description;
		handler->parent = bzwb_getOSMainWindowHandle();

		if ( handlers[i]->handle() )
		{
			result = handler->file;
			return handler->sucsessful;
		}
	}

	//if not, let the FLTK do it
	Fl_File_Chooser* fc = new Fl_File_Chooser(dir, extension, Fl_File_Chooser::SINGLE, description);
	fc->value(file);
	fc->show();

	// wait for a value
	while( fc->shown() ) { Fl::wait(); }

	// get a value (the selected filename )
	if(fc->value() == NULL)
		return false;

	result = fc->value();
	return true;
}

bool callSaveFileDialog ( std::string  &result, const char* file, const char *dir, const char *extension, const char* description )
{
	// see if anyone wants to handle it.
	controlHanderList handlers = getControlHandlers(eSaveFileDialog);
	for (int i = 0; i < (int)handlers.size(); i++)
	{
		bzw_SaveDialogControlHandler *handler = (bzw_SaveDialogControlHandler*)handlers[i];

		handler->file = file;
		handler->extension = extension;
		handler->directory = dir;
		handler->sucsessful = false;
		handler->description = description;
		handler->parent = bzwb_getOSMainWindowHandle();

		if ( handlers[i]->handle() )
		{
			result = handler->file;
			return handler->sucsessful;
		}
	}

	Fl_File_Chooser* fc = new Fl_File_Chooser(dir, extension, Fl_File_Chooser::CREATE, description);
	
	if(file)
		fc->value(file);
	fc->show();

	// wait for a value
	while( fc->shown() ) { Fl::wait(); }

	if(fc->value() == NULL)
		return false;

	result = fc->value();
	return true;
}

