#include "../include/objects/cone.h"

// default constructor
cone::cone() : 
	bz2object("cone", "<position><rotation><size><angle><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	
	// define some basic values
	this->divisions = 16;
	this->setName("default_cone");
	this->physicsDriver = string("");
	flatShading = false;
	smoothbounce = true;
	
	sweepAngle = 360.0f;
	
	// default size is 10x10x10
	this->setSize( osg::Vec3( 10, 10, 10 ) );
	
	// build the geometry
	this->buildGeometry();
	
}

// constructor with data
cone::cone(string& data) :
	bz2object("cone", "<position><rotation><size><angle><flatshading><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	
	// define some basic values
	this->divisions = 16;
	this->setName("default_cone");
	this->physicsDriver = string("");
	flatShading = false;
	smoothbounce = true;
	
	sweepAngle = 360.0f;
	
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
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str(), "end");
	
	// check and see if the proper data segment was found
	if(lines[0] == BZW_NOT_FOUND) {
		printf("cone: data not found\n");
		return 0;
	}
		
	if(!hasOnlyOne(lines, "cone")) {
		printf("cone: improper data\n");
		return 0;
	}
		
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
		
	// get the sweep angle
	vector<string> sweepAngles = BZWParser::getValuesByKey("angle", header, coneData);
	if( sweepAngles.size() > 1 ) {
		printf("cone::update(): error! defined \"divisions\" %d times\n", sweepAngles.size() );
		return 0;
	}
	if( sweepAngles.size() == 0 )
		sweepAngles.push_back( string("360") );		// default sweep is 360
		
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
	
	float oldSweepAngle = this->sweepAngle;
	this->sweepAngle = atof( sweepAngles[0].c_str() );
	
	// if the number of divisions changed or the sweep angle changed, rebuild the geometry
	if( this->divisions != oldDivisions || this->sweepAngle != oldSweepAngle ) {
		theCone->removeChild( coneNode.get() );
		theCone->removeChild( baseNode.get() );
		
		this->buildGeometry();
	}
	
	this->flatShading = (flatShadings.size() == 0 ? false : true);
	this->updateShadeModel();		// update the shade model
	
	this->smoothbounce = (smoothBounces.size() == 0 ? false : true);
	
	return 1;
}

// event handler
int cone::update( UpdateMessage& message ) {
	
	// superclass event handler
	int result = bz2object::update( message );
	
	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			this->setPos( *(message.getAsPosition()) );
			break;
			
		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			this->setPos( this->getPos() + *(message.getAsPositionFactor()) );
			break;
			
		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			this->setRotation( *(message.getAsRotation()) );
			break;
			
		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			this->setRotation( this->getRotation() + *(message.getAsRotationFactor()) );
			break;
			
		case UpdateMessage::SET_SCALE:		// handle a new scale
			this->setSize( *(message.getAsScale()) );
			break;
			
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			this->setSize( this->getSize() + *(message.getAsScaleFactor()) );
			break;
			
		default:	// unknown event; don't handle
			return result;
	}
	
	return 1;
}


// toString
string cone::toString(void) {
	return string("cone\n") +
				  this->BZWLines() +
				  "  divisions " + string(itoa(divisions)) + "\n" +
				  (flatShading == true ? "  flatshading\n" : "") +
				  (smoothbounce == true ? "  smoothbounce\n" : "") + 
				  "  angle " + string(ftoa(sweepAngle) ) + "\n" + 
				  "end\n";
}

// build the cone geometry
void cone::buildGeometry() {
	// make the group
	this->theCone = new osg::Group();
   	
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
   	
   	// build a full cone if the sweep angle is >= 360.0 degrees
   	if( sweepAngle >= 360.0f ) {
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
   	}
   	else {			// build a section of a cone
   		// add the center of the cone
   		points->push_back( osg::Vec3( 0, 0, 0 ) );
   		
   		// it's texture coordinate
   		baseTexCoords->push_back( osg::Vec2( 0.5, 0.5 ) );
   		
   		// place-holder texture coordinate
   		texCoords->push_back( osg::Vec2( 0.5, 0.5 ) );
	   	
   		float sweepAngleRads = sweepAngle * osg::PI / 180.0f;
   		for( int i = 0; i <= divisions; i++ ) {
	   		angle = sweepAngleRads * ((float)i / (float)divisions);
	   		
	   		// add the vertex
	   		points->push_back( osg::Vec3( cos(angle) * radius_x, sin(angle) * radius_y, 0 ) );
	   		
	   		// add the index of that vertex to the conical geometry
	   		indices->push_back( i+2 );
	   		
	   		// add the texture coordinate of that vertex to the concial geometry
	   		texCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );
	   		
	   		// add the texture coordinate of that vertex to the base geometry
	   		baseTexCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );
	   	}
	   	
	   	
	   	// build the base indices
	   	// switch to a triangle fan
	   	baseIndices->setMode( osg::PrimitiveSet::TRIANGLE_FAN );
	   	
	   	// make the indices
	   	// the first point should index the center of the cone
	   	baseIndices->push_back( 1 );	// the 1st point is the center
	   	for( int i = 2; i <= divisions + 2; i++ ) {
	   		baseIndices->push_back( i );
	   	}
	   	
	   	// make the cross-sections
	   	osg::DrawElementsUInt* crossSectionIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_STRIP, 0 );
	   	
	   	// add the indices to (1) the last point in the base, (2) the center, (3) the tip, and (4) the first point in the base
	   	crossSectionIndices->push_back( divisions + 2 );	// the (divisions+2)-th point is the last point in the base
	   	crossSectionIndices->push_back( 1 );	// the 1st point is the center
	   	crossSectionIndices->push_back( 0 );			// the 0th point is the tip
	   	crossSectionIndices->push_back( 2 );			// the 2st point is the first point in the base
	   	
	   	// make the cross-section texture coordinates
	   	osg::Vec2Array* crossSectionTexCoords = new osg::Vec2Array();
	   	
	   	// make sure we correspond to the indices
	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.0 ) );
	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.5 ) );
	   	crossSectionTexCoords->push_back( osg::Vec2( 1.0, 0.5 ) );
	   	// insert place-holders
	   	for( int i = 0; i <= divisions + 3; i++ )
	   		crossSectionTexCoords->push_back( osg::Vec2( 1.0, 0.0 ) );
	   		
	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.0 ) );
	   	
	   	// make the crossSection geode
	   	this->crossSectionNode = SceneBuilder::buildGeode( SceneBuilder::nameNode("coneCrossSection").c_str(), points, crossSectionIndices, crossSectionTexCoords, "share/wall.png" );
	   	theCone->addChild( crossSectionNode.get() );
   	}
   	
   	// build the geodes
   	this->coneNode = SceneBuilder::buildGeode( SceneBuilder::nameNode("cone").c_str(), points, indices, texCoords, "share/boxwall.png" );
   	this->baseNode = SceneBuilder::buildGeode( SceneBuilder::nameNode("coneBase").c_str(), points, baseIndices, baseTexCoords, "share/roof.png" );
   	
   	// add the geodes to the Renderable
   	theCone->addChild( coneNode.get() );
   	theCone->addChild( baseNode.get() );
    	
   	this->setThisNode( theCone.get() );
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
