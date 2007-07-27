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
#include <osg/Material>
#include <osg/StateAttribute>
#include <osgDB/ReaderWriter>

#include "../objects/bz2object.h"
#include "../render/Renderable.h"
#include "../ftoa.h"

#include "../render/TextureRepeaterVisitor.h"

#define SCENEBUILDER_TAIL_NODE			"|SBnode"
#define SCENEBUILDER_TAIL_GEOMETRY		"|SBgeometry"
#define SCENEBUILDER_TAIL_TEXTURE2D		"|SBtexture2d"
#define SCENEBUILDER_TAIL_SELECTED		"|SBselected"

#include <map>
#include <string>

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
	
	// build an object and return a node containing the object.
	static osg::Node* buildNode( const char* nodeFile );
	
	// build a selected node
	static osg::Node* buildSelectedNode( const char* nodeFile );
	
	// mark a node as selected
	static void markSelected( osg::Node* node );
	
	// mark a node as unselected
	static void markUnselected( osg::Node* node );
	
	// build an object and return a geode containing the object
	static osg::Geode* buildGeode( const char* nodeName, osg::Vec3Array* vertexes, osg::DrawElementsUInt* indexes, osg::Vec2Array* texCoords, const char* textureFile );
	static osg::Geode* buildGeode( const char* nodeName, osg::Geometry*, const char* textureName );
	
	// get the geometry data from a node
	static const vector< osg::ref_ptr< osg::Drawable > >* getNodeGeometry( osg::PositionAttitudeTransform* node );
	
	// assign a texture to a node
	static void assignTexture( const char* filename, osg::Node* node );
	
	// assign a material to a node
	static void assignMaterial( osg::Vec4 ambient, osg::Vec4 diffuse, osg::Vec4 specular, osg::Vec4 emissive, float shininess, float alpha, osg::Node* node );
	
	// give the string the "select" attribute
	static string nameSelected( const char* str ) {
		return nameUnselected( str ) + SCENEBUILDER_TAIL_SELECTED;
	}
	
	// remove the "select" attribute from a string
	static string nameUnselected( const char* str ) {
		string text = string(str);
		
		string::size_type start = text.find(SCENEBUILDER_TAIL_SELECTED, 0);
		string::size_type end = text.find("|", start+1);
		
		string ret = text.substr(0, start);
		
		if(end != string::npos)
			ret += text.substr(end);
			
		return ret;	
	}
	
	// get the base filename from a string
	static string baseName( const char* str ) {
		string text = string(str);
		
		string::size_type end = text.find("|", 0);
		if(end != string::npos)
			return text.substr(0, end-1);
		
		return text;	
	}
	
	// indicate that this is a node
	static string nameNode( const char* str ) {
		return itoa( ++nameCount ) + "|" + str + SCENEBUILDER_TAIL_NODE;	
	}
	
	// indicate that this is a selected node
	static string nameSelectedNode( const char* str) {
		return nameSelected( nameNode( str ).c_str() );
	}
	
private:
	
	/*
	// map strings to already-loaded nodes
	static map< string, osg::Node* > nodeData;
	
	// map strings to geometry data for already-loaded geodes
	static map< string, osg::Geometry* > geoData;
	
	// map strings to texture data for already-loaded geodes
	static map< string, osg::Texture2D* > textureData;
	*/
	
	// this gets attached to the end of any name generated from nameNode()
	// to make each node name identical
	static int nameCount;
	
};

#endif /*SCENEDATA_H_*/
