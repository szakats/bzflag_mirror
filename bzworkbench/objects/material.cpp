#include "../include/objects/material.h"

// default constructor
material::material() : 
	DataEntry("material", "<name><texture><addtexture><matref><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosort><noradar><nolighting><groupalpha><occluder><alphathresh>"),
	osg::Material() {
	name = string("");
	dynCol = NULL;
	textureMatrix = NULL;
	color = string("");
	textures = vector< osg::ref_ptr< osg::Texture2D > >();
	noTextures = noTexColor = noTexAlpha = true;
	noRadar = spheremap = noShadow = noCulling = noLighting = noSorting = groupAlpha = occluder = resetmat = false;
	alphaThreshold = 1.0f;
}

// constructor with data
material::material(string& data) :
	DataEntry("material", "<name><texture><addtexture><matref><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosort><noradar><nolighting><groupalpha><occluder><alphathresh>", data.c_str()),
	osg::Material() {
	name = string("");
	dynCol = NULL;
	textureMatrix = NULL;
	color = string("");
	textures = vector< osg::ref_ptr< osg::Texture2D > >();
	noTextures = noTexColor = noTexAlpha = true;
	noRadar = spheremap = noShadow = noCulling = noLighting = noSorting = groupAlpha = occluder = resetmat = false;
	alphaThreshold = 1.0f;
	
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
	if(dyncols.size() > 1) {
		printf("material::update(): Error! Defined \"dyncol\" %d times!\n", dyncols.size());
		return 0;
	}
		
	// get the texture matrix
	vector<string> texmats = BZWParser::getValuesByKey("texmat", header, materialData);
	if(texmats.size() > 1) {
		printf("material::update(): Error! Defined \"texmat\" %d times!\n", texmats.size());
		return 0;
	}
		
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
	if(ambients.size() > 1) {
		printf("material::update(): Error! Defined \"ambient\" %d times!\n", ambients.size());
		return 0;
	}
		
	// get the emissive colors
	vector<string> emissives = BZWParser::getValuesByKey("emission", header, materialData);
	if(emissives.size() > 1) {
		printf("material::update(): Error! Defined \"emissive\" %d times!\n", emissives.size());
		return 0;
	}
	
	// get the specular colors
	vector<string> speculars = BZWParser::getValuesByKey("specular", header, materialData);
	if(speculars.size() > 1) {
		printf("material::update(): Error! Defined \"specular\" %d times!\n", speculars.size());
		return 0;
	}
	
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
	if(shininesses.size() > 1) {
		printf("material::update(): Error! Defined \"shininess\" %d times!\n", shininesses.size());
		return 0;
	}
		
	// get alpha threshold
	vector<string> alphathresholds = BZWParser::getValuesByKey("alphathresh", header, materialData);
	if(alphathresholds.size() > 1) {
		printf("material::update(): Error! Defined \"alphathresh\" %d times!\n", alphathresholds.size());
		return 0;
	}
		
	// get other material refrences
	vector<string> matrefs = BZWParser::getValuesByKey("matref", header, materialData);
		
	// load the retrieved data into the class
	if(!DataEntry::update(data))
		return 0;
	this->name = names[0];
	this->dynCol = (dyncols.size() != 0 ? (dynamicColor*)Model::command( MODEL_GET, "dynamicColor", dyncols[0] ) : NULL);
	this->textureMatrix = (texmats.size() != 0 ? (texturematrix*)Model::command( MODEL_GET, "texturematrix", texmats[0]) : NULL);
	(emissives.size() != 0 ? this->setEmissive(RGBA( emissives[0].c_str() )) : this->setEmissive(RGBA(0, 0, 0, 0)));
	(speculars.size() != 0 ? this->setSpecular(RGBA( speculars[0].c_str() )) : this->setSpecular(RGBA(0, 0, 0, 0)));
	(ambients.size() != 0 ? this->setAmbient(RGBA( ambients[0].c_str() )) : this->setAmbient(RGBA(1, 1, 1, 1)));
	(diffuses.size() != 0 ? this->setDiffuse(RGBA( diffuses[0].c_str() )) : this->setDiffuse(RGBA(1, 1, 1, 1)));
	
	this->textures.clear();
	this->materials.clear();
	// get the materials from the model
	for( vector<string>::iterator i = matrefs.begin(); i != matrefs.end(); i++) {
		material* mat = (material*)Model::command( MODEL_GET, "material", *i );
		if( mat != NULL )
			this->materials.push_back( mat );
	}
	
	// this->textures = texs;
	
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
	(shininesses.size() > 0 ? this->setShininess(atof( shininesses[0].c_str() )) : this->setShininess(0.0) );
	this->alphaThreshold = (alphathresholds.size() > 0 ? atof( alphathresholds[0].c_str() ) : 1.0f);
	
	return 1;
}

// tostring
string material::toString(void) {
	string texString = string("");
	for(vector< osg::ref_ptr<osg::Texture2D> >::iterator i = textures.begin(); i != textures.end(); i++) {
		string filename = (*i)->getImage()->getFileName();
		string::size_type suffixIndex = filename.find(".png");
		if( suffixIndex != string::npos )
			filename = filename.substr(0, suffixIndex );
			
		texString += "  addtexture " + filename + "\n";	
	}
	
	string matString = string("");
	for(vector< osg::ref_ptr< material > >::iterator i = materials.begin(); i != materials.end(); i++) {
		matString += "  matref " + (*i)->getName() + "\n";	
	}
	
	// color strings
	string ambientString = "  ambient " + RGBA( this->getAmbient().x(), this->getAmbient().y(), this->getAmbient().z(), this->getAmbient().w() ).toString();
	string diffuseString = "  diffuse " + RGBA( this->getDiffuse().x(), this->getDiffuse().y(), this->getDiffuse().z(), this->getDiffuse().w() ).toString();
	string specularString = "  ambient " + RGBA( this->getSpecular().x(), this->getSpecular().y(), this->getSpecular().z(), this->getSpecular().w() ).toString();
	string emissiveString = "  ambient " + RGBA( this->getEmissive().x(), this->getEmissive().y(), this->getEmissive().z(), this->getEmissive().w() ).toString();
	
	
	return string("material\n") +
				  (name.length() == 0 ? string("# name") : "  name " + name) + "\n" +
				  (dynCol != NULL ? string("  dyncol ") + dynCol->getName() : string("  dyncol -1")) + "\n" +
				  (textureMatrix != NULL ? string("  texmat ") + textureMatrix->getName() : string("  texmat -1")) + "\n" +
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
				  "  ambient " + ambientString +
				  (color.length() == 0 ? "  diffuse " + diffuseString : "") +
				  "  specular " + specularString +
				  "  emission " + emissiveString +
				  "  shininess " + string(ftoa(this->getShininess())) + "\n" +
				  "  alphathresh " + string(ftoa(alphaThreshold)) + "\n" +
				  texString +
				  matString +
				  this->getUnusedText() + 
				  "end\n";
}
