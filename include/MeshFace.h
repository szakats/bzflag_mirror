#ifndef MESHFACE_H_
#define MESHFACE_H_

#include "DataEntry.h"
#include "model/BZWParser.h"
#include <vector>
#include <string>

/**
 * This is a class for reading, parsing, writing, and storing information about
 * a "mesh" object's faces.  It assists "mesh" similar to how "ColorCommand" assists
 * "dynamicColor" and "Transform" assists "bz2object"
 */
 
class MeshFace : public DataEntry {
	
public:

	// default constructor
	MeshFace() : 
		DataEntry("face", "<vertices><normals><texcoords><phydrv><matref><drivethrough><shootthrough><passable>") {
		vertices = vector<int>();
		normals = vector<int>();
		texcoords = vector<int>();
		physicsDriver = string("");
		materials = vector<string>();
		driveThrough = shootThrough = passable = false;
	}
	
	// constructor with data
	MeshFace(string& data) : DataEntry("face", "<vertices><normals><texcoords><phydrv><matref><drivethrough><shootthrough><passable>", data.c_str()) {
		vertices = vector<int>();
		normals = vector<int>();
		texcoords = vector<int>();
		physicsDriver = string("");
		materials = vector<string>();
		driveThrough = shootThrough = passable = false;
		
		this->update(data);
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		const char* header = this->getHeader().c_str();
		
		// get the chunk
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str(), "endface");
		
		// break if there are none
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
			
		// break if we have more than one
		if(!hasOnlyOne(lines, header))
			return 0;
			
		// get the data
		const char* faceData = lines[0].c_str();
		
		// get the vertices
		vector<string> vertexVals = BZWParser::getValuesByKey("vertices", header, faceData);
		if(!hasOnlyOne(vertexVals, "vertices"))
			return 0;
		if(vertexVals.size() > 0 && BZWParser::getLineElements( vertexVals[0].c_str() ).size() < 3) {
			printf("mesh::face::update(): Error! Not enough vertex elements in \"%s\"\n", vertexVals[0].c_str());
			return 0;	
		}
			
		// get the texcoords
		vector<string> texCoordVals = BZWParser::getValuesByKey("texcoords", header, faceData);
		if(texCoordVals.size() > 1) {
			printf("mesh::face::update(): Error! Defined \"texcoord\" %d times!\n", texCoordVals.size());
			return 0;	
		}
		
		// get the normals
		vector<string> normalVals = BZWParser::getValuesByKey("normals", header, faceData);
		if(normalVals.size() > 1) {
			printf("mesh::face::update(): Error! Defined \"normal\" %d times!\n", normalVals.size());
			return 0;	
		}
		
		// get the physics driver
		vector<string> physicsDriverVals = BZWParser::getValuesByKey("phydrv", header, faceData);
		if(physicsDriverVals.size() > 1) {
			printf("mesh::face::update(): Error! Defined \"phydrv\" %d times!\n", physicsDriverVals.size());
		}
		
		// get the materials
		vector<string> matrefVals = BZWParser::getValuesByKey("matref", header, faceData);
		
		// get passable
		vector<string> passableVals = BZWParser::getValuesByKey("passable", header, faceData);
		
		// get drivethrough
		vector<string> driveThroughVals = BZWParser::getValuesByKey("drivethrough", header, faceData);
		
		// get shootthrough
		vector<string> shootThroughVals = BZWParser::getValuesByKey("shootthrough", header, faceData);
		
		// do base-class update
		if(!DataEntry::update(data))
			return 0;
			
		// vertices, texcoords, and normals should have the same number of parameters...
		vector<string> vertexParams = BZWParser::getLineElements( vertexVals[0].c_str() );
		vector<string> texCoordParams, normalParams;
		
		if(texCoordVals.size() > 0)
			texCoordParams = BZWParser::getLineElements( texCoordVals[0].c_str() );
		
		if(normalVals.size() > 0)
			normalParams = BZWParser::getLineElements( normalVals[0].c_str() );
		
		// check the lengths of the parameters of "vertices", "texcoords", and "normals"...should be the same
		// (of not, then break)
		if( (vertexParams.size() > 0 && texCoordParams.size() > 0 && normalParams.size() > 0) &&
		   (vertexParams.size() != texCoordParams.size() ||
			vertexParams.size() != normalParams.size() ||
			normalParams.size() != texCoordParams.size())) {
				printf("mesh::face::update(): Error! unequal numbers of referenced vertices, normals, and texture coordinates in face! {\n");
				printf("  vertices %s # %d\n", vertexVals[0].c_str(), vertexParams.size());
				if(texCoordVals.size() > 0)
					printf("  texcoords %s # %d\n", texCoordVals[0].c_str(), texCoordParams.size());
				if(normalVals.size() > 0)
					printf("  normals %s # %d\n", normalVals[0].c_str() , normalParams.size());
				printf("}\n");
				
				return 0;
		}
		
		// load in the data
		vertices = parseValues( vertexParams );
		texcoords = parseValues( texCoordParams );
		normals = parseValues( normalParams );
		materials = matrefVals;
		physicsDriver = ( physicsDriverVals.size() == 0 ? "" : physicsDriverVals[0] );
		driveThrough = ( driveThroughVals.size() == 0 ? false : true );
		shootThrough = ( shootThroughVals.size() == 0 ? false : true );
		passable = ( passableVals.size() == 0 ? (false || (driveThrough && shootThrough)) : true );
		
		return 1;
	}
	
	// toString
	string toString(void) {
		// string-ify the material list
		string matstring = string("");
		if(materials.size() > 0) {
			for(vector<string>::iterator i = materials.begin(); i != materials.end(); i++) {
				matstring += "    matref " + (*i) + "\n";	
			}
		}
		
		return string("face\n") +
					  (vertices.size() > 0 ? "    vertices " + stringify(vertices) + "\n" : "") +
					  (texcoords.size() > 0 ? "    texcoords " + stringify(texcoords) + "\n" : "") +
					  (normals.size() > 0 ? "    normals " + stringify(normals) + "\n" : "") +
					  matstring +
					  (physicsDriver.length() != 0 ? "    phydrv " + physicsDriver + "\n" : "") +
					  (passable == true ? "    passable\n" : "") +
					  (shootThrough == true ? "    shootthrough\n" : "") +
					  (driveThrough == true ? "    drivethrough\n" : "") +
					  "  endface\n";
					  
	}
	
	// render
	int render(void) {
		return 0;
	}

private:

	/**
	 * Helper method: convert an array of strings into an array of ints
	 */
	vector<int> parseValues(vector<string>& values) {
		vector<int> ret = vector<int>();
		
		if(values.size() > 0) {
			for(vector<string>::iterator i = values.begin(); i != values.end(); i++) {
				ret.push_back( atoi( i->c_str() ) );	
			}
		}
		
		return ret;
	}

	/**
	 * Helper method: convert an array of ints into a string
	 */
	 
	string stringify(vector<int>& values) {
		string ret = string("");
		
		if(values.size() > 0) {
			for(vector<int>::iterator i = values.begin(); i != values.end(); i++) {
				ret += string(itoa(*i)) + " ";	
			}	
		}
		
		return ret;
	}
	vector<int> vertices;
	vector<int> normals;
	vector<int> texcoords;
	string physicsDriver;
	vector<string> materials;
	
	bool driveThrough, shootThrough, passable;
};

#endif /*MESHFACE_H_*/
