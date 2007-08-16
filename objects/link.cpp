#include "../include/objects/link.h"

// constructor
Tlink::Tlink() : bz2object("link", "<name><from><to>") {
	name = string("default_link");
	from = NULL;
	to = NULL;
}

// constructor with data
Tlink::Tlink(string& data) : bz2object("link", "<name><from><to>") {
	name = string("default_link");
	from = NULL;
	to = NULL;
	
	this->update(data);	
}

// getter
string Tlink::get(void) { return this->toString(); }

// setter
int Tlink::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the data chunks
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
	
	if(!hasOnlyOne(lines, "link"))
		return 0;
		
	// get the data to the block
	const char* linkData = lines[0].c_str();
	
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, linkData);
	if(names.size() > 1) {
		printf("link::update(): Error! Defined \"name\" %d times!\n", names.size());
		return 0;
	}
		
	// get the from
	vector<string> froms = BZWParser::getValuesByKey("from", header, linkData);
	if(!hasOnlyOne(froms, "from"))
		return 0;
		
	// get the to
	vector<string> tos = BZWParser::getValuesByKey("to", header, linkData);
	if(!hasOnlyOne(tos, "to"))
		return 0;
		
	// superclass update
	if(!DataEntry::update(data))
		return 0;
	
	// load in the data
	this->name = (names.size() != 0 ? names[0] : "");
	
	teleporter* prevFrom = from;
	teleporter* prevTo = to;
	
	this->from = dynamic_cast< teleporter* > (Model::command( MODEL_GET, "teleporter", froms[0] ));
	this->to = dynamic_cast< teleporter* > (Model::command( MODEL_GET, "teleporter", tos[0] ));
	
	if( from && to && (prevFrom != from || prevTo != to) )
		this->buildGeometry();
	
	return 1;
}

// toString
string Tlink::toString(void) {
	string fromName = (from == NULL ? "# from:(unknown)\n" : "  from " + from->getName() + "\n");
	string toName = (to == NULL ? "# to:(unknown)\n" : "  to " + to->getName() + "\n" );
	
	return string("link\n") +
				  (name.length() != 0 ? "  name " + name : "# name") + "\n" +
				  fromName + 
				  toName + 
				  "end\n";
}

// build the link geometry
void Tlink::buildGeometry() {
	// don't draw links if there aren't defined "from" or "to" values
	if( from == NULL || to == NULL )
		return;
		
	// basically, make a yellow line between the teleporters
	osg::ref_ptr< osg::Cylinder > line;
	
	// get the positions of the teleporters
	osg::Vec3 fromPos = from->getPos();
	osg::Vec3 toPos = to->getPos();
	
	// raise the positions to the top of the teleporters
	fromPos.set( fromPos.x(), fromPos.y(), fromPos.z() + from->getSize().z() );
	toPos.set( toPos.x(), toPos.y(), toPos.z() + to->getSize().z() );
	
	// compute the length
	osg::Vec3 diff = toPos - fromPos;
	double length = diff.length();
	
	// make the line
	line = new osg::Cylinder( fromPos, 0.1, length );
	
	// make a ShapeDrawable to contain the line
	osg::ref_ptr< osg::ShapeDrawable > drawable = new osg::ShapeDrawable( line.get() );
	
	// contain the drawable inside a goede
	osg::ref_ptr< osg::Geode > geode = new osg::Geode();
	geode->addDrawable( drawable.get() );
	
	// assign the node
	this->setThisNode( geode.get() );
}
