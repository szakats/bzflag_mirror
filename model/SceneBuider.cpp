#include "../include/model/SceneBuilder.h"

// static members of SceneBuilder
map< string, osg::ref_ptr<osg::Node> > SceneBuilder::nodeData;
map< string, osg::ref_ptr<osg::Geometry> > SceneBuilder::geoData;
map< string, osg::ref_ptr<osg::Texture2D> > SceneBuilder::textureData;

// constructor
bool SceneBuilder::init() {
	nodeData = map< string, osg::ref_ptr<osg::Node> >();
	geoData = map< string, osg::ref_ptr<osg::Geometry> >();
	textureData = map< string, osg::ref_ptr<osg::Texture2D> >();
	return true;
}

// destructor
bool SceneBuilder::shutdown() {
	return true;
}

/**
 * Object builder.
 * This method builds and returns a node loaded from nodeFile
 */
osg::ref_ptr< osg::Node > SceneBuilder::buildNode( const char* nodeFile ) {
	// string-ify the nodeFile and add the corresponsing tail
	string nodeName = string( nodeFile ) + SCENEBUILDER_TAIL_NODE;
	
	// see if this node was already loaded
	if( nodeData[ nodeName ] != NULL ) {
		return nodeData[ nodeName ];
	}
	
	// otherwise, load it in
	nodeData[ nodeName ] = osgDB::readNodeFile( nodeFile );
	
	return nodeData[nodeName];
}

/**
 * Geometry builder
 * This method builds and returns a geometry node (a geode) from arrays of vertexes, indexes, texture coordinates, and
 * a texture filename
 */
osg::ref_ptr< osg::Geode > SceneBuilder::buildGeode( const char* _nodeName, osg::Geometry* geometry, const char* textureFile ) {
	// make the node name
	string nodeName = string(_nodeName) + SCENEBUILDER_TAIL_NODE;
	
	// make the geometry name
	string geometryName = string(_nodeName) + SCENEBUILDER_TAIL_GEOMETRY;
	
	// string-ify the texture name
	string textureName = string(textureFile) + SCENEBUILDER_TAIL_TEXTURE2D;
	
	// make a geode
	osg::Geode* geode = new osg::Geode();
	
	// load up the texture
    // load the image itself
	if(textureFile != NULL) {
		// the texture itself
		osg::Texture2D* texture = NULL;
		
		// if the texture has already been loaded then initialize it
		if( textureData[ textureName ] != NULL) {
			texture = textureData[ textureName ].get();
		}
		else {
			texture = new osg::Texture2D();
			
			osg::Image* image = osgDB::readImageFile( textureFile );
		
			if( image != NULL ) {	// only build the texture if the image exists!
			
				// don't allow OSG to optimize the texture (otherwise it may disappear)
				texture->setDataVariance( osg::Object::DYNAMIC );
			
				// set the texture's image
				texture->setImage( image );
				
				// turn on GL_REPEAT texture wrapping
				texture->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
				texture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
				texture->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT ); 
			}
			
			textureData[ textureName ] = texture;
		}
		 
 		// make a new state set for the texture (so we can manipulate the texture attributes)
		osg::StateSet* texStateSet = new osg::StateSet();
		
		// assign the texture to the state set and activate it
		texStateSet->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
		
		// finally, attach the texture to the geode
		geode->setStateSet( texStateSet );
	}
	
	// assign the geometry data to the geode (assign it the flyweighted one if it exists; otherwise add this geometry)
	if( geoData[ geometryName ] == NULL ) {
		geoData[ geometryName ] = geometry;
	}
	
	geode->addDrawable( geoData[ geometryName ].get() );
	
	// add this geode to nodeData to be reused again
	if( nodeData[ nodeName ] == NULL )
		nodeData[ nodeName ] = (osg::Node*)(geode);
	
	return geode;
}

// build a geometry and call the other buildGeode method
osg::ref_ptr< osg::Geode > SceneBuilder::buildGeode( const char* nodeName, osg::Vec3Array* vertexes, osg::DrawElementsUInt* indexes, osg::Vec2Array* texCoords, const char* textureName ) {
	// don't bother if the data is NULL
	if(vertexes == NULL || indexes == NULL || texCoords == NULL)
		return NULL;
	
	// make a new geometry
	osg::Geometry* geometry = new osg::Geometry();
	
	// assign the vertexes
	geometry->setVertexArray( vertexes );
	
	// assign the indexes
	geometry->addPrimitiveSet( indexes );
	
	// assign the texture coordinates
	geometry->setTexCoordArray( 0, texCoords );
	
	return SceneBuilder::buildGeode( nodeName, geometry, textureName );
}

/**
 * Gets the geometry data from a node (returns NULL if it has no geometry).
 * It does NOT look at children nodes.
 */
const vector< osg::ref_ptr<osg::Drawable> >* SceneBuilder::getNodeGeometry( osg::Node* node ) {
	const osg::Geode* geode = dynamic_cast< osg::Geode* > (node);
	if(!geode) {		// return NULL if the dynamic_cast failed
		return NULL;
	}
	
	return &geode->getDrawableList();
}

/**
 * Encapsulate a node inside a PositionAttitudeTransform node (i.e. this allows the node to be more easily transformed)
 */
osg::ref_ptr< osg::PositionAttitudeTransform > SceneBuilder::transformable( osg::Node* node ) {
	if(node == NULL)
		return NULL;
	
	// make the transform node
	osg::ref_ptr< osg::PositionAttitudeTransform > transformNode = new osg::PositionAttitudeTransform();
	
	// assign the node
	transformNode->addChild( node );
	
	// return the node
	return transformNode;
}
