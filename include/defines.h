#ifndef DEFINES_H_
#define DEFINES_H_

#include <string>
using namespace std;

#define DEFAULT_TEXTSIZE 14

#define BASE_RED 1
#define BASE_GREEN 2
#define BASE_BLUE 3
#define BASE_PURPLE 4

// for windows users...
#define _CRT_SECURE_NO_DEPRECATE

/**
 * Flag types (for global access)
 */
 

#define NUM_TEAMFLAGS 4
// list of team flags
const string _teamFlags[NUM_TEAMFLAGS] = {
	"R*",		// red team
	"G*",		// green team
	"B*",		// blue team
	"P*"		// purple team
};

#define NUM_GOODFLAGS 26
// list of good superflags
const string _goodFlags[NUM_GOODFLAGS] = {
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
	"SR"		// steam roller
};

#define NUM_BADFLAGS 8
// list of the bad superflags
const string _badFlags[NUM_BADFLAGS] = {
	"CB",		// colorblind
	"M",		// inertia
	"B",		// blindness
	"JM",		// radar jamming
	"WA",		// wide (fish-eye) angle view
	"NJ",		// no jumping
	"TR",		// trigger happy
	"RC"		// reverse controls
};

#endif /*DEFINES_H_*/
