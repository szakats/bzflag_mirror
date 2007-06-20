#ifndef DEFINE_H_
#define DEFINE_H_

#include "../DataEntry.h"

#include "bz2object.h"

#include "arc.h"
#include "base.h"
#include "box.h"
#include "cone.h"
#include "group.h"
#include "mesh.h"
#include "meshbox.h"
#include "meshpyr.h"
#include "pyramid.h"
#include "sphere.h"
#include "teleporter.h"
#include "tetra.h"


class define : public DataEntry {
	
public:
	
	// constructor
	define();
	
	// constructor with data
	define(string& data);
	
	// destructor
	~define();
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	// name
	string name;

	// the objects in the definition 
	vector<bz2object*> objects;
};

#endif /*DEFINE_H_*/
