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
	int update(UpdateMessage& message);
	
	// override the getSize and setSize methods
	osg::Vec3 getSize() { return osg::Vec3( realSize.x(), realSize.y(), realSize.z() ); }
	void setSize( osg::Vec3 newSize ) {
		this->realSize = newSize;
		UpdateMessage msg = UpdateMessage( UpdateMessage::SET_SCALE, &newSize );
		this->updateGeometry( msg );
	}
	
	// tostring
	string toString(void);
	
	// binary getters and setters
	float getBorder() { return border; }
	
	void setBorder( float b ) { this->border = b; }
	
private:
	
	float border;
	
	// parts of the teleporter node (necessary because teleporters don't scale normally, and we'll need to intervene :) )
	osg::ref_ptr< Renderable > leftLeg, rightLeg, topBeam, frontPortal, rearPortal;
	
	// the teleporter group
	osg::ref_ptr< osg::Group > theTeleporter;
	
	// helper method: update the teleporter nodes
	void updateGeometry( UpdateMessage& message );
	
	// the real size (not reflected in the node geometry)
	Point3D realSize;
};

#endif /*TELEPORTER_H_*/
