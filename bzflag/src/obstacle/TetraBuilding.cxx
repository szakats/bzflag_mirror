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

#include <math.h>
#include "common.h"
#include "global.h"
#include "TetraBuilding.h"
#include "Intersect.h"


const char*		TetraBuilding::typeName = "TetraBuilding";

static bool makePlane (const float* p1, const float* p2, const float* pc, float* r);


TetraBuilding::TetraBuilding(const float (*_vertices)[3], const bool *_visible,
			                 const bool *_colored, const float (*_colors)[4],
			                 bool drive, bool shoot) :
                             Obstacle(_vertices[0], 0.0f, 50.0f, 50.0f, 50.0f, drive, shoot) //FIXME
{
  memcpy (vertices, _vertices, 4 * sizeof (float[3]));
  memcpy (visible, _visible, 4 * sizeof (bool));
  memcpy (colored, _colored, 4 * sizeof (bool));
  memcpy (colors, _colors, 4 * sizeof (float[4]));
  
  // make sure the the planes are facing outwards
  float edge[3][3]; // edges from vertex 0
  for (int v = 0; v < 3; v++) {
    for (int a = 0; a < 3; a++) {
      edge[v][a] = vertices[v+1][a] - vertices[0][a];
    }
  }
  float cross[3];
  cross[0] = (edge[0][1] * edge[1][2]) - (edge[0][2] * edge[1][1]);
  cross[1] = (edge[0][2] * edge[1][0]) - (edge[0][0] * edge[1][2]);
  cross[2] = (edge[0][0] * edge[1][1]) - (edge[0][1] * edge[1][0]);
  float dot =
    (cross[0] * edge[2][0]) + (cross[1] * edge[2][1]) + (cross[2] * edge[2][2]);
  // swap vertices 1 & 2 if we are out of order
  if (dot < 0.0f) {
    memcpy (vertices[1], _vertices[2], sizeof(float[3]));
    memcpy (vertices[2], _vertices[1], sizeof(float[3]));
    visible[1] = _visible[2];
    visible[2] = _visible[1];
  }

  // FIXME - what to do here if one of the planes fails?
  // make outward facing normals to the planes
  makePlane (vertices[1], vertices[2], vertices[3], planes[0]);
  makePlane (vertices[0], vertices[3], vertices[2], planes[1]);
  makePlane (vertices[0], vertices[1], vertices[3], planes[2]);
  makePlane (vertices[0], vertices[2], vertices[1], planes[3]);
  
  return;
}                             


TetraBuilding::~TetraBuilding()
{
  // do nothing
  return;
}


const char*		TetraBuilding::getType() const
{
  return typeName;
}


const char*		TetraBuilding::getClassName() // const
{
  return typeName;
}


void			TetraBuilding::getCorner(int index,
						float* pos) const
{
  memcpy (pos, vertices[index], 3 * sizeof(float));
  return;
}


static bool makePlane (const float* p1, const float* p2, const float* pc,
                       float* r)
{
  // make vectors from points
  float x[3] = {p1[0] - pc[0], p1[1] - pc[1], p1[2] - pc[2]};
  float y[3] = {p2[0] - pc[0], p2[1] - pc[1], p2[2] - pc[2]};
  float n[3];

  // cross product to get the normal
  n[0] = (x[1] * y[2]) - (x[2] * y[1]);
  n[1] = (x[2] * y[0]) - (x[0] * y[2]);
  n[2] = (x[0] * y[1]) - (x[1] * y[0]);

  // normalize
  float len = (n[0] * n[0]) + (n[1] * n[1]) + (n[2] * n[2]);
  if (len < +0.001f) {
    return false;
  } else {
    len = 1.0f / sqrtf (len);
  }
  r[0] = n[0] * len;
  r[1] = n[1] * len;
  r[2] = n[2] * len;

  // finish the plane equation: {rx*px + ry*py + rz+pz + rd = 0}
  r[3] = -((pc[0] * r[0]) + (pc[1] * r[1]) + (pc[2] * r[2]));

  return true;
}                       


