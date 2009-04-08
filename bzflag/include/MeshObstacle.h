/* bzflag
 * Copyright (c) 1993 - 2009 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* TetraBuilding:
 *	Encapsulates a tetrahederon in the game environment.
 */

#ifndef	BZF_MESH_OBSTACLE_H
#define	BZF_MESH_OBSTACLE_H

#include "common.h"
#include <string>
#include <vector>
#include <iostream>
#include "vectors.h"
#include "Ray.h"
#include "Obstacle.h"
#include "MeshFace.h"
#include "MeshTransform.h"

class MeshDrawInfo;

class MeshObstacle : public Obstacle {
  public:
    MeshObstacle();
    MeshObstacle(const MeshTransform& transform,
		 const std::vector<char>& checkTypes,
		 const std::vector<fvec3>& checkPoints,
		 const std::vector<fvec3>& vertices,
		 const std::vector<fvec3>& normals,
		 const std::vector<fvec2>& texcoords,
		 int faceCount, bool noclusters, bool bounce,
		 unsigned char drive, unsigned char shoot, bool ricochet);

    bool addFace(const std::vector<int>& vertices,
                 const std::vector<int>& normals,
                 const std::vector<int>& texcoords,
                 const BzMaterial* bzMaterial, int physics,
                 bool noclusters, bool bounce,
                 unsigned char drive, unsigned char shoot, bool ricochet,
                 bool triangulate);

    ~MeshObstacle();

    void finalize();

    Obstacle* copyWithTransform(const MeshTransform&) const;
    void copyFace(int face, MeshObstacle* mesh) const;

    void setName(const std::string& name);
    const std::string&	getName() const;

    enum CheckType {
      CheckInside =  0,
      CheckOutside = 1,
      InsideParity = 2,
      OutsidePartiy = 3
    };

    const char* getType() const;
    static const char* getClassName(); // const
    bool isValid() const;

    float intersect(const Ray&) const;
    void getNormal(const fvec3& p, fvec3& n) const;
    void get3DNormal(const fvec3& p, fvec3& n) const;

    bool inCylinder(const fvec3& p, float radius, float height) const;
    bool inBox(const fvec3& p, float angle,
	       float halfWidth, float halfBreadth, float height) const;
    bool inMovingBox(const fvec3& oldP, float oldAngle,
		     const fvec3& newP, float newAngle,
		     float halfWidth, float halfBreadth, float height) const;
    bool isCrossing(const fvec3& p, float angle,
		    float halfWidth, float halfBreadth, float height,
		    fvec4* plane) const;

    bool getHitNormal(const fvec3& pos1, float azimuth1,
		      const fvec3& pos2, float azimuth2,
		      float halfWidth, float halfBreadth,
		      float height, fvec3& normal) const;

    bool containsPoint(const fvec3& point) const;
    bool containsPointNoOctree(const fvec3& point) const;

    const char *getCheckTypes() const;
    const fvec3 *getCheckPoints() const;
    const fvec3 *getVertices() const;
    const fvec3 *getNormals() const;
    const fvec2 *getTexcoords() const;
    int getVertexCount() const;
    int getNormalCount() const;
    int getTexcoordCount() const;
    int getFaceCount() const;
    const MeshFace* getFace(int face) const;
    bool useSmoothBounce() const;
    bool noClusters() const;

    const MeshDrawInfo* getDrawInfo() const;
    void setDrawInfo(MeshDrawInfo*);

    int packSize() const;
    void *pack(void*) const;
    void *unpack(void*);

    void print(std::ostream& out, const std::string& indent) const;
    void printOBJ(std::ostream& out, const std::string& indent) const;

    virtual int getTypeID() const {return meshType;}

  private:
    void makeFacePointers(const std::vector<int>& _vertices,
			  const std::vector<int>& _normals,
			  const std::vector<int>& _texcoords,
			  fvec3**& v, fvec3**& n, fvec2**& t);

  private:
    static const char* typeName;

    std::string name;

    int checkCount;
    char* checkTypes;
    fvec3* checkPoints;
    int vertexCount;
    fvec3* vertices;
    int normalCount;
    fvec3* normals;
    int texcoordCount;
    fvec2* texcoords;
    int faceCount, faceSize;
    MeshFace** faces;
    bool smoothBounce;
    bool noclusters;
    bool inverted; // used during building. can be ditched if
		   // edge tables are setup with bi-directional
		   // ray-vs-face tests and parity counts.

    MeshDrawInfo* drawInfo; // hidden data stored in extra texcoords
};


inline const char* MeshObstacle::getCheckTypes() const
{
  return checkTypes;
}

inline const fvec3* MeshObstacle::getCheckPoints() const
{
  return checkPoints;
}

inline const fvec3* MeshObstacle::getVertices() const
{
  return vertices;
}

inline const fvec3* MeshObstacle::getNormals() const
{
  return normals;
}

inline const fvec2* MeshObstacle::getTexcoords() const
{
  return texcoords;
}

inline int MeshObstacle::getVertexCount() const
{
  return vertexCount;
}

inline int MeshObstacle::getNormalCount() const
{
  return normalCount;
}

inline int MeshObstacle::getTexcoordCount() const
{
  return texcoordCount;
}

inline int MeshObstacle::getFaceCount() const
{
  return faceCount;
}

inline const MeshFace* MeshObstacle::getFace(int face) const
{
  return faces[face];
}

inline bool MeshObstacle::useSmoothBounce() const
{
  return smoothBounce;
}

inline bool MeshObstacle::noClusters() const
{
  return noclusters;
}

inline const MeshDrawInfo* MeshObstacle::getDrawInfo() const
{
  return drawInfo;
}

inline const std::string& MeshObstacle::getName() const
{
  return name;
}

inline void MeshObstacle::setName(const std::string& str)
{
  name = str;
}


#endif // BZF_MESH_OBSTACLE_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
