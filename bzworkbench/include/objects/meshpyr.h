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

#ifndef MESHPYR_H_
#define MESHPYR_H_

#include "objects/cone.h"

class meshpyr : public cone {
public:
	
	meshpyr();
	meshpyr(std::string& data);
	
	static DataEntry* init() { return new meshpyr(); }
	static DataEntry* init(string& data) { return new meshpyr(data); }
	
	// restore default values
	virtual void setDefaults();
};

#endif /*MESHPYR_H_*/
