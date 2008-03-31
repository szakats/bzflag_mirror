/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef VIEWMESSAGE_H_
#define VIEWMESSAGE_H_

/**
 * This is a messaging class that the Model sends to the Observers.
 */
 
class ObserverMessage {

public:

	
	typedef enum ObserverMessageType {
		UPDATE_OBJECT = 1,	// update an object
		REMOVE_OBJECT,		// remove an object
		ADD_OBJECT,			// add an object
		UPDATE_WORLD,		// re-size the world
		UPDATE_WATERLEVEL	// alter the water level
	};
	
	ObserverMessageType type;
	void* data;
	
	ObserverMessage(ObserverMessageType _type, void* _data) { this->type = _type; this->data = _data; }
	
};

#endif /*VIEWMESSAGE_H_*/
