#ifndef TEXTURESCALERVISITOR_H_
#define TEXTURESCALERVISITOR_H_

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/NodeVisitor>
#include <osg/Drawable>
#include <osg/StateSet>

// forward declare Renderable
class Renderable;

/**
 * A modified osg::NodeVisitor that scales the texture coordinates of a non-mesh object (i.e. BZW 1.x objects)
 * to ensure that their textures don't get stretched or shrunk.
 */
 
class TextureScalerVisitor : public osg::NodeVisitor {

public:
	
	// constructor
	TextureScalerVisitor( Renderable* r, osg::Vec3 scaleFactor );
	
	// apply method for nodes (i.e. calls the apply method for Geode and Group if the passed node is an instance of one of them, otherwise nothing)
	void apply( osg::Node& node );
	
	// apply method for geodes (i.e. gets its geometries and scales the texture coordinates)
	void apply( osg::Geode& geode );
	
	// apply method for groups (i.e. calls the apply method for each of its children)
	void apply( osg::Group& group );
	
private:
	
	// the Renderable this visitor got applied to
	Renderable* renderable;
	
	// the scale factor of the Renderable
	osg::Vec3 scaleFactor;
};
 

#endif /*TEXTURESCALERVISITOR_H_*/
