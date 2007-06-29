#include "../include/model/SceneBuilder.h"
#include "../include/windows/View.h"

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
 * This method builds and returns a node loaded from nodeFile, and will load the same node as if it were selected
 * (i.e. as if it had a different color)
 */
osg::ref_ptr< osg::Node > SceneBuilder::buildNode( const char* nodeFile, bool loadSelectedToo ) {
	// string-ify the nodeFile and add the corresponsing tail
	string nodeName = string( nodeFile ) + SCENEBUILDER_TAIL_NODE;
	string selectedNodeName = nodeName + SCENEBUILDER_TAIL_SELECTED;

	// see if this node was already loaded
	if( loadSelectedToo == false && nodeData[ nodeName ] != NULL) {
		return nodeData[ nodeName ];
	}
	if( loadSelectedToo == true && nodeData[ selectedNodeName ] != NULL) {
		return nodeData[ selectedNodeName ];	
	}
	
	// otherwise, load it in
	nodeData[ nodeName ] = osgDB::readNodeFile( nodeFile );
	
	// each node stores nodeName (i.e. the string it's mapped to) as its name
	nodeData[ nodeName ]->setName( nodeName );
	
	// load in the selected version if applicable
	if( loadSelectedToo ) {
		nodeData[ selectedNodeName ] = osgDB::readNodeFile( nodeFile );
		osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
		pat->addChild( nodeData[ selectedNodeName ].get() );
		View::markSelected( pat.get() );
		nodeData[ selectedNodeName ]->setName( selectedNodeName );
	}
	
	return nodeData[nodeName];
}

/**
 * Geometry builder
 * This method builds and returns a geometry node (a geode) from arrays of vertexes, indexes, texture coordinates, and
 * a texture filename
 */
osg::ref_ptr< osg::Geode > SceneBuilder::buildGeode( const char* _nodeName, osg::Geometry* geometry, const char* textureFile, bool loadSelectedToo ) {
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
osg::ref_ptr< osg::Geode > SceneBuilder::buildGeode( const char* nodeName, osg::Vec3Array* vertexes, osg::DrawElementsUInt* indexes, osg::Vec2Array* texCoords, const char* textureName, bool loadSelectedToo ) {
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
	
	return SceneBuilder::buildGeode( nodeName, geometry, textureName, loadSelectedToo );
}

/**
 * Gets the geometry data from a node (returns NULL if it has no geometry).
 * It does NOT look at children nodes.
 */
const vector< osg::ref_ptr<osg::Drawable> >* SceneBuilder::getNodeGeometry( osg::PositionAttitudeTransform* node ) {
	const osg::Group* group = dynamic_cast< osg::Group* > (node);
	if(!group) {		// return NULL if the dynamic_cast failed
		return NULL;
	}
	
	const osg::Geode* geode = dynamic_cast< osg::Geode* > (node);
	if(!geode) {
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
	
	// assign the transformation the same name as the node
	transformNode->setName( node->getName());
	
	// return the node
	return transformNode;
}

/**
 * Recursively iterate through a group and return an array of all its children
 */
vector< osg::ref_ptr< osg::Node > >* SceneBuilder::extractChildren( osg::Group* group ) {
	// declare an array of children
	vector< osg::ref_ptr< osg::Node > >* children = new vector< osg::ref_ptr< osg::Node > >();
	
	// see if we have any children to iterate through
	int numChildren = group->getNumChildren();
	if(numChildren <= 0)
		return children;		// return an empty array
	
	// go through the children
	for(int i = 0; i < numChildren; i++) {
		// try to dynamic_cast this to a group
		osg::Group* childGroup = dynamic_cast< osg::Group* > (group->getChild( i ) );
		// if this is a group, recursively get its children
		if(childGroup) {
			vector< osg::ref_ptr< osg::Node > >* childGroupChildren = SceneBuilder::extractChildren( childGroup );	
			// copy over the children (if any exist)
			if( childGroupChildren->size() > 0 ) {
				for(vector< osg::ref_ptr< osg::Node > >::iterator i = childGroupChildren->begin(); i != childGroupChildren->end(); i++) {
					children->push_back( *i );	
				}	
			}
			delete childGroupChildren;
		}
		// this wasn't a group; add it to the return
		else {
			children->push_back( group->getChild( i ) );	
		}
	}
	
	// return the children
	return children;
}
