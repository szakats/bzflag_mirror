#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../DataEntry.h"
#include "../render/RGBA.h"
#include "../BZWParser.h"

#include <string>
#include <vector>

using namespace std;

class material : public DataEntry {

public:
	material() : 
		DataEntry("material", "<name><texture><addtexture><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosorting><noradar><groupalpha><occluder><alphathresh>") {
		name = string("");
		dynamicColor = string("");
		textureMatrix = string("");
		color = string("");
		textures = vector<string>();
		noTextures = noTexColor = noTexAlpha = true;
		noRadar = spheremap = noShadow = noCulling = noSorting = groupAlpha = occluder = false;
		alphaThreshold = 1.0f;
		ambient = RGBA(0, 0, 0, 0);
		diffuse = RGBA(0, 0, 0, 0);
		emissive = RGBA(0, 0, 0, 0);
		specular = RGBA(0, 0, 0, 0);
		shiny = 0.0f;
	}
	
	material(string& data) :
		DataEntry("material", "<name><texture><addtexture><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosorting><noradar><groupalpha><occluder><alphathresh>") {
		name = string("");
		dynamicColor = string("");
		textureMatrix = string("");
		color = string("");
		textures = vector<string>();
		noTextures = noTexColor = noTexAlpha = true;
		noRadar = spheremap = noShadow = noCulling = noSorting = groupAlpha = occluder = false;
		alphaThreshold = 1.0f;
		ambient = RGBA(0, 0, 0, 0);
		diffuse = RGBA(0, 0, 0, 0);
		emissive = RGBA(0, 0, 0, 0);
		specular = RGBA(0, 0, 0, 0);
		shiny = 0.0f;
		
		this->update(data);
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	void update(string& data) {
		const char* header = this->getHeader().c_str();
		
		// get the section
		vector<string> chunks = BZWParser::getSectionsByHeader(header, data.c_str());
		
		if(chunks[0] == BZW_NOT_FOUND)
			return;
		
		const char* materialData = chunks[0].c_str();
		
		// get the name
		vector<string> names = BZWParser::getValuesByKey("name", header, materialData);
		if(!hasOnlyOne(names, "name"))
			return;
		
		// get the dynamic color
		vector<string> dyncols = BZWParser::getValuesByKey("dyncol", header, materialData);
		if(!hasOnlyOne(dyncols, "dyncol"))
			return;
			
		// get the texture matrix
		vector<string> texmats = BZWParser::getValuesByKey("texmat", header, materialData);
		if(!hasOnlyOne(texmats, "texmat"))
			return;
			
		// get the diffuse colors
		vector<string> colors = BZWParser::getValuesByKey("color", header, materialData);
		vector<string> diffs = BZWParser::getValuesByKey("diffuse", header, materialData);
		if((hasOnlyOne(colors, "color") && hasOnlyOne(diffs, "diffuse")) || 
		   (!hasOnlyOne(colors, "color") && !hasOnlyOne(diffs, "diffuse"))) {
		   	printf("material::update():  Error! Could not parse \"color\" or \"diffuse\" properly!\n");
		   return;
		}
		   
		vector<string> diffuses = (colors.size() == 0 ? diffs : colors);
		
		// get the ambient colors
		vector<string> ambients = BZWParser::getValuesByKey("ambient", header, materialData);
		if(!hasOnlyOne(ambients, "ambient"))
			return;
			
		// get the emissive colors
		vector<string> emissives = BZWParser::getValuesByKey("emission", header, materialData);
		if(!hasOnlyOne(emissives, "emissive"))
			return;
			
		// get the specular colors
		vector<string> speculars = BZWParser::getValuesByKey("specular", header, materialData);
		if(!hasOnlyOne(speculars, "specular"))
			return;	
		
		// get the textures
		vector<string> texs = BZWParser::getValuesByKey("addtexture", header, materialData);
		
		// get notextures
		vector<string> notextures = BZWParser::getLinesByKey("notexture", header, materialData);
		
		// get notexcolor
		vector<string> notexcolors = BZWParser::getLinesByKey("notexcolor", header, materialData);
		
		// get spheremap
		vector<string> spheremaps = BZWParser::getLinesByKey("spheremap", header, materialData);
		
		// get noshadow
		vector<string> noshadows = BZWParser::getLinesByKey("noshadows", header, materialData);
		
		// get noculling
		vector<string> nocullings = BZWParser::getLinesByKey("noculling", header, materialData);
		
		// get nosorting
		vector<string> nosortings = BZWParser::getLinesByKey("nosorting", header, materialData);
		
		// get noradar
		vector<string> noradars = BZWParser::getLinesByKey("noradar", header, materialData);
		
		// get notexalpha
		vector<string> notexalphas = BZWParser::getLinesByKey("notexalpha", header, materialData);
		
		// get groupalpha
		vector<string> groupalphas = BZWParser::getLinesByKey("groupalphs", header, materialData);
		
		// get occluder
		vector<string> occluders = BZWParser::getLinesByKey("occluder", header, materialData);
		
		// get resetmat
		// vector<string> resetmat = BZWParser::getLinesByKey("resetmat", header, materialData);
		
		// get shininess
		vector<string> shininesses = BZWParser::getValuesByKey("shininess", header, materialData);
		if(!hasOnlyOne(shininesses, "shininess"))
			return;
			
		// get alpha threshold
		vector<string> alphathresholds = BZWParser::getValuesByKey("alphathresh", header, materialData);
		if(!hasOnlyOne(alphathresholds, "alphathresh"))
			return;
			
		// load the retrieved data into the class
		this->name = names[0];
		this->dynamicColor = dyncols[0];
		this->textureMatrix = texmats[0];
		this->emissive = RGBA( emissives[0].c_str() );
		this->specular = RGBA( speculars[0].c_str() );
		this->ambient = RGBA( ambients[0].c_str() );
		this->diffuse = RGBA( diffuses[0].c_str() );
		this->textures = texs;
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
		this->shiny = atof( shininesses[0].c_str() );
		this->alphaThreshold = atof( alphathresholds[0].c_str() );
	}
	
	// tostring
	string toString(void) {
		string texString = string("");
		for(vector<string>::iterator i = textures.begin(); i != textures.end(); i++) {
			texString += "  addtexture " + (*i) + "\n";	
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
					  (noSorting == true ? "  nosorting\n" : "") +
					  (noLighting == true ? "  nolighting\n" : "") +
					  (noRadar == true ? "  noradar\n" : "") +
					  (groupAlpha == true ? "  groupalpha\n" : "") +
					  (occluder == true ? "  occluder\n" : "") +
					  "  ambient " + ambient.toString() +
					  (color.length() == 0 ? "  diffuse " + diffuse.toString() : "") +
					  "  specular " + specular.toString() +
					  "  emission " + emissive.toString() +
					  "  shininess " + string(ftoa(shiny)) + "\n" +
					  "  alphathresh " + string(ftoa(alphaThreshold)) + "\n" +
					  texString +
					  "end\n";
	}
	
	// render
	int render(void) {
		return 0;
	}
	
private:
	string name, dynamicColor, textureMatrix, color;
	vector<string> textures;
	bool noTextures, noTexColor, noTexAlpha, spheremap, noShadow, noCulling, noSorting, noRadar, noLighting, groupAlpha, occluder;
	RGBA ambient, diffuse, specular, emissive;
	float shiny, alphaThreshold;
	
};

#endif /*MATERIAL_H_*/
