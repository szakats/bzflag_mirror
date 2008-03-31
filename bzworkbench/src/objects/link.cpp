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

#include "objects/link.h"

// constructor
Tlink::Tlink() : bz2object("link", "<name><from><to>") {
	name = string("default_link");
	from = NULL;
	to = NULL;
}

// constructor with data
Tlink::Tlink(string& data) : bz2object("link", "<name><from><to>") {
	name = string("default_link");
	from = NULL;
	to = NULL;
	
	update(data);	
}

// getter
string Tlink::get(void) { return toString(); }

// setter
int Tlink::update(string& data) {
	const char* header = getHeader().c_str();
	
	// get the data chunks
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
	
	if(!hasOnlyOne(lines, "link"))
		return 0;
		
	// get the data to the block
	const char* linkData = lines[0].c_str();
	
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, linkData);
	if(names.size() > 1) {
		printf("link::update(): Error! Defined \"name\" %d times!\n", (int)names.size());
		return 0;
	}
		
	// get the from
	vector<string> froms = BZWParser::getValuesByKey("from", header, linkData);
	if(!hasOnlyOne(froms, "from"))
		return 0;
		
	// get the to
	vector<string> tos = BZWParser::getValuesByKey("to", header, linkData);
	if(!hasOnlyOne(tos, "to"))
		return 0;
		
	// superclass update
	if(!DataEntry::update(data))
		return 0;
	
	// load in the data
	name = (names.size() != 0 ? names[0] : "");
	
	teleporter* prevFrom = from;
	teleporter* prevTo = to;
	
	from = dynamic_cast< teleporter* > (Model::command( MODEL_GET, "teleporter", froms[0] ));
	to = dynamic_cast< teleporter* > (Model::command( MODEL_GET, "teleporter", tos[0] ));
	
	if( from && to && (prevFrom != from || prevTo != to) )
		buildGeometry();
	
	return 1;
}

// toString
string Tlink::toString(void) {
	string fromName = (from == NULL ? "# from:(unknown)\n" : "  from " + from->getName() + "\n");
	string toName = (to == NULL ? "# to:(unknown)\n" : "  to " + to->getName() + "\n" );
	
	return string("link\n") +
				  (name.length() != 0 ? "  name " + name : "# name") + "\n" +
				  fromName + 
				  toName + 
				  "end\n";
}

// build the link geometry
void Tlink::buildGeometry() {
	// don't draw links if there aren't defined "from" or "to" values
	if( from == NULL || to == NULL )
		return;
		
	// basically, make a yellow line between the teleporters
	osg::ref_ptr< osg::Cylinder > line;
	
	// get the positions of the teleporters
	osg::Vec3 fromPos = from->getPos();
	osg::Vec3 toPos = to->getPos();
	
	// raise the positions to the top of the teleporters
	fromPos.set( fromPos.x(), fromPos.y(), fromPos.z() + from->getSize().z() );
	toPos.set( toPos.x(), toPos.y(), toPos.z() + to->getSize().z() );
	
	
}
