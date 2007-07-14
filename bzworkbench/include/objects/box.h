#ifndef BOX_H_
#define BOX_H_

#include "../DataEntry.h"
#include "../render/Point3D.h"
#include "../ftoa.h"
#include "../model/BZWParser.h"
#include "bz2object.h"

#include "../model/SceneBuilder.h"

/**
 * Box data
 */
class box : public bz2object {

public:

	// constructor
	box();
	
	// constructor with data
	box(string& data);
	
	static DataEntry* init() { return new box(); }
	static DataEntry* init(string& data) { return new box(data); }
	
	// nothing to destroy...
	virtual ~box();
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
private:
	Point3D size;
	
};

#endif /*BOX_H_*/
