#ifndef CONE_H_
#define CONE_H_

#include "bz2object.h"

#include "../model/SceneBuilder.h"
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/ShadeModel>

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
	int update(UpdateMessage& msg);
	
	// toString
	string toString(void);
	
	// getters/setters
	bool hasFlatShading() { return flatShading; }
	bool hasSmoothBounce() { return smoothbounce; }
	float getSweepAngle() { return sweepAngle; }
	int getDivisions() { return divisions; }
	
	void setFlatShading(bool value) {
		this->flatShading = value;
		this->updateShadeModel();
	}
	
	void setSmoothBounce(bool value) { this->smoothbounce = value; }
	
	void setSweepAngle(float value) {
		if( value != this->sweepAngle ) {		// refresh the geometry
			theCone->removeChild( coneNode.get() );
			theCone->removeChild( baseNode.get() );
			
			this->buildGeometry();
		}
		
		this->sweepAngle = value;
	}
	
	void setDivisions(int value) {
		if( value != this->divisions ) {	// refresh the geometry
			theCone->removeChild( coneNode.get() );
			theCone->removeChild( baseNode.get() );
			
			this->buildGeometry();
		}
		
		this->divisions = value;
	}
	
private:
	bool flatShading, smoothbounce;
	int divisions;
	
	// sweep angle
	float sweepAngle;
	
	// reference to the nodes that make up the cone
	osg::ref_ptr< osg::Geode > coneNode;
	osg::ref_ptr< osg::Geode > baseNode;
	osg::ref_ptr< osg::Geode > crossSectionNode;	// this is only used when sweepAngle < 360
	
	osg::ref_ptr< osg::Group > theCone;	// the group containing the aforementioned geodes
	
	// helper method to build the geometry
	void buildGeometry();
	
	// helper method to change the node's shading (flat-shaded or smooth-shaded)
	void updateShadeModel();
	
};

#endif /*CONE_H_*/
