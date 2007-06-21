#include "../include/objects/mesh.h"

// default constructor
mesh::mesh(void) :
	bz2object("mesh", "<vertex><normal><texcoord><inside><outside><shift><scale><shear><spin><phydrv><smoothbounce><noclusters><face><drawinfo>") {
	
	vertices = vector<Point3D>();
	texCoords = vector<TexCoord2D>();
	normals = vector<Vector3D>();
	insidePoints = vector<Point3D>();
	outsidePoints = vector<Point3D>();
	noClusters = smoothBounce = useDrawInfo = false;
	materials = vector<string>();
	faces = vector<MeshFace>();
	drawInfo = DrawInfo();
	materialMap = map<string, string>();
}

// constructor with data
mesh::mesh(string& data) :
	bz2object("mesh", "<vertex><normal><texcoord><inside><outside><shift><scale><shear><spin><phydrv><smoothbounce><noclusters><face><drawinfo>", data.c_str()) {
	
	vertices = vector<Point3D>();
	texCoords = vector<TexCoord2D>();
	normals = vector<Vector3D>();
	insidePoints = vector<Point3D>();
	outsidePoints = vector<Point3D>();
	noClusters = smoothBounce = useDrawInfo = false;
	materials = vector<string>();
	faces = vector<MeshFace>();
	drawInfo = DrawInfo();
	materialMap = map<string, string>();
	
	this->update(data);
}

// getter
string mesh::get(void) { return this->toString(); }

