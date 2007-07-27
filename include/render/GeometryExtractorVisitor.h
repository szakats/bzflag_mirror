#ifndef GEODEEXTRACTORVISITOR_H_
#define GEODEEXTRACTORVISITOR_H_

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Node>
#include <osg/Group>
#include <osg/NodeVisitor>

#include <vector>

using namespace std;

/**
 * This visitor class traverses a node and extracts all of it's geometries.
 * Just constructing one of these classes will cause a traversal.
 */
 
class GeometryExtractorVisitor : public osg::NodeVisitor {

public:
	
	// constructor (takes a renderable)
	GeometryExtractorVisitor( osg::Node* node );
	
	// destructor (does nothing)
	virtual ~GeometryExtractorVisitor() { }
	
	// get the geometries that were extracted
	vector< osg::Geometry* >& getGeometries() { return this->geometries; }
	
	// override the apply() method for nodes.
	// This method will attempt to cast the node as a geode or a group and
	// forward it to the other apply methods
	void apply( osg::Node& node );
	
	// override the apply() method for geodes.
	// This is where the bulk of the work is done--this method extracts the
	// Drawables from the geode and attempts to cast them as Geometries.  If
	// the cast works, they are stored to the cache.
	void apply( osg::Geode& node );
	
	// override the apply() method for groups.
	// This method iterates through all of the group's children and calls
	// the apply() method on each
	void apply( osg::Group& node );
	
	
private:

	// geometry cache
	vector< osg::Geometry* > geometries;
	
};

#endif /*GEODEEXTRACTORVISITOR_H_*/
