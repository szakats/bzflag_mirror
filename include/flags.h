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

#ifndef FLAGS_H_
#define FLAGS_H_

#include <string>
#include <vector>
#include <stdlib.h>

#define NUM_TEAMFLAGS 4
// list of team flags
static const string _teamFlags[NUM_TEAMFLAGS] = {
	"R*",		// red team
	"G*",		// green team
	"B*",		// blue team
	"P*"		// purple team
};

#define NUM_GOODFLAGS 28
// list of good superflags
static const string _goodFlags[NUM_GOODFLAGS] = {
	"V",		// high speed
	"QT",		// quick turn
	"A",		// agility
	"OO",		// oscillation overthruster
	"F",		// rapid fire
	"MG",		// machine gun
	"GM",		// guided missile
	"L",		// laser
	"R",		// ricochet
	"SB",		// super bullet
	"ST",		// stealth
	"CL",		// cloaking
	"IB",		// invisible bullet
	"T",		// tiny
	"SW",		// shockwave
	"PZ",		// phantom zone
	"G",		// genocide
	"JP",		// jumping
	"ID",		// identify
	"MQ",		// masquerade
	"BU",		// burrow
	"SE",		// seer
	"TH",		// thief
	"US",		// useless
	"WG",		// wings
	"SR",		// steam roller
	"SH",		// shield
	"N"			// narrow
};

#define NUM_BADFLAGS 8
// list of the bad superflags
static const string _badFlags[NUM_BADFLAGS] = {
	"CB",		// colorblind
	"M",		// inertia
	"B",		// blindness
	"JM",		// radar jamming
	"WA",		// wide (fish-eye) angle view
	"NJ",		// no jumping
	"TR",		// trigger happy
	"RC"		// reverse controls
};


/**
 * Flag types (for global access)
 */

class Flag {
	
public:
	// flag parser methods
	 
	 /*
	  * Helper method:  determines whether or not a flag is a team flag
	  */
	 static bool isTeamFlag(const char* flag) {
	 	for(int i = 0; i < NUM_TEAMFLAGS; i++) {
	 		if(_teamFlags[i] == flag)
	 			return true;	
	 	}
	 	return false;
	 }
	 
	 /*
	  * Helper method: determines whether or not a flag is a good flag
	  */
	 
	 static bool isGoodFlag(const char* flag) {
	 	for(int i = 0; i < NUM_GOODFLAGS; i++) {
	 		if(_goodFlags[i] == flag)
	 			return true;	
	 	}
	 	return false;
	 }
	 
	 /*
	  * Helper method: determines whether or not a flag is a bad flag
	  */
	 static bool isBadFlag(const char* flag) {
	 	for(int i = 0; i < NUM_BADFLAGS; i++) {
	 		if(_badFlags[i] == flag)
	 			return true;	
	 	}	
	 	return false;
	 }
	 
	 /*
	 * Helper method:  determines whether or not flag is valid
	 */
	
	static bool isFlag(const char* flag) {
	 	// compare against team flags
	 	return (isTeamFlag(flag) || isGoodFlag(flag) || isBadFlag(flag));
	 }

};


#endif /*FLAGS_H_*/
