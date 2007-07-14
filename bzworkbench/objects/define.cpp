#include "../include/objects/define.h"

// constructor
define::define() : DataEntry("define", "<arc><base><box><cone><group><mesh><meshbox><meshpyr><pyramid><sphere><teleporter><tetra>") {
	this->objects = vector<bz2object*>();
	this->name = "";	
}

// constructor with data
define::define(string& data) : DataEntry("define", "<arc><base><box><cone><group><mesh><meshbox><meshpyr><pyramid><sphere><teleporter><tetra>", data.c_str()) {
	this->objects = vector<bz2object*>();
	this->name = "";	
	this->update(data);
}

// destructor
define::~define() {
	// free previous objects
	if(objects.size() > 0) {
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			if(*i != NULL) {
				delete *i;
				*i = NULL;	
			}	
		}	
	}
}

// getter
string define::get(void) { return this->toString(); }

// setter
int define::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the text
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
	
	if(lines.size() > 1) {
		printf("define::update(): Error! Defined \"define\" %d times!\n", lines.size());
		return 0;	
	}
	
	// get the name
	vector<string> names = BZWParser::getValuesByKey("define", header, lines[0].c_str());
	if(!hasOnlyOne(names, header))
		return 0;
			
	// get the data
	string::size_type sectionStart = lines[0].find("\n") + 1;
	string::size_type sectionEnd = lines[0].find( "enddef", sectionStart );
	
	lines[0] = lines[0].substr( sectionStart, sectionEnd );
	
	const char* defineData = lines[0].c_str();
	
	// get the chunks
	vector<string> chunks = BZWParser::getSections( defineData );
	
	// get all the supported objects
	
	// arc
	vector<string> arcs = BZWParser::findSections("arc", chunks);
	vector<arc> arcData;
	if(arcs[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = arcs.begin(); i != arcs.end(); i++) {
			arc tmp = arc();
			if(!tmp.update(*i))
				return 0;
			arcData.push_back( tmp );	
		}	
	}
	
	// base
	vector<string> bases = BZWParser::findSections("base", chunks);
	vector<base> baseData;
	if(bases[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = bases.begin(); i != bases.end(); i++) {
			base tmp = base();
			if(!tmp.update(*i))
				return 0;
			baseData.push_back( tmp );	
		}	
	}
	
	// box
	vector<string> boxes = BZWParser::findSections("box", chunks);
	vector<box> boxData;
	if(boxes[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = boxes.begin(); i != boxes.end(); i++) {
			box tmp = box();
			if(!tmp.update(*i))
				return 0;
			boxData.push_back( tmp );	
		}	
	}
	
	// cone
	vector<string> cones = BZWParser::findSections("cone", chunks);
	vector<cone> coneData;
	if(cones[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = cones.begin(); i != cones.end(); i++) {
			cone tmp = cone();
			if(!tmp.update(*i))
				return 0;
			coneData.push_back( tmp );	
		}	
	}
	
	// group
	vector<string> groups = BZWParser::findSections("group", chunks);
	vector<group> groupData;
	if(groups[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = groups.begin(); i != groups.end(); i++) {
			group tmp = group();
			if(!tmp.update(*i))
				return 0;
			groupData.push_back( tmp );	
		}	
	}
	
	// mesh
	vector<string> meshes = BZWParser::findSections("mesh", chunks);
	vector<mesh> meshData;
	if(meshes[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = meshes.begin(); i != meshes.end(); i++) {
			mesh tmp = mesh();
			if(!tmp.update(*i))
				return 0;
			meshData.push_back( tmp );	
		}	
	}
	
	// meshbox
	vector<string> meshboxes = BZWParser::findSections("meshbox", chunks);
	vector<meshbox> meshboxData;
	if(meshboxes[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = meshboxes.begin(); i != meshboxes.end(); i++) {
			meshbox tmp = meshbox();
			if(!tmp.update(*i))
				return 0;
			meshboxData.push_back( tmp );	
		}	
	}
	
	// meshpyr
	vector<string> meshpyrs = BZWParser::findSections("meshpyr", chunks);
	vector<meshpyr> meshpyrData;
	if(meshpyrs[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = meshpyrs.begin(); i != meshpyrs.end(); i++) {
			meshpyr tmp = meshpyr();
			if(!tmp.update(*i))
				return 0;
			meshpyrData.push_back( tmp );	
		}	
	}
	
	// pyramid
	vector<string> pyramids = BZWParser::findSections("pyramid", chunks);
	vector<pyramid> pyramidData;
	if(pyramids[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = pyramids.begin(); i != pyramids.end(); i++) {
			pyramid tmp = pyramid();
			if(!tmp.update(*i))
				return 0;
			pyramidData.push_back( tmp );	
		}	
	}
	
	// sphere
	vector<string> spheres = BZWParser::findSections("sphere", chunks);
	vector<sphere> sphereData;
	if(spheres[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = spheres.begin(); i != spheres.end(); i++) {
			sphere tmp = sphere();
			if(!tmp.update(*i))
				return 0;
			sphereData.push_back( tmp );	
		}	
	}
	
	// teleporter
	vector<string> teleporters = BZWParser::findSections("teleporter", chunks);
	vector<teleporter> teleporterData;
	if(teleporters[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = teleporters.begin(); i != teleporters.end(); i++) {
			teleporter tmp = teleporter();
			if(!tmp.update(*i))
				return 0;
			teleporterData.push_back( tmp );	
		}	
	}
	
	// tetra
	vector<string> tetras = BZWParser::findSections("tetra", chunks);
	vector<tetra> tetraData;
	if(tetras[0] != BZW_NOT_FOUND) {
		for(vector<string>::iterator i = tetras.begin(); i != tetras.end(); i++) {
			tetra tmp = tetra();
			if(!tmp.update(*i))
				return 0;
			tetraData.push_back( tmp );	
		}	
	}
	
	
	// base-class update
	if(!DataEntry::update(data)) {
		return 0;
	}
	
	// commit name
	this->name = names[0];
	
	// free previous objects
	if(objects.size() > 0) {
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			if(*i != NULL) {
				delete *i;
				*i = NULL;	
			}	
		}	
	}
	
	objects.clear();
	
	// commit arcs
	if(arcData.size() > 0) {
		for(vector<arc>::iterator i = arcData.begin(); i != arcData.end(); i++) {
			this->objects.push_back( new arc(*i) );	
		}	
	}
	
	// commit bases
	if(baseData.size() > 0) {
		for(vector<base>::iterator i = baseData.begin(); i != baseData.end(); i++) {
			this->objects.push_back( new base(*i) );	
		}	
	}
	
	// commit boxes
	if(boxData.size() > 0) {
		for(vector<box>::iterator i = boxData.begin(); i != boxData.end(); i++) {
			this->objects.push_back( new box(*i) );	
		}	
	}
	
	// commit cones
	if(coneData.size() > 0) {
		for(vector<cone>::iterator i = coneData.begin(); i != coneData.end(); i++) {
			this->objects.push_back( new cone(*i) );	
		}	
	}
	
	// commit groups
	if(groupData.size() > 0) {
		for(vector<group>::iterator i = groupData.begin(); i != groupData.end(); i++) {
			this->objects.push_back( new group(*i) );	
		}	
	}
	
	// commit meshes
	if(meshData.size() > 0) {
		for(vector<mesh>::iterator i = meshData.begin(); i != meshData.end(); i++) {
			this->objects.push_back( new mesh(*i) );	
		}	
	}
	
	// commit meshboxes
	if(meshboxData.size() > 0) {
		for(vector<meshbox>::iterator i = meshboxData.begin(); i != meshboxData.end(); i++) {
			this->objects.push_back( new meshbox(*i) );	
		}	
	}
	
	// commit meshpyrs
	if(meshpyrData.size() > 0) {
		for(vector<meshpyr>::iterator i = meshpyrData.begin(); i != meshpyrData.end(); i++) {
			this->objects.push_back( new meshpyr(*i) );	
		}	
	}
	
	// commit pyramids
	if(pyramidData.size() > 0) {
		for(vector<pyramid>::iterator i = pyramidData.begin(); i != pyramidData.end(); i++) {
			this->objects.push_back( new pyramid(*i) );	
		}	
	}
	
	// commit spheres
	if(sphereData.size() > 0) {
		for(vector<sphere>::iterator i = sphereData.begin(); i != sphereData.end(); i++) {
			this->objects.push_back( new sphere(*i) );	
		}	
	}
	
	// commit teleporters
	if(teleporterData.size() > 0) {
		for(vector<teleporter>::iterator i = teleporterData.begin(); i != teleporterData.end(); i++) {
			this->objects.push_back( new teleporter(*i) );	
		}
	}
	
	// commit tetras
	if(tetraData.size() > 0) {
		for(vector<tetra>::iterator i = tetraData.begin(); i != tetraData.end(); i++) {
			this->objects.push_back( new tetra(*i) );	
		}	
	}
	
	return 1;
}

// toString
string define::toString(void) {
	string objString = "";
	if(objects.size() > 0) {
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			objString += (*i)->toString() + "\n";
		}
	}
	
	return "define " + name + "\n" + objString + "enddef\n";
}

// render
int define::render(void) { return 0; }
