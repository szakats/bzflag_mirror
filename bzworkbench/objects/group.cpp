#include "../include/objects/group.h"

// constructor
group::group() : 
	bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>") {
	this->team = 0;
	this->tintColor = RGBA(1, 1, 1, 1);
	this->driveThrough = false;
	this->shootThrough = false;
	this->setName("");
	this->setPos( osg::Vec3( 0.0, 0.0, 0.0 ) );
	pos = osg::Vec3( 0.0, 0.0, 0.0 );
}

// constructor with data
group::group(string& data) : 
	bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>", data.c_str()) {
	this->team = 0;
	this->tintColor = RGBA(1, 1, 1, 1);
	this->driveThrough = false;
	this->shootThrough = false;
	this->setName("");
	this->setPos( osg::Vec3( 0.0, 0.0, 0.0 ) );
	this->update(data);	
	pos = this->getPos();
}

// getter
string group::get(void) {
	return this->toString(); 
}

// setter
int group::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the section from the data
	const vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "group"))
		return 0;
	
	const char* groupData = lines[0].c_str();
	
	// get name (from the first line)
	vector<string> headers = BZWParser::getValuesByKey("group", header, groupData);
	
	// get tint
	vector<string> tints = BZWParser::getValuesByKey("tint", header, groupData);
	if(tints.size() > 1) {
		printf("group::update(): Error! Defined \"tint\" %d times\n", tints.size());
		return 0;	
	}
	
	// get team
	vector<string> teams = BZWParser::getValuesByKey("team", header, groupData);
	if(teams.size() > 1) {
		printf("group::update(): Error! Defined \"team\" %d times\n", tints.size());
		return 0;	
	}
	
	// get drivethrough
	vector<string> driveThroughs = BZWParser::getValuesByKey("drivethrough", header, groupData);
		
	// get shootthrough
	vector<string> shootThroughs = BZWParser::getValuesByKey("shootthrough", header, groupData);
		
	// do base class update
	if(!bz2object::update(data))
		return 0;
	
	// assign data
	// see if the name changed (if so, recompute the object list)
	string oldName = this->getName();
	this->setName( headers[0] );
	if( oldName != this->getName() )
		this->updateObjects();
		
	this->tintColor = (tints.size() > 0 ? RGBA( tints[0].c_str() ) : RGBA(-1, -1, -1, -1));
	this->team = (teams.size() > 0 ? (int)(atof( teams[0].c_str() )) : -1);
	this->driveThrough = (driveThroughs.size() == 0 ? false : true);
	this->shootThrough = (shootThroughs.size() == 0 ? false : true);
	
	return 1;
	
}

// event handler
int group::update( UpdateMessage& message ) {
	
	// superclass update (i.e. handle transformation changes)
	int result = bz2object::update( message );
	
	switch( message.type ) {
			
		case UpdateMessage::SET_POSITION_FACTOR: {	// handle a translation
			osg::Vec3 translationFactor = *(message.getAsPositionFactor());
			pos += translationFactor;
			
			// get objects
			vector< osg::ref_ptr< bz2object > > objects = def->getObjects();
			
			// propogate the translation into child objects
			if( objects.size() > 0 ) {
				for( vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++ ) {
					UpdateMessage msg( UpdateMessage::SET_POSITION_FACTOR, &translationFactor );
					(*i)->update( msg );
				}
			}
			
			break;
		}
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

// get position (for the 3D cursor, mainly)
// groups have no concept of position, so we'll return the average position of the child objects
osg::Vec3 group::getPos() {
	if( def == NULL )
		return osg::Vec3( 0, 0, 0 );		// no definition association!
		
	// get the objects
	vector< osg::ref_ptr< bz2object > > objects = def->getObjects();
	
	if( objects.size() <= 0 )
		return osg::Vec3( 0, 0, 0);		// no objects!
	
	// compute the average position
	float x = 0.0, y = 0.0, z = 0.0;
	for( vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++ ) {
		x += (*i)->getPos().x();
		y += (*i)->getPos().y();
		z += (*i)->getPos().z();
	}
	
	x /= objects.size();
	y /= objects.size();
	z /= objects.size();
	
	return osg::Vec3( x, y, z);
}


// toString
string group::toString(void) {
	string tintString = string(""), teamString = string("");
	if(tintColor.r() > 0 && tintColor.g() > 0 && tintColor.b() > 0 && tintColor.a() > 0)
		tintString = "  tint " + tintColor.toString();
	if(team > 0)
		teamString = "  team " + string(itoa(team)) + "\n";
	
	return string("group ") + this->getName() + "\n" +
				  tintString + 
				  teamString +
				  (driveThrough == true ? "  drivethrough\n" : "") +
				  (shootThrough == true ? "  shootThrough\n" : "") +
				  "shift " + ftoa(pos.x()) + " " + ftoa(pos.y()) + " " + ftoa(pos.z()) + "\n" +  // add the pos value as a "shift" transformation
				  this->BZWLines() +
				  "end\n";
}

// build the ring geometry around the objects
void group::buildGeometry() {
	
}

// re-compute the list of objects contained in the group
void group::updateObjects() {
	// remove all current objects
	this->removeChildren(0, this->getNumChildren());
	
	// get the "define" reference
	define* def = (define*)Model::command( MODEL_GET, "define", this->getName() );
	
	// if it was valid, add the objects
	if( def != NULL ) {
		this->def = def;
		// get the objects
		vector< osg::ref_ptr< bz2object > > objects = def->getObjects();
		
		// put each object inside a PositionAttitudeTransform
		// add them as children of this object
		if( objects.size() > 0 ) {	
			for( vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++ ) {
				this->addChild( i->get() );
				printf(" added %s\n", (*i)->getName().c_str() );
			}
		}
	}
}
