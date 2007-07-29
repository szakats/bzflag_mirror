#ifndef TELEPORTER_H_
#define TELEPORTER_H_

#include "bz2object.h"
#include "../model/SceneBuilder.h"
#include "../render/Point3D.h"

#include <string>
#include <vector>

using namespace std;

class teleporter : public bz2object {
public:
	
	// constructor
	teleporter();
	
	// constructor with data
	teleporter(string& data);
	
	static DataEntry* init() {
		return new teleporter();
	}
	static DataEntry* init(string& data) { return new teleporter(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	int update(string& data, UpdateMessage& message);
	
	// tostring
	string toString(void);
	
	// binary getters and setters
	float getBorder() { return border; }
	string getLinkName() { return lname; }
	
	void setBorder( float b ) { this->border = b; }
	void setLinkName( const char* s ) { this->lname = s; }
	
private:
	
	float border;
	string lname;	// used for linkage; not the official name
	
	// parts of the teleporter node (necessary because teleporters don't scale normally, and we'll need to intervene :) )
	osg::ref_ptr< Renderable > leftLeg, rightLeg, topBeam, frontPortal, rearPortal;
	
	// helper method: update the teleporter nodes
	void updateGeometry( UpdateMessage& message );
	
	// the real size (not reflected in the node geometry)
	Point3D realSize;
};

#endif /*TELEPORTER_H_*/
