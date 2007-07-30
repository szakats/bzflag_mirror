#ifndef CONE_H_
#define CONE_H_

#include "bz2object.h"

#include "../model/SceneBuilder.h"
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>

#include <math.h>

class cone : public bz2object {
public:
	// default constructor
	cone();
	
	// constructor with data
	cone(string& data);
	
	static DataEntry* init() { return new cone(); }
	static DataEntry* init(string& data) { return new cone(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
private:
	bool flatShading, smoothbounce;
	int divisions;
	
	// reference to the nodes that make up the cone
	osg::ref_ptr< osg::Geode > coneNode;
	osg::ref_ptr< osg::Geode > baseNode;
	
	// helper method to build the geometry
	void buildGeometry();
	
};

#endif /*CONE_H_*/
