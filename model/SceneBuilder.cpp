#include "../include/model/SceneBuilder.h"
#include "../include/windows/View.h"


int SceneBuilder::nameCount;

// constructor
bool SceneBuilder::init() {
	nameCount = 0;
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
osg::Node* SceneBuilder::buildNode( const char* nodeFile ) {
	if(!nodeFile)
		return NULL;
		
	// string-ify the nodeFile and add the corresponsing tail
	string nodeName = SceneBuilder::nameNode( nodeFile );
	
	// load it in
	osg::Node* node = osgDB::readNodeFile( nodeFile );
	
	if(node == NULL)
		return NULL;
		
	// each node stores nodeName (i.e. the string it's mapped to) as its name
	node->setName( nodeName );
	
	// make a TextureVisitor to enable GL_REPEAT in the node's textures
	TextureRepeaterVisitor tv = TextureRepeaterVisitor();
	
	// do a traversal to apply the modification
	node->accept( tv );
	
	// make a TextureScalerVisitor
	//Renderable* r = new Renderable( node );
	//TextureScalerVisitor tsv = TextureScalerVisitor( r, osg::Vec3( 7.0, 3.5, 1.0 ) );
	
	//node->accept( tsv );
	
	return node;
}

/**
 * Load a node and mark it selected
 */
 
osg::Node* SceneBuilder::buildSelectedNode( const char* fileName ) {
	string selectedNodeName = SceneBuilder::nameSelected( SceneBuilder::nameNode( fileName ).c_str() );
	
	osg::Node* node = osgDB::readNodeFile( fileName );
	if( node == NULL )
		return NULL;
		
	SceneBuilder::markSelected( node );
	
	node->setName( selectedNodeName );
	
	return node;
}

/**
 * Geometry builder
 * This method builds and returns a geometry node (a geode) from arrays of vertexes, indexes, texture coordinates, and
 * a texture filename
 */
osg::Geode* SceneBuilder::buildGeode( const char* _nodeName, osg::Geometry* geometry, const char* textureFile ) {
	// make the node name
	string nodeName = string(_nodeName) + SCENEBUILDER_TAIL_NODE;
	
	// make the geometry name
	string geometryName = string(_nodeName) + SCENEBUILDER_TAIL_GEOMETRY;
	
	// make a geode
	osg::Geode* geode = new osg::Geode();
	
	// load up the texture
    SceneBuilder::assignTexture( textureFile, geode );
	
	geode->addDrawable( geometry );
	
	return geode;
}

// assign a texture to a Node
void SceneBuilder::assignTexture( const char* _textureName, osg::Node* node ) {
	if(_textureName != NULL) {
		string textureName = string(_textureName) + SCENEBUILDER_TAIL_TEXTURE2D;
		
		// the texture itself
		osg::Texture2D* texture = NULL;
		
		texture = new osg::Texture2D();
		
		osg::Image* image = osgDB::readImageFile( _textureName );
	
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
		 
 		// make a new state set for the texture (so we can manipulate the texture attributes)
		osg::StateSet* texStateSet = node->getOrCreateStateSet();
		
		// assign the texture to the state set and activate it
		texStateSet->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
		
		// finally, attach the texture to the geode
		node->setStateSet( texStateSet );
	}
}

// assign a material to a node
void SceneBuilder::assignMaterial( osg::Vec4 ambient, osg::Vec4 diffuse, osg::Vec4 specular, osg::Vec4 emissive, float shininess, float alpha, osg::Node* node ) {
	osg::Material* mat = new osg::Material();
	
	// set ambient lighting
	mat->setAmbient( osg::Material::FRONT, ambient );
	
	// set diffuse color
	mat->setDiffuse( osg::Material::FRONT, diffuse );
	
	// set specular
	mat->setSpecular( osg::Material::FRONT, specular );
	
	// set emissive
	mat->setEmission( osg::Material::FRONT, emissive );
	
	// set shininess
	mat->setShininess( osg::Material::FRONT, shininess );
	
	// set transparency
	mat->setAlpha( osg::Material::FRONT, alpha );
	
	// get the state set from the node
	osg::StateSet* stateSet = node->getOrCreateStateSet();
	
	// assign the material
	stateSet->setAttribute( mat, osg::StateAttribute::OVERRIDE );
	
	// assign the new state set
	node->setStateSet( stateSet );
}

// get the material from a node
osg::Material* extractMaterial( Renderable* r ) {
	osg::StateSet* stateSet = r->getOrCreateStateSet();
	
	return (osg::Material*)(stateSet->getAttribute( osg::StateAttribute::MATERIAL ) );
}

// build a geometry and call the other buildGeode method
osg::Geode* SceneBuilder::buildGeode( const char* nodeName, osg::Vec3Array* vertexes, osg::DrawElementsUInt* indexes, osg::Vec2Array* texCoords, const char* textureName ) {
	// don't bother if the data is NULL
	if(vertexes == NULL || indexes == NULL)
		return NULL;
	
	// make a new geometry
	osg::Geometry* geometry = new osg::Geometry();
	
	// assign the vertexes
	geometry->setVertexArray( vertexes );
	
	// assign the indexes
	geometry->addPrimitiveSet( indexes );
	
	if( texCoords != NULL) 
		// assign the texture coordinates
		geometry->setTexCoordArray( 0, texCoords );
	
	return SceneBuilder::buildGeode( nodeName, geometry, textureName );
}

/**
 * Gets the geometry data from a node (returns NULL if it has no geometry).
 * It does NOT look at children nodes.
 */
const vector< osg::ref_ptr< osg::Drawable > >* getNodeGeometry( osg::PositionAttitudeTransform* node ) {
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
 * Mark an OSG node as selected (i.e. color it green)
 */

void SceneBuilder::markSelected( osg::Node* theNode ) {
	SceneBuilder::assignMaterial(  osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 0.0, 0.0, 0.0, 1.0 ),
								   osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   0.0,
								   1.0,
								   theNode );
	/*
	// try dynamic_cast-ing the node to a group
    osg::Group* theGroup = dynamic_cast< osg::Group* > (theNode);
    
    // the Geodes
    vector<osg::Geode*> theGeodes = vector<osg::Geode*>();
    
    // if the group dynamic_cast succeeded, then try dynamic_cast-ing its children into the geode array
    if(theGroup != NULL) {
    	// get the children of the group
    	vector< osg::Node* >* children = SceneBuilder::extractChildren( theGroup );
    	// see if any are geodes (if so, then add them to theGeodes)
    	if( children->size() > 0 ) {
    		for(vector< osg::Node* >::iterator i = children->begin(); i != children->end(); i++) {
    			// try to dynamic_cast the node to a geode
    			osg::Geode* geode = dynamic_cast< osg::Geode* > (*i);
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
		tec->setMode( osg::TexEnv::ADD );
		states->setTextureAttribute(0, tec, osg::StateAttribute::ON );
		
		(*i)->setStateSet( states );
	}
	*/
}

// mark a node as unselected
void SceneBuilder::markUnselected( osg::Node* theNode ) {
	SceneBuilder::assignMaterial(  osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 0.0, 0.0, 0.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   0.0,
								   1.0,
								   theNode );
	/*
	// try dynamic_cast-ing the node to a group
    osg::Group* theGroup = dynamic_cast< osg::Group* > (theNode);
    
    // the Geodes
    vector<osg::Geode*> theGeodes = vector<osg::Geode*>();
    
    // if the group dynamic_cast succeeded, then try dynamic_cast-ing its children into the geode array
    if(theGroup != NULL) {
    	// get the children of the group
    	vector< osg::Node* >* children = SceneBuilder::extractChildren( theGroup );
    	// see if any are geodes (if so, then add them to theGeodes)
    	if( children->size() > 0 ) {
    		for(vector< osg::Node* >::iterator i = children->begin(); i != children->end(); i++) {
    			// try to dynamic_cast the node to a geode
    			osg::Geode* geode = dynamic_cast< osg::Geode* > (*i);
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
    
	// disable the green coloration
	for(vector< osg::Geode* >::iterator i = theGeodes.begin(); i != theGeodes.end(); i++) {
		osg::Vec4 color(1.0, 1.0, 1.0, 1.0);
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		
		colors->push_back( color );
		
		vector< osg::ref_ptr< osg::Drawable > > drawables = (*i)->getDrawableList();
		if(drawables.size() > 0) {
			osg::Geometry* geo;
			for( vector< osg::ref_ptr< osg::Drawable > >::iterator i = drawables.begin(); i != drawables.end(); i++ ) {
				geo = (*i)->asGeometry();
				if( geo ) {
					geo->setColorArray( colors.get() );
					geo->setColorBinding( osg::Geometry::BIND_OFF );
				}
			}	
		}
		
		osg::StateSet* states = (*i)->getOrCreateStateSet();
		
		osg::TexEnv* tec = new osg::TexEnv();
		tec->setMode( osg::TexEnv::MODULATE );
		states->setTextureAttribute(0, tec, osg::StateAttribute::ON );
		
		(*i)->setStateSet( states );
	}
	*/
}