// setter
int mesh::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get lines
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str(), "end", "<drawinfo><face>", "");
	
	// get the lines without the drawinfo (so we don't mistake drawinfo's data with the global data)
	vector<string> linesNoDrawInfo = BZWParser::getSectionsByHeader(header, data.c_str(), "end", "<drawinfo><face>", "<drawinfo>");
	
	// get lines without faces and without drawinfo (so bz2object doesn't think there are multiple physics drivers)
	vector<string> linesNoSubobjects = BZWParser::getSectionsByHeader(header, data.c_str(), "end", "<drawinfo><face>", "<drawinfo><face>");
	
	// break if there are none
	if(lines[0] == BZW_NOT_FOUND) {
		printf("mesh not found\n");
		return 0;
	}
		
	// break if too many
	if(!hasOnlyOne(lines, header))
		return 0;
		
	// get the data
	const char* meshData = lines[0].c_str();
	
	// get drawinfo-less data
	const char* meshDataNoDrawInfo = meshData;
	if(linesNoDrawInfo.size() > 0)
		meshDataNoDrawInfo = linesNoDrawInfo[0].c_str();
		
	// get no-subobject data
	const char* meshDataNoSubobjects = meshData;
	if(linesNoSubobjects.size() > 0)
		meshDataNoSubobjects = linesNoSubobjects[0].c_str();
	
	// get the vertices
	vector<string> vertexVals = BZWParser::getValuesByKey("vertex", header, meshDataNoDrawInfo);
	
	// get the texcoords
	vector<string> texCoordVals = BZWParser::getValuesByKey("texcoord", header, meshDataNoDrawInfo);
	
	// get the normals
	vector<string> normalVals = BZWParser::getValuesByKey("normal", header, meshDataNoDrawInfo);
		
	// get inside points
	vector<string> insidePointVals = BZWParser::getValuesByKey("inside", header, meshDataNoDrawInfo);
	
	// get outside points
	vector<string> outsidePointVals = BZWParser::getValuesByKey("outside", header, meshDataNoDrawInfo);
	
	// get faces
	vector<string> faceVals = BZWParser::getSectionsByHeader("face", meshDataNoDrawInfo, "endface");
	
	// get materials in the order they come in relation to faces.
	// since we know how many faces there are in faceVals, we can
	// deduce which materials refer to which faces by counting the number
	// of faces between matrefs and later mapping those faces in faceVals
	// to those materials since in both matrefs and faceVals the faces
	// come in the same order.
	vector<string> matFaceKeys = vector<string>();
	matFaceKeys.push_back("face");
	matFaceKeys.push_back("matref");
	vector<string> matrefs = BZWParser::getLinesByKeys(matFaceKeys, header, meshDataNoDrawInfo);
	
	// get drawinfo
	vector<string> drawInfoVals = BZWParser::getSectionsByHeader("drawinfo", meshData);
	if(drawInfoVals.size() > 1) {
		printf("mesh::update(): Error! Defined \"drawinfo\" %d times\n", drawInfoVals.size());
		return 0;
	}
		
	// get noclusters
	vector<string> noClusterVals = BZWParser::getValuesByKey("noclusters", header, meshData);
	
	// get smoothbounce
	vector<string> smoothBounceVals = BZWParser::getValuesByKey("smoothbounce", header, meshData);
	
	// parse faces
	vector<MeshFace> faceParams = vector<MeshFace>();
	if(faceVals.size() > 0) {
		for(vector<string>::iterator i = faceVals.begin(); i != faceVals.end(); i++) {
			faceParams.push_back( MeshFace(*i) );	
		}	
	}
	
	// parse material map.
	// deduce which faces map to which materials by counting the number of faces between
	// matrefs and mapping faces from faceParams to materials
	map<string, string> matmap = map<string, string>();
	if(matrefs.size() > 0 && faceParams.size() > 0) {
		string activeMaterial = "";		// the current material that faces will be mapped to
		vector<string> lineElements;	// temporary container for elements of each matrefs line
		unsigned int faceParamsIndex = 0;		// used to index faceParams to map its elements to materials
		for(vector<string>::iterator i = matrefs.begin(); i != matrefs.end(); i++) {
			// get the line elements
			lineElements = BZWParser::getLineElements(i->c_str());
			if(lineElements.size() == 0)
				continue;
			
			// see if we came across a face
			if(lineElements[0] == "face") {
				// if so, then map the face to activeMaterial
				matmap[ faceParams[faceParamsIndex].toString() ] = string(activeMaterial);
				faceParamsIndex++;
			}
			// see if we came across a valid matref
			else if(lineElements[0] == "matref") {
				// if so, then reset the active material
				if(lineElements.size() > 1)
					activeMaterial = lineElements[1];
				else
					activeMaterial = "-1";
				
			}
			
			// break if there are no more faces to map
			if(faceParamsIndex >= faceParams.size())
				break;
		}
	}
	
	// parse drawinfo
	DrawInfo drawInfoParam = DrawInfo();
	bool doDrawInfo = false;
	if(drawInfoVals[0] != BZW_NOT_FOUND) {
		drawInfoParam = DrawInfo(drawInfoVals[0]);
		doDrawInfo = true;
	}
	
	// parse the values
	vector<Point3D> vertexData = vector<Point3D>();
	vector<Vector3D> vectorData = vector<Vector3D>();
	vector<TexCoord2D> texCoordData = vector<TexCoord2D>();
	vector<Point3D> insideData = vector<Point3D>();
	vector<Point3D> outsideData = vector<Point3D>();
	
	// validate the vertexes (i.e. make sure they all have 3 values)
	if(vertexVals.size() > 0) {
		vector<string> tmp;
		for(vector<string>::iterator i = vertexVals.begin(); i != vertexVals.end(); i++) {
			tmp = BZWParser::getLineElements( i->c_str() );
			if(tmp.size() != 3) {
				printf("mesh::update(): Error! \"vertex\" in \"vertex %s\" needs 3 values!\n", i->c_str());
				return 0;
			}
			vertexData.push_back( Point3D( i->c_str() ) );
		}
	}
	
	// validate the normals (i.e. make sure they all have 3 values)
	if(normalVals.size() > 0) {
		vector<string> tmp;
		for(vector<string>::iterator i = normalVals.begin(); i != normalVals.end(); i++) {
			tmp = BZWParser::getLineElements( i->c_str() );
			if(tmp.size() != 3) {
				printf("mesh::update(): Error! \"normal\" in \"normal %s\" needs 3 values!\n", i->c_str());
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
				printf("mesh::update(): Error! \"texcoord\" in \"texcoord %s\" needs 2 values!\n", i->c_str());
				return 0;
			}
			texCoordData.push_back( TexCoord2D( i->c_str() ) );
		}
	}
	
	// validate the inside points
	if(insidePointVals.size() > 0) {
		vector<string> tmp;
		for(vector<string>::iterator i = insidePointVals.begin(); i != insidePointVals.end(); i++) {
			tmp = BZWParser::getLineElements( i->c_str() );
			if(tmp.size() != 3) {
				printf("mesh::update(): Error! \"inside\" in \"inside %s\" needs 1 value!\n", i->c_str());
				return 0;	
			}
			insideData.push_back( Point3D( i->c_str() ) );
		}	
	}
	
	// validate the outside points
	if(outsidePointVals.size() > 0) {
		vector<string> tmp;
		for(vector<string>::iterator i = outsidePointVals.begin(); i != outsidePointVals.end(); i++) {
			tmp = BZWParser::getLineElements( i->c_str() );
			if(tmp.size() != 3) {
				printf("mesh::update(): Error! \"outside\" in \"outside %s\" needs 1 value!\n", i->c_str());
				return 0;	
			}
			outsideData.push_back( Point3D( i->c_str() ) );
		}	
	}
	
	// do base-class update
	string bz2object_data = string(meshDataNoSubobjects);
	if(!bz2object::update(bz2object_data)) {
		printf("mesh::update(): Error! could not do bz2object update!\n");
		return 0;
	}
		
	// load data in
	vertices = vertexData;
	texCoords = texCoordData;
	normals = vectorData;
	drawInfo = drawInfoParam;
	useDrawInfo = doDrawInfo;
	insidePoints = insideData;
	outsidePoints = outsideData;
	faces = faceParams;
	noClusters = (noClusterVals.size() == 0 ? false : true);
	smoothBounce = (smoothBounceVals.size() == 0 ? false : true);
	materialMap = matmap;
	
	return 1;
}

