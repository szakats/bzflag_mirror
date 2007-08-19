#ifndef DEFINE_H_
#define DEFINE_H_

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
	
	// static constructor
	static DataEntry* init() { return new define(); }
	static DataEntry* init(string& data) { return new define(data); }
	
	// destructor
	~define();
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// getters
	string& getName() { return this->name; }
	vector< osg::ref_ptr< bz2object > >& getObjects() { return this->objects; }
	
	// setters
	void setName( const string& name ) { this->name = name; }
	void setObjects( vector< osg::ref_ptr< bz2object > >& objects ) { this->objects = objects; }
	
private:
	// name
	string name;

	// the objects in the definition 
	vector< osg::ref_ptr< bz2object > > objects;
};

#endif /*DEFINE_H_*/
