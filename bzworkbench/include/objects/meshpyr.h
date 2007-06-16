#ifndef MESHPYR_H_
#define MESHPYR_H_

#include "bz2object.h"

class meshpyr : public bz2object {
public:
	
	meshpyr();
	meshpyr(string& data);
	
	static DataEntry* init() { return new meshpyr(); }
	static DataEntry* init(string& data) { return new meshpyr(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString(void);
	
	// render
	int render(void);
	
};

#endif /*MESHPYR_H_*/
