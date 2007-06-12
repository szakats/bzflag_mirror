#include "../include/objects/material.h"

// default constructor
material::material() : 
	DataEntry("material", "<name><texture><addtexture><matref><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosort><noradar><nolighting><groupalpha><occluder><alphathresh>") {
	name = string("");
	dynamicColor = string("");
	textureMatrix = string("");
	color = string("");
	textures = vector<string>();
	noTextures = noTexColor = noTexAlpha = true;
	noRadar = spheremap = noShadow = noCulling = noSorting = groupAlpha = occluder = resetmat = false;
	alphaThreshold = 1.0f;
	ambient = RGBA(0, 0, 0, 0);
	diffuse = RGBA(0, 0, 0, 0);
	emissive = RGBA(0, 0, 0, 0);
	specular = RGBA(0, 0, 0, 0);
	shiny = 0.0f;
}

// constructor with data
material::material(string& data) :
	DataEntry("material", "<name><texture><addtexture><matref><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosort><noradar><nolighting><groupalpha><occluder><alphathresh>", data.c_str()) {
	name = string("");
	dynamicColor = string("");
	textureMatrix = string("");
	color = string("");
	textures = vector<string>();
	noTextures = noTexColor = noTexAlpha = true;
	noRadar = spheremap = noShadow = noCulling = noSorting = groupAlpha = occluder = resetmat = false;
	alphaThreshold = 1.0f;
	ambient = RGBA(0, 0, 0, 0);
	diffuse = RGBA(0, 0, 0, 0);
	emissive = RGBA(0, 0, 0, 0);
	specular = RGBA(0, 0, 0, 0);
	shiny = 0.0f;
	
	this->update(data);
}

// getter
string material::get(void) { return this->toString(); }

// setter
int material::update(string& data) {
	
	const char* header = this->getHeader().c_str();
	
	// get the section
	vector<string> chunks = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(chunks[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(chunks, "material"))
		return 0;
	
	const char* materialData = chunks[0].c_str();
	
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, materialData);
	if(!hasOnlyOne(names, "name"))
		return 0;
	
	// get the dynamic color
	vector<string> dyncols = BZWParser::getValuesByKey("dyncol", header, materialData);
	if(!hasOnlyOne(dyncols, "dyncol"))
		return 0;
		
	// get the texture matrix
	vector<string> texmats = BZWParser::getValuesByKey("texmat", header, materialData);
	if(!hasOnlyOne(texmats, "texmat"))
		return 0;
		
	// get the diffuse colors
	vector<string> colors = BZWParser::getValuesByKey("color", header, materialData);
	vector<string> diffs = BZWParser::getValuesByKey("diffuse", header, materialData);
	if((colors.size() > 1 && diffs.size() == 0) || 
	   (colors.size() == 0 && diffs.size() > 1)) {
	   	printf("material::update():  Error! Could not parse \"color\" or \"diffuse\" properly!\n");
	   return 0;
	}
	   
	vector<string> diffuses = (colors.size() == 0 ? diffs : colors);
	
	// get the ambient colors
	vector<string> ambients = BZWParser::getValuesByKey("ambient", header, materialData);
	if(!hasOnlyOne(ambients, "ambient"))
		return 0;
		
	// get the emissive colors
	vector<string> emissives = BZWParser::getValuesByKey("emission", header, materialData);
	if(!hasOnlyOne(emissives, "emissive"))
		return 0;
		
	// get the specular colors
	vector<string> speculars = BZWParser::getValuesByKey("specular", header, materialData);
	if(!hasOnlyOne(speculars, "specular"))
		return 0;	
	
	// get the textures
	vector<string> texs = BZWParser::getValuesByKey("addtexture", header, materialData);
	
	// get notextures
	vector<string> notextures = BZWParser::getValuesByKey("notexture", header, materialData);
	
	// get notexcolor
	vector<string> notexcolors = BZWParser::getValuesByKey("notexcolor", header, materialData);
	
	// get spheremap
	vector<string> spheremaps = BZWParser::getValuesByKey("spheremap", header, materialData);
	
	// get noshadow
	vector<string> noshadows = BZWParser::getValuesByKey("noshadow", header, materialData);
	
	// get noculling
	vector<string> nocullings = BZWParser::getValuesByKey("noculling", header, materialData);
	
	// get nolighting
	vector<string> nolightings = BZWParser::getValuesByKey("nolighting", header, materialData);
	
	// get nosorting
	vector<string> nosortings = BZWParser::getValuesByKey("nosort", header, materialData);
	
	// get noradar
	vector<string> noradars = BZWParser::getValuesByKey("noradar", header, materialData);
	
	// get notexalpha
	vector<string> notexalphas = BZWParser::getValuesByKey("notexalpha", header, materialData);
	
	// get groupalpha
	vector<string> groupalphas = BZWParser::getValuesByKey("groupalpha", header, materialData);
	
	// get occluder
	vector<string> occluders = BZWParser::getValuesByKey("occluder", header, materialData);
	
	// get resetmat
	vector<string> resetmats = BZWParser::getValuesByKey("resetmat", header, materialData);
	if(resetmats.size() > 1)
		printf("material::update(): Warning!  Multiple \"resetmat\" declared!\n");
	
	// get shininess
	vector<string> shininesses = BZWParser::getValuesByKey("shininess", header, materialData);
	if(!hasOnlyOne(shininesses, "shininess"))
		return 0;
		
	// get alpha threshold
	vector<string> alphathresholds = BZWParser::getValuesByKey("alphathresh", header, materialData);
	if(!hasOnlyOne(alphathresholds, "alphathresh"))
		return 0;
		
	// get other material refrences
	vector<string> matrefs = BZWParser::getValuesByKey("matref", header, materialData);
		
	// load the retrieved data into the class
	if(!DataEntry::update(data))
		return 0;
	this->name = names[0];
	this->dynamicColor = dyncols[0];
	this->textureMatrix = texmats[0];
	this->emissive = RGBA( emissives[0].c_str() );
	this->specular = RGBA( speculars[0].c_str() );
	this->ambient = RGBA( ambients[0].c_str() );
	this->diffuse = RGBA( diffuses[0].c_str() );
	this->textures = texs;
	this->materials = matrefs;
	this->noTextures = (notextures.size() == 0 ? false : true);
	this->noTexColor = (notexcolors.size() == 0 ? false : true);
	this->spheremap = (spheremaps.size() == 0 ? false : true);
	this->noShadow = (noshadows.size() == 0 ? false : true);
	this->noCulling = (nocullings.size() == 0 ? false : true);
	this->noSorting = (nosortings.size() == 0 ? false : true);
	this->noRadar = (noradars.size() == 0 ? false : true);
	this->noTexAlpha = (notexalphas.size() == 0 ? false : true);
	this->groupAlpha = (groupalphas.size() == 0 ? false : true);
	this->occluder = (occluders.size() == 0 ? false : true);
	this->resetmat = (resetmats.size() == 0 ? false : true);
	this->noLighting = (nolightings.size() == 0 ? false : true);
	this->shiny = atof( shininesses[0].c_str() );
	this->alphaThreshold = atof( alphathresholds[0].c_str() );
	
	return 1;
}