float			TetraBuilding::intersect(const Ray& ray) const
{
  // NOTE: i'd use a quick test here first, but the
  //       plan is to use an octree for the collision
  //       manager which should get us close enough
  //       that a quick test might actually eat up time.
  //
  // find where the ray crosses each plane, and then
  // check the dot-product of the three bounding planes
  // to see if the intersection point is contained within 
  // the face.
  //
  //  L - line unit vector          Lo - line origin
  //  N - plane normal unit vector  d  - plane offset
  //  P - point in question         t - time
  //  
  //  (N dot P) + d = 0                      { plane equation }
  //  P = (t * L) + Lo                       { line equation }
  //  t (N dot L) + (N dot Lo) + d = 0
  //
  //  t = - (d + (N dot Lo)) / (N dot L)     { time of impact }
  //
  int p;
  const float* dir = ray.getDirection();
  const float* origin = ray.getOrigin();
  float times[4];
  
  // get the time until the shot would hit each plane
  for (p = 0; p < 4; p++) {
    const float linedot = (planes[p][0] * dir[0]) +
                          (planes[p][1] * dir[1]) +
                          (planes[p][2] * dir[2]);
    if (linedot >= -0.001f) {
      // shot is either parallel, or going through backwards
      times[p] = Infinity;
      continue;
    }
    const float origindot = (planes[p][0] * origin[0]) +
                            (planes[p][1] * origin[1]) +
                            (planes[p][2] * origin[2]);
    // linedot should be safe to divide with now
    times[p] = - (planes[p][3] + origindot) / linedot;
    if (times[p] < 0.0f) {
      times[p] = Infinity;
    }
  }
  
  // sort, smallest time first - FIXME (ick, bubble sort)
  int order[4] = { 0, 1, 2, 3 };
  for (int i = 3; i > 0; i--) {
    for (int j = 3; j > (3 - i); j--)
      if (times[order[j]] < times[order[j - 1]]) {
        int tmp = order[j];
        order[j] = order[j - 1];
        order[j - 1] = tmp;
    }
  }
    
  // see if the point is within the face
  for (p = 0; p < 4; p++) {
    int target = order[p];
    if (times[target] == Infinity) {
      continue;
    }
    float targetTime = times[target];
    // get the contact location
    float point[3];
    point[0] = (dir[0] * targetTime) + origin[0];
    point[1] = (dir[1] * targetTime) + origin[1];
    point[2] = (dir[2] * targetTime) + origin[2];
    bool gotFirstHit = true;
    // now test against the planes
    for (int q = 0; q < 4; q++) {
      if (q == target) {
        continue;
      }
      float d = (planes[q][0] * point[0]) +
                (planes[q][1] * point[1]) +
                (planes[q][2] * point[2]) + planes[q][3];
      if (d > 0.001f) {
        gotFirstHit = false;
        break;
      }      
    }
    if (gotFirstHit) {
      return targetTime;
    }
  }

  return -1.0f;
}


void			TetraBuilding::get3DNormal(const float* p, float* n) const
{
  // FIXME: this is silly, we should just use the computations in
  // intersect() to pull out this information, maybe have intersect
  // return an 'int' relating to a collision code (each obstacle 
  // type gets to define its own, for tetra (1-4 = planes, 5-10 = edges)? 
  float closestDist = -Infinity;
  int closestPlane = -1;
  for (int i = 0; i < 4; i++) {
    float d = (planes[i][0] * p[0]) +
              (planes[i][1] * p[1]) +
              (planes[i][2] * p[2]) + planes[i][3];
    if (fabsf(d) < fabsf(closestDist)) {
      closestDist = d;
      closestPlane = i;
    }
  }
  memcpy (n, planes[closestPlane], sizeof(float[3]));
  return;
}


/////////////////////////////////////////////////////////////
//  FIXME - everything after this point is currently JUNK! //
/////////////////////////////////////////////////////////////



void			TetraBuilding::getNormal(const float* p,
							float* n) const
{
  p = p;
  n[0] = 0.0f;
  n[1] = 0.0f;
  n[2] = +1.0f;
}


bool			TetraBuilding::getHitNormal(
				const float* pos1, float,
				const float* pos2, float,
				float, float, float height,
				float* normal) const
{
  pos1 = pos1;
  pos2 = pos2;
  height = height;
  normal[0] = 0.0f;
  normal[1] = 0.0f;
  normal[2] = +1.0f;
  return false;
}


bool			TetraBuilding::inCylinder(const float* p,
						float radius, float height) const
{
  p = p;
  radius = height;
  if ((fabsf(p[0] - vertices[0][0]) < 10.0f) &&
      (fabsf(p[1] - vertices[0][1]) < 10.0f)) {
    return true;
  }
  return false;
}


bool			TetraBuilding::inBox(const float* p, float a,
						float dx, float dy, float height) const
{
  p = p;
  dx = dy = a = height;
  if ((fabsf(p[0] - vertices[0][0]) < 10.0f) &&
      (fabsf(p[1] - vertices[0][1]) < 10.0f)) {
    return true;
  }
  return false;
}


bool			TetraBuilding::inMovingBox(const float*, float,
                                             const float* p, float angle,
                                             float dx, float dy, float dz) const
{
  p = p;
  dx = dy;
  dz = angle;
  if ((fabsf(p[0] - vertices[0][0]) < 10.0f) &&
      (fabsf(p[1] - vertices[0][1]) < 10.0f)) {
    return true;
  }
  return false;
}                                          


bool			TetraBuilding::isCrossing(const float* p, float a,
					float dx, float dy, float height, float* plane) const
{
  p = p;
  dx = dy = a = height;
  if ((fabsf(p[0] - vertices[0][0]) < 10.0f) &&
      (fabsf(p[1] - vertices[0][1]) < 10.0f)) {
    if (plane != NULL) {
      plane[0] = 0.0f;
      plane[1] = 0.0f;
      plane[2] = 1.0f;
      plane[3] = 0.0f;
    }
    return true;
  }
  return false;
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

