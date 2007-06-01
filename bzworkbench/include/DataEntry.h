#ifndef DATAENTRY_H_
#define DATAENTRY_H_

#include <stdio.h>
#include <string>

using namespace std;

// every class the model stores must be derived from this:
class DataEntry {
public:
	virtual void get(string& data) = 0; 
	virtual void update(string& data) = 0;
	virtual string toString() {
		return string("# unknown DataEntry");
	}
	
	DataEntry() { 
	
	}
	virtual ~DataEntry() {
	
	}
};

#endif /*DATAENTRY_H_*/
