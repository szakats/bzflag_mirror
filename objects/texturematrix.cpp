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

#include "../include/objects/texturematrix.h"

// default constructor
texturematrix::texturematrix() :
	DataEntry("texturematrix", "<name><scale><spin><shift><center><fixedscale><fixedspin><fixedshift>") {
	
	
	this->name = string("default_texturematrix");
	this->fixedSpin = 0.0f;
	this->texCenter = TexCoord2D();
	this->texFixedScale = TexCoord2D();
	this->texFixedShift = TexCoord2D();
	this->texFreq = TexCoord2D();
	this->texScale = TexCoord2D();
	this->texShift = TexCoord2D();
	this->spin = 0.0f;
}

// constructor with data
texturematrix::texturematrix(string& data) :
	DataEntry("texturematrix", "<name><scale><spin><shift><center><fixedscale><fixedspin><fixedshift>", data.c_str()) {
		
}

// getter
string texturematrix::get(void) { return this->toString(); }

// setter
int texturematrix::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the data
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	// break if not found
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
	
	// break if too many found
	if(!hasOnlyOne(lines, header))
		return 0;
		
	// get the pointer to the data
	const char* texmatData = lines[0].c_str();
	
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, texmatData);
	if(!hasOnlyOne(names, "name"))
		return 0;
		
	// get the scale
	vector<string> scales = BZWParser::getValuesByKey("scale", header, texmatData);
	if(!hasOnlyOne(scales, "scale") || !hasNumElements(scales[0], 4))
		return 0;
		
	// get the spin
	vector<string> spins = BZWParser::getValuesByKey("spin", header, texmatData);
	if(!hasOnlyOne(spins, "spin") || !hasNumElements(spins[0], 1))
		return 0;
		
	// get the shift
	vector<string> shifts = BZWParser::getValuesByKey("shift", header, texmatData);
	if(!hasOnlyOne(shifts, "shift") || !hasNumElements(shifts[0], 2))
		return 0;
		
	// get the center
	vector<string> centers = BZWParser::getValuesByKey("center", header, texmatData);
	if(!hasOnlyOne(centers, "center") || !hasNumElements(centers[0], 2))
		return 0;
	
	// get fixed scale
	vector<string> fixedScales = BZWParser::getValuesByKey("fixedscale", header, texmatData);
	if(!hasOnlyOne(fixedScales, "fixedscale") || !hasNumElements(fixedScales[0], 2))
		return 0;
		
	// get fixed spin
	vector<string> fixedSpins = BZWParser::getValuesByKey("fixedspin", header, texmatData);
	if(!hasOnlyOne(fixedSpins, "fixedspin") || !hasNumElements(fixedSpins[0], 1))
		return 0;
		
	// get fixed shift
	vector<string> fixedShifts = BZWParser::getValuesByKey("fixedshift", header, texmatData);
	if(!hasOnlyOne(fixedShifts, "fixedshift") || !hasNumElements(fixedShifts[0], 2))
		return 0;
	
	// do base class update
	if(!DataEntry::update(data))
		return 0;
		
	// read in the data
	// first, break up the two value pairs of scales[0]
	vector<string> texScaleLineElements = BZWParser::getLineElements( scales[0].c_str() );
	string texFreqs = texScaleLineElements[0] + " " + texScaleLineElements[1] + "\0";
	string texScales = texScaleLineElements[2] + " " + texScaleLineElements[3] + "\0";
	
	this->name = names[0];
	this->texScale = TexCoord2D( texScales.c_str() );
	this->spin = atof( spins[0].c_str() );
	this->texCenter = TexCoord2D( centers[0].c_str() );
	this->texFixedScale = TexCoord2D( fixedScales[0].c_str() );
	this->texFixedShift = TexCoord2D( fixedShifts[0].c_str() );
	this->texFreq = TexCoord2D( texFreqs.c_str() );
	this->texShift = TexCoord2D( shifts[0].c_str() );
	this->fixedSpin = atof( fixedSpins[0].c_str() );
	
	return 1;
}

// toString
string texturematrix::toString(void) {
	return string("texturematrix\n") +
				  "  name " + name + "\n" +
				  "  scale " + texFreq.toString() + " " + texScale.toString() + "\n" +
				  "  spin " + string(ftoa(spin)) + "\n" +
				  "  shift " + texShift.toString() + "\n" +
				  "  center " + texCenter.toString() + "\n" +
				  "  fixedscale " + texFixedScale.toString() + "\n" +
				  "  fixedspin " + string(ftoa(fixedSpin)) + "\n" +
				  "  fixedshift " + texFixedShift.toString() + "\n" +
				  "end\n";
}

// render
int texturematrix::render(void) {
	return 0;
	
}
