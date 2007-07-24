#ifndef TEXTUREREPEATERVISITOR_H_
#define TEXTUREREPEATERVISITOR_H_

#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Drawable>
#include <osg/StateSet>

#include <vector>

using namespace std;

/**
 * A modified osg::NodeVisitor that changes the texture wrapping mode to GL_REPEAT for the node's
 * (and child nodes') textures.
 */
 
class TextureRepeaterVisitor : public osg::NodeVisitor {
	
public:
	
	// constructor
	TextureRepeaterVisitor();
	
	// destructor
	virtual ~TextureRepeaterVisitor();
	
	// override the apply() method for nodes (i.e. so we can get their textures, if they exist)
	void apply(osg::Node& node);
	
	// override the apply() method for geodes (i.e. so we can get their textures, if they exist)
	void apply(osg::Geode& geode);
	
	// override the apply() method for groups (i.e. so we can get their children, and then call the appropriate apply methods)
	void apply(osg::Group& group);
	
};

#endif /*TEXTUREREPEATERVISITOR_H_*/
