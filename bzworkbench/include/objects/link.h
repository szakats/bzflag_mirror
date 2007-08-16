#ifndef LINK_H_
#define LINK_H_

#include "bz2object.h"

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Shape>

class teleporter;

/**
 * Link object between teleporters.
 * Needs to be called "Tlink" because link() is a system call
 * which voids the effect of the constructor
 */
class Tlink : public bz2object {
	
public:
	
	// constructor
	Tlink();
	
	// constructor with data
	Tlink(string& data);
	
	static DataEntry* init() { return new Tlink(); }
	static DataEntry* init(string& data) { return new Tlink(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// getters
	string& getName() { return this->name; }
	teleporter* getFrom() { return from; }
	teleporter* getTo() { return to; }
	
	// setters
	void setName( const string& name ) { this->name = name; }
	void setFrom( teleporter* from ) { this->from = from; }
	void setTo( teleporter* to ) { this->to = to; }
	
private:
	string name;
	teleporter *from;
	teleporter *to;
	
	// build the linkage geometry
	void buildGeometry();
};


#include "teleporter.h"

#endif /*LINK_H_*/