// tostring
string material::toString(void) {
	string texString = string("");
	for(vector<string>::iterator i = textures.begin(); i != textures.end(); i++) {
		texString += "  addtexture " + (*i) + "\n";	
	}
	
	string matString = string("");
	for(vector<string>::iterator i = materials.begin(); i != materials.end(); i++) {
		matString += "  matref " + (*i) + "\n";	
	}
	
	return string("material\n") +
				  "  name " + name + "\n" +
				  (dynamicColor.length() != 0 ? string("  dyncol ") + dynamicColor : string("  dyncol -1")) + "\n" +
				  (textureMatrix.length() != 0 ? string("  texmat ") + textureMatrix : string("  texmat -1")) + "\n" +
				  (color.length() != 0 ? string("  color ") + color + "\n" : "") +
				  (noTextures == true ? "  notextures\n" : "") +
				  (noTexColor == true ? "  notexcolor\n" : "") +
				  (noTexAlpha == true ? "  notexalpha\n" : "") +
				  (spheremap == true ? "  spheremap\n" : "") +
				  (noShadow == true ? "  noshadow\n" : "") +
				  (noCulling == true ? "  noculling\n" : "") +
				  (noSorting == true ? "  nosort\n" : "") +
				  (noLighting == true ? "  nolighting\n" : "") +
				  (noRadar == true ? "  noradar\n" : "") +
				  (groupAlpha == true ? "  groupalpha\n" : "") +
				  (occluder == true ? "  occluder\n" : "") +
				  (resetmat == true ? "  resetmat\n" : "") +
				  "  ambient " + ambient.toString() +
				  (color.length() == 0 ? "  diffuse " + diffuse.toString() : "") +
				  "  specular " + specular.toString() +
				  "  emission " + emissive.toString() +
				  "  shininess " + string(ftoa(shiny)) + "\n" +
				  "  alphathresh " + string(ftoa(alphaThreshold)) + "\n" +
				  texString +
				  matString +
				  this->getUnusedText() + 
				  "end\n";
}

// render
int material::render(void) {
	return 0;
}
