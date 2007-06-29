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

#define SCENEBUILDER_ABSOLUTE_NODE_NAME(s) ( s + SCENEBUILER_TAIL_NODE )
#define SCENEBUILDER_ABSULTE_SELECTED_NODE_NAME(s) ( SCENEBUILDER_NODE_NAME(s) + SCENEBUILDER_TAIL_SELECTED )
#define SCENEBUILDER_SELECTED_NODE_NAME(s) ( s + SCENEBUILDER_TAIL_SELECTED )

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
	
	// see if a particular name is already mapped to a node
	static bool alreadyLoaded( const char* _name ) {
		string name = string( _name );
		if( nodeData[ name ].get() != NULL )
			return true;
		else
			return false;
	}
	
	// get a node by name
	static osg::Node* getNode( const char* _name ) {
		string name	= string( _name );
		return nodeData[ name ].get();
	}
	
	// give the string the "select" attribute
	static string setSelected( const char* str ) {
		return setUnselected( str ) + SCENEBUILDER_TAIL_SELECTED;
	}
	
	// remove the "select" attribute from a string
	static string setUnselected( const char* str ) {
		string text = string(str);
		
		string::size_type start = text.find(SCENEBUILDER_TAIL_SELECTED, 0);
		string::size_type end = text.find("|", start+1);
		
		string ret = text.substr(0, start);
		
		if(end != string::npos)
			ret += text.substr(end);
			
		return ret;	
	}
	
private:

	// map strings to already-loaded nodes
	static map< string, osg::ref_ptr<osg::Node> > nodeData;
	
	// map strings to geometry data for already-loaded geodes
	static map< string, osg::ref_ptr<osg::Geometry> > geoData;
	
	// map strings to texture data for already-loaded geodes
	static map< string, osg::ref_ptr<osg::Texture2D> > textureData;
	
};

#endif /*SCENEDATA_H_*/
