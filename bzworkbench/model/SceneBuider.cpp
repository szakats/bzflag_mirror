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
 * (i.e. as if it had a different color) and store it in the flyweight if loadSelectedToo is true
 */
osg::ref_ptr< osg::Node > SceneBuilder::buildNode( const char* nodeFile, bool loadSelectedToo ) {
	// string-ify the nodeFile and add the corresponsing tail
	string nodeName = SceneBuilder::nameNode( nodeFile );
	
	// see if this node was already loaded
	if(nodeData.count( nodeName ) > 0) {
		return nodeData[ nodeName ];
	}
	
	// otherwise, load it in
	osg::ref_ptr< osg::Node > node = osgDB::readNodeFile( nodeFile );
	
	if(node == NULL)
		return NULL;
		
	// each node stores nodeName (i.e. the string it's mapped to) as its name
	node->setName( nodeName );
	nodeData[ nodeName ] = node;
	
	// load in the selected version if applicable
	if( loadSelectedToo ) {
		SceneBuilder::buildSelectedNode( nodeFile );
	}
	
	return nodeData[nodeName];
}

/**
 * Load a node and mark it selected
 */
 
osg::ref_ptr< osg::Node > SceneBuilder::buildSelectedNode( const char* fileName ) {
	string selectedNodeName = SceneBuilder::nameSelected( SceneBuilder::nameNode( fileName ).c_str() );
	
	if( nodeData.count( selectedNodeName ) > 0 )
		return nodeData[ selectedNodeName ];
	
	osg::ref_ptr< osg::Node > node = osgDB::readNodeFile( fileName );
	if( node == NULL )
		return NULL;
	
	SceneBuilder::markSelected( node.get() );
	
	node->setName( selectedNodeName );
	nodeData[ selectedNodeName ] = node;
	
	return nodeData[ selectedNodeName ];
}

/**
 * Return a node in the flyweight, or NULL if it hasn't been loaded
 */
 
osg::ref_ptr< osg::Node > SceneBuilder::getNode( const char* nodeFile ) {
	string nodeName = nodeFile;
	
	if( nodeData.count( nodeName ) > 0 )
		return nodeData[ nodeName ];
		
	return NULL;
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
 * Encapsulate a node inside a Renderable (i.e. this allows the node to be more easily transformed)
 */
osg::ref_ptr< Renderable > SceneBuilder::renderable( osg::Node* node, bz2object* obj ) {
	if(node == NULL)
		return NULL;
	
	// make the transform node
	osg::ref_ptr< Renderable > transformNode = new Renderable( node->getName().c_str(), obj );
	
	// assign the node
	transformNode->addChild( node );
	
	// assign the renderable the same name as the node
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

/**
 * Mark an OSG node as selected (i.e. color it green)
 */

void SceneBuilder::markSelected( osg::Node* theNode ) {
	
	// try dynamic_cast-ing the node to a group
    osg::Group* theGroup = dynamic_cast< osg::Group* > (theNode);
    
    // the Geodes
    vector<osg::Geode*> theGeodes = vector<osg::Geode*>();
    
    // if the group dynamic_cast succeeded, then try dynamic_cast-ing its children into the geode array
    if(theGroup != NULL) {
    	// get the children of the group
    	vector< osg::ref_ptr< osg::Node > >* children = SceneBuilder::extractChildren( theGroup );
    	// see if any are geodes (if so, then add them to theGeodes)
    	if( children->size() > 0 ) {
    		for(vector< osg::ref_ptr< osg::Node > >::iterator i = children->begin(); i != children->end(); i++) {
    			// try to dynamic_cast the node to a geode
    			osg::Geode* geode = dynamic_cast< osg::Geode* > (i->get());
    			if(geode)
    				theGeodes.push_back( geode );	
    		}
    	}
    	// free the children memory
    	delete children;
    }
    else {
    	// if that didn't work, then try making the node a geode
    	osg::Geode* geode = dynamic_cast< osg::Geode* > (theNode);
    	if(geode)
    		theGeodes.push_back( geode );
    }
    
    // break if there are no geodes to select
    if( theGeodes.size() == 0) {
    	return;
    }
    
	// assign all geodes a different color
	for(vector< osg::Geode* >::iterator i = theGeodes.begin(); i != theGeodes.end(); i++) {
		osg::Vec4 color(0.0, 1.0, 0.0, 1.0);
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		
		colors->push_back( color );
		
		vector< osg::ref_ptr< osg::Drawable > > drawables = (*i)->getDrawableList();
		if(drawables.size() > 0) {
			osg::Geometry* geo;
			for( vector< osg::ref_ptr< osg::Drawable > >::iterator i = drawables.begin(); i != drawables.end(); i++ ) {
				geo = (*i)->asGeometry();
				if( geo ) {
					geo->setColorArray( colors.get() );
					geo->setColorBinding( osg::Geometry::BIND_OVERALL );
				}
			}	
		}
		
		osg::StateSet* states = (*i)->getOrCreateStateSet();
		
		osg::TexEnv* tec = new osg::TexEnv();
		tec->setMode( osg::TexEnv::BLEND );
		states->setTextureAttribute(0, tec, osg::StateAttribute::ON );
		
		(*i)->setStateSet( states );
	}
}
