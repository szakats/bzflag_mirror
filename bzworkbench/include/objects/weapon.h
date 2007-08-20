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

#ifndef WEAPON_H_
#define WEAPON_H_

#include "bz2object.h"
#include "../flags.h"


class weapon : public bz2object {

public:
	
	// default constructor
	weapon();
	
	// constructor with data
	weapon(string& data);
	
	static DataEntry* init() { return new weapon(); }
	static DataEntry* init(string& data) { return new weapon(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	string type, trigger, eventTeam;
	float initdelay, tilt;
	vector<float> delay;
	int team;
};

#endif /*WEAPON_H_*/
