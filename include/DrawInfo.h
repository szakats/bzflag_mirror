#ifndef DRAWINFO_H_
#define DRAWINFO_H_

/**
 * This is an assistant class to "mesh".  It stores the drawinfo section of the mesh
 */

#include "DataEntry.h"
#include "model/BZWParser.h"
#include "render/Point3D.h"
#include "render/Index3D.h"
#include "render/TexCoord2D.h"
#include "render/Vector3D.h"
#include "LOD.h"
#include "LODCommand.h"

class DrawInfo : public DataEntry {

public:

	// default constructor
	DrawInfo() :
		DataEntry("drawinfo", "<dlist><decorative><angvel><extends><sphere><corner><vertex><normal><texcoord><lod>") {
	
		vertices = vector<Point3D>();
		normals = vector<Vector3D>();
		texcoords = vector<TexCoord2D>();
		corners = vector<Index3D>();
		
		minExtends = Point3D(0.0f, 0.0f, 0.0f);
		maxExtends = Point3D(0.0f, 0.0f, 0.0f);
		spherePosition = Point3D(0.0f, 0.0f, 0.0f);
		
		angvel = 0.0f;
		sphereRadius = 0.0f;
		
		dlist = true;
		decorative = false;
	}
	
	// constructor with data
	DrawInfo(string& data) :
		DataEntry("drawinfo", "<dlist><decorative><angvel><extends><sphere><corner><vertex><normal><texcoord><lod>", data.c_str()) {
	
		vertices = vector<Point3D>();
		normals = vector<Vector3D>();
		texcoords = vector<TexCoord2D>();
		corners = vector<Index3D>();
		
		minExtends = Point3D(0.0f, 0.0f, 0.0f);
		maxExtends = Point3D(0.0f, 0.0f, 0.0f);
		spherePosition = Point3D(0.0f, 0.0f, 0.0f);
		
		angvel = 0.0f;
		sphereRadius = 0.0f;
		
		dlist = true;
		decorative = false;
		
		this->update(data);
	}
	
	// getter
	string get(void) { return this->toString(); }
		
