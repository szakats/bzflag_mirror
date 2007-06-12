#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../DataEntry.h"
#include "../render/RGBA.h"
#include "../model/BZWParser.h"

#include <string>
#include <vector>

using namespace std;

class material : public DataEntry {

public:
	// default constructor
	material();
	
	// constructor with data
	material(string& data);
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// tostring
	string toString(void);
	
	// render
	int render(void);
	
private:
	string name, dynamicColor, textureMatrix, color;
	vector<string> textures, materials;
	bool noTextures, noTexColor, noTexAlpha, spheremap, noShadow, noCulling, noSorting, noRadar, noLighting, groupAlpha, occluder, resetmat;
	RGBA ambient, diffuse, specular, emissive;
	float shiny, alphaThreshold;
	
};

#endif /*MATERIAL_H_*/
