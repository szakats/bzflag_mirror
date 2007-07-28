#ifndef BOX_H_
#define BOX_H_

#include "../DataEntry.h"
#include "../render/Point3D.h"
#include "../ftoa.h"
#include "../model/BZWParser.h"
#include "bz2object.h"

#include "../model/SceneBuilder.h"

#include "../render/GeometryExtractorVisitor.h"

#include <osg/Geometry>
#include <osg/PrimitiveSet>

/**
 * Box data
 */
class box : public bz2object {

public:

	// constructor
	box();
	
	// constructor with binary data
	box( osg::Vec3 position, float rotation, osg::Vec3 scale );
	
	// constructor with data
	box(string& data);
	
	static DataEntry* init() { return new box( osg::Vec3( 0.0, 0.0, 0.0 ), 0.0f, osg::Vec3( 10.0, 10.0, 10.0 ) ); }
	static DataEntry* init(string& data) {
		if( data.size() <= 1 )
			return new box( osg::Vec3( 0.0, 0.0, 0.0 ), 0.0f, osg::Vec3( 10.0, 10.0, 10.0 ) );
		else
			return new box(data);
	}
	
	// nothing to destroy...
	virtual ~box();
	
	// getter
	string get(void);
	
	// setters
	int update(string& data);
	int update(string& data, UpdateMessage& msg);
	
	// toString
	string toString(void);
	
private:
	Point3D size;
	
	// update the box's geometry
	void updateGeometry( UpdateMessage& msg );
	
};

#endif /*BOX_H_*/