	// setter
	int update(string& data) {
		const char* header = this->getHeader().c_str();
		
		// get the section
		vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
		
		// break if there's no section
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
			
		// break if there's too many
		if(!hasOnlyOne(lines, header))
			return 0;
			
		// get the data
		const char* drawInfoData = lines[0].c_str();
		
		// get the vertices
		vector<string> vertexVals = BZWParser::getValuesByKey("vertex", header, drawInfoData);
		
		// get the texcoords
		vector<string> texCoordVals = BZWParser::getValuesByKey("texcoord", header, drawInfoData);
		
		// get the normals
		vector<string> normalVals = BZWParser::getValuesByKey("normal", header, drawInfoData);
		
		// get the dlist
		vector<string> dlistVals = BZWParser::getValuesByKey("dlist", header, drawInfoData);
		
		// get the decorative
		vector<string> decorativeVals = BZWParser::getValuesByKey("decorative", header, drawInfoData);
		
		// get angvel
		vector<string> angvelVals = BZWParser::getValuesByKey("angvel", header, drawInfoData);
		if(angvelVals.size() > 1) {
			printf("mesh::DrawInfo::update(): Error! Defined \"angvel\" %d times!\n", angvelVals.size());
			return 0;	
		}
		if(angvelVals.size() > 0 && !hasNumElements(angvelVals[0], 1))
			return 0;
		
		// get corners
		vector<string> cornerVals = BZWParser::getValuesByKey("corner", header, drawInfoData);
		
		// get spheres
		vector<string> sphereVals = BZWParser::getValuesByKey("sphere", header, drawInfoData);
		if(sphereVals.size() > 1) {
			printf("mesh::DrawInfo::update(): Error! Defined \"sphere\" %d times!\n", sphereVals.size());
			return 0;	
		}
		if(sphereVals.size() > 0 && !hasNumElements(sphereVals[0], 4))
			return 0;
		
		// get extends
		vector<string> extendsVals = BZWParser::getValuesByKey("extends", header, drawInfoData);
		if(extendsVals.size() > 1) {
			printf("mesh::DrawInfo::update(): Error! Defined \"extends\" %d times!\n", sphereVals.size());
			return 0;	
		}
		if(extendsVals.size() > 0 && !hasNumElements(extendsVals[0], 6))
			return 0;
				
		// need same amount of corners as vertexes
		if(cornerVals.size() != vertexVals.size()) {
			printf("mesh::DrawInfo::update(): Error! Unequal numbers of corners and vertexes\n");
			return 0;
		}
		
		// need same amount of corners as normals
		if(cornerVals.size() != normalVals.size()) {
			printf("mesh::DrawInfo::update(): Error! Unequal numbers of corners and normals\n");
			return 0;
		}
			
		// parse the values
		vector<Point3D> vertexData = vector<Point3D>();
		vector<Index3D> cornerData = vector<Index3D>();
		vector<Vector3D> vectorData = vector<Vector3D>();
		vector<TexCoord2D> texCoordData = vector<TexCoord2D>();
		
		// validate the vertexes (i.e. make sure they all have 3 values)
		if(vertexVals.size() > 0) {
			vector<string> tmp;
			for(vector<string>::iterator i = vertexVals.begin(); i != vertexVals.end(); i++) {
				tmp = BZWParser::getLineElements( i->c_str() );
				if(tmp.size() != 3) {
					printf("mesh::DrawInfo::update(): Error! \"vertex\" in \"vertex %s\" needs 3 values!\n", i->c_str());
					return 0;
				}
				vertexData.push_back( Point3D( i->c_str() ) );
			}
		}
		
		// validate the corners (i.e. make sure they all have 3 values)
		if(cornerVals.size() > 0) {
			vector<string> tmp;
			for(vector<string>::iterator i = cornerVals.begin(); i != cornerVals.end(); i++) {
				tmp = BZWParser::getLineElements( i->c_str() );
				if(tmp.size() != 3) {
					printf("mesh::DrawInfo::update(): Error! \"corner\" in \"corner %s\" needs 3 values!\n", i->c_str());
					return 0;
				}
				cornerData.push_back( Index3D( i->c_str() ) );
			}
		}
		
		// validate the normals (i.e. make sure they all have 3 values)
		if(normalVals.size() > 0) {
			vector<string> tmp;
			for(vector<string>::iterator i = normalVals.begin(); i != normalVals.end(); i++) {
				tmp = BZWParser::getLineElements( i->c_str() );
				if(tmp.size() != 3) {
					printf("mesh::DrawInfo::update(): Error! \"normal\" in \"normal %s\" needs 3 values!\n", i->c_str());
					return 0;
				}
				vectorData.push_back( Vector3D( i->c_str() ) );
			}
		}
		
		// validate the texture coordinates (i.e. make sure they all have 2 values)
		if(vertexVals.size() > 0) {
			vector<string> tmp;
			for(vector<string>::iterator i = texCoordVals.begin(); i != texCoordVals.end(); i++) {
				tmp = BZWParser::getLineElements( i->c_str() );
				if(tmp.size() != 2) {
					printf("mesh::DrawInfo::update(): Error! \"texcoord\" in \"texcoord %s\" needs 2 values!\n", i->c_str());
					return 0;
				}
				texCoordData.push_back( TexCoord2D( i->c_str() ) );
			}
		}
		
		// get the "extends" if it exists
		Point3D eLow = Point3D(0.0f, 0.0f, 0.0f), eHigh = Point3D(0.0f, 0.0f, 0.0f);
		if(extendsVals.size() > 0) {
			vector<string> extendsParams = BZWParser::getLineElements( extendsVals[0].c_str() );
			string eLowString = extendsParams[0] + " " + extendsParams[1] + " " + extendsParams[2];
			string eHighString = extendsParams[3] + " " + extendsParams[4] + " " + extendsParams[5];
			eLow = Point3D(eLowString.c_str());
			eHigh = Point3D(eHighString.c_str());
		}
		
		// get the "sphere" if it exists
		Point3D spherePoint(0.0f, 0.0f, 0.0f);
		float sphereRad = 0;
		if(sphereVals.size() > 0) {
			vector<string> sphereParams = BZWParser::getLineElements( sphereVals[0].c_str() );
			string pString = sphereParams[0] + " " + sphereParams[1] + " " + sphereParams[2];
			sphereRad = atof( sphereParams[3].c_str() );
			spherePoint = Point3D(pString.c_str());
		}
		// get LOD blocks
		vector<string> lodBlocks = BZWParser::getSectionsByHeader( "lod", drawInfoData);
		vector<LOD> lodElements = vector<LOD>();
		if(lodBlocks.size() > 0) {
			for(vector<string>::iterator i = lodBlocks.begin(); i != lodBlocks.end(); i++) {
				lodElements.push_back( LOD( *i ) );
			}
		}
		
		// do base-class update
		if(!DataEntry::update(data))
			return 0;
		
		// finally, set the data
		this->vertices = vertexData;
		this->texcoords = texCoordData;
		this->corners = cornerData;
		this->normals = vectorData;
		this->minExtends = eLow;
		this->maxExtends = eHigh;
		this->sphereRadius = sphereRad;
		this->spherePosition = spherePoint;
		this->angvel = (angvelVals.size() > 0 ? atof( angvelVals[0].c_str() ) : 0.0f);
		this->dlist = (dlistVals.size() > 0 ? true : false);
		this->decorative = (decorativeVals.size() > 0 ? true : false);
		this->lods = lodElements;
		
		return 1;
	}
	
