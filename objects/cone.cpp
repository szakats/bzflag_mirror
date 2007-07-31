#include "../include/objects/cone.h"

// default constructor
cone::cone() : 
	bz2object("cone", "<position><rotation><size><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	
	// define some basic values
	this->divisions = 16;
	this->setName("default_cone");
	this->physicsDriver = string("");
	flatShading = false;
	smoothbounce = true;
	
	// default size is 10x10x10
	this->setSize( osg::Vec3( 10, 10, 10 ) );
	
	// build the geometry
	this->buildGeometry();
	
}

// constructor with data
cone::cone(string& data) :
	bz2object("cone", "<position><rotation><size><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	
	// define some basic values
	this->divisions = 16;
	this->setName("default_cone");
	this->physicsDriver = string("");
	flatShading = false;
	smoothbounce = true;
	
	// default size is 10x10x10
	this->setSize( osg::Vec3( 10, 10, 10 ) );
	
	if( this->update(data) == 0 ) {
		// if the update failed, just add a default cone
		this->buildGeometry();
	}
}

// getter
string cone::get(void) { return this->toString(); }

// setter
int cone::update(string& data) {
	const char* header = this->getHeader().c_str();
	// get the chunk we need
	
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	// check and see if the proper data segment was found
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "cone"))
		return 0;
		
	const char* coneData = lines[0].c_str();
		
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, coneData);
	if(names.size() > 1) {
		printf("cone::update(): error! defined \"name\" %d times\n", names.size() );
		return 0;
	}
	if( names.size() == 0 )
		names.push_back( string("default_cone") );	// default name is "default_cone"
		
	// get the divisions
	vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, coneData);
	if( vDivisions.size() > 1 ) {
		printf("cone::update(): error! defined \"divisions\" %d times\n", vDivisions.size() );
		return 0;
	}
	if( vDivisions.size() == 0 )
		vDivisions.push_back( string("16") );		// default # of divisions is 16
		
	// get flatshading
	vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, coneData);
	
	// get smoothbounce
	vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, coneData);
	
	// do base class update
	if(!bz2object::update(data))
		return 0;
	
	// set the data
	this->setName( names[0] );
	
	// see if the divisions changed (if so, then update the geometry)
	int oldDivisions = this->divisions;
	this->divisions = atoi( vDivisions[0].c_str() );
	if( this->divisions != oldDivisions ) {
		this->removeChild( coneNode.get() );
		this->removeChild( baseNode.get() );
		
		this->buildGeometry();
	}
	
	this->flatShading = (flatShadings.size() == 0 ? false : true);
	this->updateShadeModel();		// update the shade model
	
	this->smoothbounce = (smoothBounces.size() == 0 ? false : true);
	
	return 1;
}

// toString
string cone::toString(void) {
	return string("cone\n") +
				  this->BZWLines() +
				  "  divisions " + string(itoa(divisions)) + "\n" +
				  (flatShading == true ? "  flatshading\n" : "") +
				  (smoothbounce == true ? "  smoothbounce\n" : "") + 
				  "end\n";
}

// build the cone geometry
void cone::buildGeometry() {
	
	// geometry data for the conical component
	osg::Vec3Array* points = new osg::Vec3Array();
	osg::DrawElementsUInt* indices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_FAN, 0 );
	osg::Vec2Array* texCoords = new osg::Vec2Array();
	
	// geometry for the base of the cone
	osg::DrawElementsUInt* baseIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_STRIP, 0 );
	osg::Vec2Array* baseTexCoords = new osg::Vec2Array();
	
   	// add the tip of the cone to the conical geometry
   	points->push_back( osg::Vec3( 0.0, 0.0, 1.0 ) );	// the tip of the cone
   	indices->push_back( 0 );	// the index of the tip of the cone
   	texCoords->push_back( osg::Vec2( 0.5, 0.5 ) );	// texture coordinate of the tip of the cone
   	baseTexCoords->push_back( osg::Vec2( 0.5, 0.5) );	// just a space-holder here
   	
   	// build the base
   	float radius_x = 1.0;
   	float radius_y = 1.0;
   	float angle;
   	for( int i = 0; i < divisions; i++ ) {
   		angle = 2.0 * osg::PI * ((float)i / (float)divisions);
   		
   		// add the vertex
   		points->push_back( osg::Vec3( cos(angle) * radius_x, sin(angle) * radius_y, 0 ) );
   		
   		// add the index of that vertex to the conical geometry
   		indices->push_back( i+1 );
   		
   		// add the texture coordinate of that vertex to the concial geometry
   		texCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );
   		
   		// add the texture coordinate of that vertex to the base geometry
   		baseTexCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );
   	}
   	
   	// add the final face to connect the last index to the first
   	indices->push_back( 1 );
   	
   	// build the base indices
   	baseIndices->push_back(1);
   	baseIndices->push_back(2);
   	for( int i = 3; i <= (divisions >> 1) + 1; i++ ) {
   		baseIndices->push_back( divisions - i + 3 );
   		baseIndices->push_back( i );
   	}
   	// if we have an odd # of divisions, add the final face
   	if( divisions % 2 == 1 ) {
   		baseIndices->push_back( (divisions>>1) + 2 );
   	}
   	
   	// build the geodes
   	this->coneNode = SceneBuilder::buildGeode( SceneBuilder::nameNode("cone").c_str(), points, indices, texCoords, "share/boxwall.png" );
   	this->baseNode = SceneBuilder::buildGeode( SceneBuilder::nameNode("conebase").c_str(), points, baseIndices, baseTexCoords, "share/roof.png" );
   	
   	// add the geodes to the Renderable
   	this->theCone = new osg::Group();
   	theCone->addChild( coneNode.get() );
   	theCone->addChild( baseNode.get() );
    	
   	thisNode = theCone.get();
   	this->addChild( thisNode.get() );
}

// set the shade model based on the value of flatShading
void cone::updateShadeModel() {
	// get state set
	osg::StateSet* states = this->getOrCreateStateSet();
	
	// get the shade model
	osg::ShadeModel* shadeModel = dynamic_cast< osg::ShadeModel* >( states->getAttribute( osg::StateAttribute::SHADEMODEL ) );
	if( shadeModel == NULL ) {
		shadeModel = new osg::ShadeModel();		// if one doesn't exist, then make one
	}
		
	if( flatShading ) {
		shadeModel->setMode( osg::ShadeModel::FLAT );
	}
	else {
		shadeModel->setMode( osg::ShadeModel::SMOOTH );
	}
	
	// set the shade model
	states->setAttribute( shadeModel );
}
