/* bzflag
 * Copyright (c) 1993 - 2004 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* SphereObstacle:
 *	Encapsulates a cone in the game environment.
 */

#ifndef	BZF_SPHERE_OBSTACLE_H
#define	BZF_SPHERE_OBSTACLE_H

#include "common.h"
#include <string>
#include "Obstacle.h"
#include "MeshObstacle.h"
#include "BzMaterial.h"


class SphereObstacle : public Obstacle {
  public:

    enum {
      Edge,
      Bottom,
      MaterialCount
    };
    
    SphereObstacle();
    SphereObstacle(const float* _pos, const float* _size,
                   float _rotation, const float _texsize[2], 
                   bool _useNormals, bool hemisphere,
                   int _divisions, const BzMaterial* mats[MaterialCount],
                   bool bounce, bool drive, bool shoot);
    ~SphereObstacle();
    MeshObstacle* getMesh();

    const char* getType() const;
    static const char* getClassName(); // const
    bool isValid() const;
    void getExtents(float* mins, float* maxs) const;

    float intersect(const Ray&) const;
    void getNormal(const float* p, float* n) const;
    void get3DNormal(const float* p, float* n) const;

    bool inCylinder(const float* p, float radius, float height) const;
    bool inBox(const float* p, float angle,
               float halfWidth, float halfBreadth, float height) const;
    bool inMovingBox(const float* oldP, float oldAngle,
                     const float *newP, float newAngle,
                     float halfWidth, float halfBreadth, float height) const;
    bool isCrossing(const float* p, float angle,
                    float halfWidth, float halfBreadth, float height,
                    float* plane) const;

    bool getHitNormal(const float* pos1, float azimuth1,
                      const float* pos2, float azimuth2,
                      float halfWidth, float halfBreadth,
                      float height, float* normal) const;

    void *pack(void*);
    void *unpack(void*);
    int packSize();

    void print(std::ostream& out, int level);

  private:
    void finalize();

  private:
    static const char* typeName;

    MeshObstacle* mesh;
    
    int divisions;
    bool hemisphere;
    bool smoothBounce;
    bool useNormals;
    float texsize[2];
    const BzMaterial* materials[MaterialCount];
};


#endif // BZF_SPHERE_OBSTACLE_H

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