	// toString
	string toString(void) {
		// string-ify the vertices, normals, corners, LODs, and texcoords
		string vertexString(""), normalString(""), texcoordString(""), cornerString(""), lodString("");
		
		if(vertices.size() > 0) {
			for(vector<Point3D>::iterator i = vertices.begin(); i != vertices.end(); i++) {
				vertexString += "    vertex " + i->toString();
			}		
		}
		
		if(normals.size() > 0) {
			for(vector<Vector3D>::iterator i = normals.begin(); i != normals.end(); i++) {
				normalString += "    normal " + i->toString();
			}		
		}
		
		if(texcoords.size() > 0) {
			for(vector<TexCoord2D>::iterator i = texcoords.begin(); i != texcoords.end(); i++) {
				texcoordString += "    texcoord " + i->toString() + "\n";	
			}
		}
		
		if(corners.size() > 0) {
			for(vector<Index3D>::iterator i = corners.begin(); i != corners.end(); i++) {
				cornerString += "    corner " + i->toString();	
			}	
		}
		
		if(lods.size() > 0) {
			for(vector<LOD>::iterator i = lods.begin(); i != lods.end(); i++) {	
				lodString += "    " + i->toString();
			}
		}
		
		return string("drawinfo\n") +
					  (dlist == true ? "    dlist\n" : "") +
					  (decorative == true ? "    decorative\n" : "") +
					  vertexString +
					  normalString +
					  cornerString +
					  texcoordString +
					  lodString +
					  "  end\n";
	}
	
	// render
	int render(void) {
		return 0;	
	}
	
	// binary getters
	vector<Point3D>& getVertices() { return vertices; }
	
private:

	/**
	 * Helper method: convert an array of strings into an array of floats
	 */
	vector<float> parseValues(vector<string>& values) {
		vector<float> ret = vector<float>();
		
		if(values.size() > 0) {
			for(vector<string>::iterator i = values.begin(); i != values.end(); i++) {
				ret.push_back( atof( i->c_str() ) );	
			}
		}
		
		return ret;
	}

	/**
	 * Helper method: convert an array of floats into a string
	 */
	 
	string stringify(vector<float>& values) {
		string ret = string("");
		
		if(values.size() > 0) {
			for(vector<float>::iterator i = values.begin(); i != values.end(); i++) {
				ret += string(ftoa(*i)) + " ";	
			}	
		}
		
		return ret;
	}

	vector<Point3D> vertices;
	vector<Vector3D> normals;
	vector<TexCoord2D> texcoords;
	vector<Index3D> corners;
	vector<LOD> lods;
	
	Point3D minExtends, maxExtends, spherePosition;
	
	float angvel, sphereRadius;
	
	bool dlist, decorative;
};

#endif /*DRAWINFO_H_*/
