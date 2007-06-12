#ifndef CONE_H_
#define CONE_H_

#include "bz2object.h"

class cone : public bz2object {
public:
	// default constructor
	cone();
	
	// constructor with data
	cone(string& data);
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	bool flatShading, smoothbounce;
	int divisions;
	
};

#endif /*CONE_H_*/