// to string
string mesh::toString(void) {
	// string-ify the vertices, normals, texcoords, inside points, outside points, and faces
	string vertexString(""), normalString(""), texcoordString(""), insideString(""), outsideString(""), faceString("");
	
	if(vertices.size() > 0) {
		for(vector<Point3D>::iterator i = vertices.begin(); i != vertices.end(); i++) {
			vertexString += "  vertex " + i->toString();
		}		
	}
	
	if(normals.size() > 0) {
		for(vector<Vector3D>::iterator i = normals.begin(); i != normals.end(); i++) {
			normalString += "  normal " + i->toString();
		}		
	}
	
	if(texCoords.size() > 0) {
		for(vector<TexCoord2D>::iterator i = texCoords.begin(); i != texCoords.end(); i++) {
			texcoordString += "  texcoord " + i->toString() + "\n";	
		}
	}
	
	// special instance:
	// make sure to keep the order of faces and materials constant
	if(faces.size() > 0) {
		string currMaterial = "";
		for(vector<MeshFace>::iterator i = faces.begin(); i != faces.end(); i++) {
			// see if we came across a new material
			if(currMaterial != materialMap[ i->toString() ]) {
				// if so, set the current material and append the matref to the faceString
				currMaterial = materialMap[ i->toString() ];
				faceString += "  matref " + currMaterial + "\n";
			}
			faceString += "  " + i->toString();	
		}	
	}
	
	if(insidePoints.size() > 0) {
		for(vector<Point3D>::iterator i = insidePoints.begin(); i != insidePoints.end(); i++) {
			insideString += "  inside " + i->toString();
		}		
	}
	
	if(outsidePoints.size() > 0) {
		for(vector<Point3D>::iterator i = outsidePoints.begin(); i != outsidePoints.end(); i++) {
			outsideString += "  outside " + i->toString();
		}		
	}
	
	return string("mesh\n") +
				  (noClusters == true ? "  noclusters\n" : "") +
				  (smoothBounce == true ? "  smoothbounce\n" : "") +
				  insideString +
				  outsideString +
				  vertexString +
				  normalString +
				  texcoordString +
				  faceString +
				  (useDrawInfo == true ? "  " + drawInfo.toString() : "") + "\n" +
				  "end\n";
}

// render
int mesh::render(void) {
	return 0;
}
