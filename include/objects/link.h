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
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	string name, from, to;
};

#endif /*LINK_H_*/
