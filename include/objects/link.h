#ifndef LINK_H_
#define LINK_H_

#include "../DataEntry.h"
#include "../model/BZWParser.h"

/**
 * Link object between teleporters.
 * Needs to be called "Tlink" because link() is a system call
 * which voids the effect of the constructor
 */
class Tlink : public DataEntry {
	
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
	
	// render
	int render(void);
	
	// getters
	string& getName() { return this->name; }
	
	// setters
	void setName( const string& name ) { this->name = name; }
	
private:
	string name, from, to;
};

#endif /*LINK_H_*/
