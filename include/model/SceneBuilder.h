#ifndef SCENEDATA_H_
#define SCENEDATA_H_

#include <osg/Geode>
#include <osg/Group>
#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osg/CopyOp>
#include <osgDB/ReadFile>

#define SCENEBUILDER_TAIL_NODE			"|node"
#define SCENEBUILDER_TAIL_GEOMETRY		"|geometry"
#define SCENEBUILDER_TAIL_TEXTURE2D		"|texture2d"
#define SCENEBUILDER_TAIL_SELECTED		"|selected"

#include <map>

using namespace std;

/**
 * This is a data broker that stores single instances of scene data to save memory, and just passes out references.
 * This is an API class; methods are accessed statically.
 */
class SceneBuilder {
	
public:
	
	// initialize
	static bool init();
	
	// shut down
	static bool shutdown();
	
	// build an object and return a node containing the object
	static osg::ref_ptr< osg::Node > buildNode( const char* nodeFile, bool loadSelectedToo = false );
	
	// build an object and return a geode containing the object
	static osg::ref_ptr< osg::Geode > buildGeode( const char* nodeName, osg::Vec3Array* vertexes, osg::DrawElementsUInt* indexes, osg::Vec2Array* texCoords, const char* textureFile, bool loadSelectedToo = false );
	static osg::ref_ptr< osg::Geode > buildGeode( const char* nodeName, osg::Geometry*, const char* textureName, bool loadSelectedToo = false );
	
	// return a PositionAttitudeTransform node encapsulating the node
	static osg::ref_ptr< osg::PositionAttitudeTransform > transformable( osg::Node* node );
	
	// get the geometry data from a node
	static const vector< osg::ref_ptr<osg::Drawable> >* getNodeGeometry( osg::PositionAttitudeTransform* node );
	
	// get all children from a group
	static vector< osg::ref_ptr<osg::Node> >* extractChildren( osg::Group* group );
	
private:

	// map strings to already-loaded nodes
	static map< string, osg::ref_ptr<osg::Node> > nodeData;
	
	// map strings to geometry data for already-loaded geodes
	static map< string, osg::ref_ptr<osg::Geometry> > geoData;
	
	// map strings to texture data for already-loaded geodes
	static map< string, osg::ref_ptr<osg::Texture2D> > textureData;
	
};

#endif /*SCENEDATA_H_*/
