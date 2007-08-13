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
	
	// turn on blending
	osg::StateSet* stateSet = node->getOrCreateStateSet();
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	   
	
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
osg::Geode* SceneBuilder::buildGeode( const char* _nodeName, osg::Geometry* geometry, const char* textureFile, unsigned int textureMode ) {
	// make the node name
	string nodeName = string(_nodeName) + SCENEBUILDER_TAIL_NODE;
	
	// make the geometry name
	string geometryName = string(_nodeName) + SCENEBUILDER_TAIL_GEOMETRY;
	
	// make a geode
	osg::Geode* geode = new osg::Geode();
	
	// load up the texture
    SceneBuilder::assignTexture( textureFile, geode, textureMode );
	
	geode->addDrawable( geometry );
	
	return geode;
}

/**
 * Build a Texture2D from a file; return NULL if not found
 */
 
osg::Texture2D* SceneBuilder::buildTexture2D( const char* filename ) {
	if( filename != NULL ) {
		osg::Texture2D* texture = NULL;
		
		osg::Image* image = osgDB::readImageFile( filename );
	
		if( image != NULL ) {	// only build the texture if the image exists!
			
			texture = new osg::Texture2D();
		
			// don't allow OSG to optimize the texture (otherwise it may disappear)
			texture->setDataVariance( osg::Object::DYNAMIC );
		
			// set the texture's image
			texture->setImage( image );
			
			// turn on GL_REPEAT texture wrapping
			texture->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
			texture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
			texture->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT ); 
		}
		
		return texture;
	}
	return NULL;
}

// assign a texture to a Node
void SceneBuilder::assignTexture( const char* _textureName, osg::Node* node, unsigned int mode ) {
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
		texStateSet->setTextureAttributeAndModes( 0, texture, mode );
		
		// finally, attach the texture to the geode
		node->setStateSet( texStateSet );
	}
}

// assign a material to a node
void SceneBuilder::assignMaterial( osg::Vec4 ambient, osg::Vec4 diffuse, osg::Vec4 specular, osg::Vec4 emissive, float shininess, float alpha, osg::Node* node, osg::StateAttribute::Values val ) {
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
	stateSet->setAttribute( mat, val);
	
	// assign the new state set
	node->setStateSet( stateSet );
}

// assign a BZWB material to a bz2object
void SceneBuilder::assignBZMaterial( material* bzmat, bz2object* obj ) {
	obj->setStateSet( bzmat );
	
}

// get the material from a node
osg::Material* extractMaterial( Renderable* r ) {
	osg::StateSet* stateSet = r->getOrCreateStateSet();
	
	return (osg::Material*)(stateSet->getAttribute( osg::StateAttribute::MATERIAL ) );
}

// build a geometry and call the other buildGeode method
osg::Geode* SceneBuilder::buildGeode( const char* nodeName, osg::Vec3Array* vertexes, osg::DrawElementsUInt* indexes, osg::Vec2Array* texCoords, const char* textureName, unsigned int textureMode ) {
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
	
	return SceneBuilder::buildGeode( nodeName, geometry, textureName, textureMode );
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
								   theNode,
								   osg::StateAttribute::OVERRIDE );

}

// mark a node as unselected
void SceneBuilder::markUnselected( osg::Node* theNode ) {
	SceneBuilder::assignMaterial(  osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 0.0, 0.0, 0.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   0.0,
								   1.0,
								   theNode,
								   osg::StateAttribute::OVERRIDE );
}

// mark the object selected without destroying its state set
void SceneBuilder::markSelectedAndPreserveStateSet( bz2object* theNode ) {
	
	osg::StateSet* currStateSet = theNode->getOrCreateStateSet();
	osg::Material* currMaterial = (osg::Material*)currStateSet->getAttribute( osg::StateAttribute::MATERIAL );
	if( currStateSet != NULL ) {
		theNode->savedStateSet = new osg::StateSet( *currStateSet );
		printf(" saving states\n");
		
		if( currMaterial != NULL ) {
			printf("    ambient:  %f %f %f %f\n", currMaterial->getAmbient( osg::Material::FRONT ).x(), currMaterial->getAmbient( osg::Material::FRONT ).y(), currMaterial->getAmbient( osg::Material::FRONT ).z(), currMaterial->getAmbient( osg::Material::FRONT ).w() );
			printf("    diffuse:  %f %f %f %f\n", currMaterial->getDiffuse( osg::Material::FRONT ).x(), currMaterial->getDiffuse( osg::Material::FRONT ).y(), currMaterial->getDiffuse( osg::Material::FRONT ).z(), currMaterial->getDiffuse( osg::Material::FRONT ).w() );
			printf("    specular: %f %f %f %f\n", currMaterial->getSpecular( osg::Material::FRONT ).x(), currMaterial->getSpecular( osg::Material::FRONT ).y(), currMaterial->getSpecular( osg::Material::FRONT ).z(), currMaterial->getSpecular( osg::Material::FRONT ).w() );
			printf("    emissive: %f %f %f %f\n", currMaterial->getEmission( osg::Material::FRONT ).x(), currMaterial->getEmission( osg::Material::FRONT ).y(), currMaterial->getEmission( osg::Material::FRONT ).z(), currMaterial->getEmission( osg::Material::FRONT ).w() );
		}
	}
	
	SceneBuilder::assignMaterial(  osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 0.0, 0.0, 0.0, 1.0 ),
								   osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   0.0,
								   1.0,
								   theNode,
								   osg::StateAttribute::OVERRIDE );

}

// mark a node as unselected by restoring it's state set if applicable
void SceneBuilder::markUnselectedAndRestoreStateSet( bz2object* theNode ) {
	
	osg::StateSet* stateSet = theNode->savedStateSet.get();
	osg::Material* mat = (osg::Material*)stateSet->getAttribute( osg::StateAttribute::MATERIAL );
	if( stateSet != NULL ) {
		printf(" restoring states\n");
		
		if( mat != NULL ) {
			printf("    ambient:  %f %f %f %f\n", mat->getAmbient( osg::Material::FRONT ).x(), mat->getAmbient( osg::Material::FRONT ).y(), mat->getAmbient( osg::Material::FRONT ).z(), mat->getAmbient( osg::Material::FRONT ).w() );
			printf("    diffuse:  %f %f %f %f\n", mat->getDiffuse( osg::Material::FRONT ).x(), mat->getDiffuse( osg::Material::FRONT ).y(), mat->getDiffuse( osg::Material::FRONT ).z(), mat->getDiffuse( osg::Material::FRONT ).w() );
			printf("    specular: %f %f %f %f\n", mat->getSpecular( osg::Material::FRONT ).x(), mat->getSpecular( osg::Material::FRONT ).y(), mat->getSpecular( osg::Material::FRONT ).z(), mat->getSpecular( osg::Material::FRONT ).w() );
			printf("    emissive: %f %f %f %f\n", mat->getEmission( osg::Material::FRONT ).x(), mat->getEmission( osg::Material::FRONT ).y(), mat->getEmission( osg::Material::FRONT ).z(), mat->getEmission( osg::Material::FRONT ).w() );
		}
		
		theNode->setStateSet( stateSet );
	}
	
	else
		SceneBuilder::markUnselected( theNode );
}

