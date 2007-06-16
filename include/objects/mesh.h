#ifndef MESH_H_
#define MESH_H_

#include "bz2object.h"
#include "../render/Point3D.h"
#include "../render/Index3D.h"
#include "../render/TexCoord2D.h"
#include "../render/Vector3D.h"
#include "../MeshFace.h"
#include "../LOD.h"
#include "../LODCommand.h"
#include "../DrawInfo.h"

class mesh : public bz2object {
	
public:

	// constructor
	mesh(void);
	
	// constructor with data
	mesh(string& data);
	
	static DataEntry* init() { return new mesh(); }
	static DataEntry* init(string& data) { return new mesh(data); }
	
	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:

	// vertices
	vector<Point3D> vertices;
	
	// texture coordinates
	vector<TexCoord2D> texCoords;
	
	// normals
	vector<Vector3D> normals;
	
	// inside points
	vector<Point3D> insidePoints;
	
	// outside points
	vector<Point3D> outsidePoints;
	
	// boolean options
	bool noClusters, smoothBounce;
	
	// faces
	vector<MeshFace> faces;
	
	// draw info
	DrawInfo drawInfo;
	bool useDrawInfo;
};

#endif /*MESH_H_*/
