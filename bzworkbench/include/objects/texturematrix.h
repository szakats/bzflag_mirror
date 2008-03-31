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

#ifndef TEXTUREMATRIX_H_
#define TEXTUREMATRIX_H_

#include "../DataEntry.h"
#include "../model/BZWParser.h"
#include "../render/TexCoord2D.h"

class texturematrix : public DataEntry {

public:
	
	// default constructor
	texturematrix();
	
	// constructor with data
	texturematrix(string& data);
	
	static DataEntry* init() { return new texturematrix(); }
	static DataEntry* init(string& data) { return new texturematrix(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
	// getters
	string& getName() { return this->name; }
	
	// setters
	void setName( const string& _name ) { this->name = _name; }
	
private:

	string name;
	TexCoord2D texFreq, texScale, texShift, texCenter, texFixedScale, texFixedShift;
	float spin, fixedSpin;
	
};


#endif /*TEXTUREMATRIX_H_*/
