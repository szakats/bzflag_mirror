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
#include "Pack.h"



// FIXME - some these should go into a common file
// Some handy geometry functions

static inline void vec3sub (float *result, const float* v1, const float* v2)
{
  result[0] = v1[0] - v2[0];
  result[1] = v1[1] - v2[1];
  result[2] = v1[2] - v2[2];
  return;
}
static inline float vec3dot (const float* v1, const float* v2)
{
  return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}
static inline void vec3cross (float* result, const float* v1, const float* v2)
{
  result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
  result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
  result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
  return;
}
static inline void projectTetra (float *dists, const float* normal,
                                 const float (*vertices)[3])
{
  dists[0] = vec3dot(normal, vertices[0]);
  dists[1] = vec3dot(normal, vertices[1]);
  dists[2] = vec3dot(normal, vertices[2]);
  dists[3] = vec3dot(normal, vertices[3]);
  return;
}
// NOTE: unlike the rest of bzflag code, this OrigBox is centered
//       at the origin, with 'sizes' extents radiating out from it
static inline void projectOrigBox (float* dist, const float* normal,
                                   const float* sizes)
{
  // setup the test corner
  // this can be determined easily based
  // on the normal vector for the plane
  float corner[3];
  for (int a = 0; a < 3; a++) {
    if (normal[a] > 0.0f) {
      corner[a] = +sizes[a];
    } else {
      corner[a] = -sizes[a];
    }
  }
  *dist = fabsf(vec3dot(normal, corner));
  return;
}
static inline void makeNormal (const float* p1, const float* p2,
                               const float* pc, float* r)
{
  // make vectors from points
  float x[3], y[3];
  // make some vectors
  vec3sub (x, p1, pc);
  vec3sub (y, p2, pc);
  // cross product to get the normal
  vec3cross (r, x, y);
  return;
}
inline bool TetraBuilding::checkTest (int testNumber) const
{ 
  int i;
  const planeTest* pt = &axisTests[testNumber];
  const float* td = pt->tetraDists;

  float minT = +MAXFLOAT;
  float maxT = -MAXFLOAT;
  for (i = 0; i < 4; i++) {
    if (td[i] < minT) minT = td[i];
    if (td[i] > maxT) maxT = td[i];
  }
  
  if ((minT > pt->boxDist) || (maxT < -pt->boxDist)) {
    return true;
  } else {
    return false;
  }  
}

static bool makePlane (const float* p1, const float* p2,
                       const float* pc, float* r);



////////////////////////////////////////////////////////////////////////////////
//
// TetraBuilding
//


const char* TetraBuilding::typeName = "TetraBuilding";
TetraBuilding::planeTest TetraBuilding::axisTests[25];


TetraBuilding::TetraBuilding(const float (*_vertices)[3], const bool *_visible,
                             const bool *_colored, const float (*_colors)[4],
			     bool drive, bool shoot)
{
  int v, a;

  memcpy (vertices, _vertices, 4 * sizeof (float[3]));
  memcpy (visible, _visible, 4 * sizeof (bool));
  memcpy (colored, _colored, 4 * sizeof (bool));
  memcpy (colors, _colors, 4 * sizeof (float[4]));

  // make sure the the planes are facing outwards
  float edge[3][3]; // edges from vertex 0
  for (v = 0; v < 3; v++) {
    for (a = 0; a < 3; a++) {
      edge[v][a] = vertices[v+1][a] - vertices[0][a];
    }
  }
  float cross[3];
  vec3cross(cross, edge[0], edge[1]);
  
  const float dot = vec3dot (cross, edge[2]);

  // swap vertices 1 & 2 if we are out of order
  if (dot < 0.0f) {
    memcpy (vertices[1], _vertices[2], sizeof(float[3]));
    memcpy (vertices[2], _vertices[1], sizeof(float[3]));
    memcpy (colors[1], _colors[2], sizeof (float[4]));
    memcpy (colors[2], _colors[1], sizeof (float[4]));
    visible[1] = _visible[2];
    visible[2] = _visible[1];
    colored[1] = _colored[2];
    colored[2] = _colored[1];
  }

  // make outward facing normals to the planes
  if (!makePlane (vertices[1], vertices[2], vertices[3], planes[0]) ||
      !makePlane (vertices[0], vertices[3], vertices[2], planes[1]) ||
      !makePlane (vertices[0], vertices[1], vertices[3], planes[2]) ||
      !makePlane (vertices[0], vertices[2], vertices[1], planes[3])) {
    // trigger isValid() to return false;
    for (v = 0; v < 4; v++) {
      for (a = 0; a < 3; a++) {
        vertices[v][a] = 0.0f;
      }
    }
  }

  // setup the extents
  mins[0] = mins[1] = mins[2] = +Infinity;
  maxs[0] = maxs[1] = maxs[2] = -Infinity;
  for (v = 0; v < 4; v++) {
    const float* vertex = vertices[v];
    for (a = 0; a < 3; a++) {
      if (vertex[a] < mins[a]) {
        mins[a] = vertex[a];
      }
      if (vertex[a] > maxs[a]) {
        maxs[a] = vertex[a];
      }
    }
  }

  // setup the basic obstacle parameters
  pos[0] = 0.5f * (maxs[0] + mins[0]);
  pos[1] = 0.5f * (maxs[1] + mins[1]);
  pos[2] = mins[2];
  size[0] = 0.5f * (maxs[0] - mins[0]);
  size[1] = 0.5f * (maxs[1] - mins[1]);
  size[2] = maxs[2] - mins[2];
  angle = 0.0f;
  driveThrough = drive;
  shootThrough = shoot;
  ZFlip = false;

  return;
}


TetraBuilding::~TetraBuilding()
{
  // do nothing
  return;
}


const char* TetraBuilding::getType() const
{
  return typeName;
}


const char* TetraBuilding::getClassName() // const
{
  return typeName;
}


bool TetraBuilding::isValid() const
{
  int v, a;

  float edge[3][3]; // edges from vertex 0
  for (v = 0; v < 3; v++) {
    for (a = 0; a < 3; a++) {
      edge[v][a] = vertices[v+1][a] - vertices[0][a];
    }
  }
  float cross[3];
  vec3cross(cross, edge[0], edge[1]);
  float dot = vec3dot (cross, edge[2]);

  if (fabsf(dot) < 0.000001) {
    return false; // tetrahedrons require a volume, at least for now
  }

  // now check the vertices
  for (v = 0; v < 4; v++) {
    for (a = 0; a < 3; a++) {
      if (fabsf(vertices[v][a]) > maxExtent) {
        return false;
      }
    }
  }

  return true;
}


void TetraBuilding::getExtents(float* _mins, float* _maxs) const
{
  memcpy (_mins, mins, sizeof(float[3]));
  memcpy (_maxs, maxs, sizeof(float[3]));
  return;
}


void TetraBuilding::getCorner(int index, float* pos) const
{
  memcpy (pos, vertices[index], 3 * sizeof(float));
  return;
}


static bool makePlane (const float* p1, const float* p2, const float* pc,
                       float* r)
{
  // make vectors from points
  float x[3], y[3], n[3];

  vec3sub (x, p1, pc);
  vec3sub (y, p2, pc);

  // cross product to get the normal
  vec3cross (n, x, y);

  // normalize
  float len = vec3dot (n, n);
  if (len < +0.000001f) {
    return false;
  } else {
    len = 1.0f / sqrtf (len);
  }
  r[0] = n[0] * len;
  r[1] = n[1] * len;
  r[2] = n[2] * len;

  // finish the plane equation: {rx*px + ry*py + rz+pz + rd = 0}
  r[3] = -vec3dot(pc, r);

  return true;
}


float TetraBuilding::intersect(const Ray& ray) const
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
    const float linedot = vec3dot(planes[p], dir);
    if (linedot >= -0.001f) {
      // shot is either parallel, or going through backwards
      times[p] = Infinity;
      continue;
    }
    const float origindot = vec3dot(planes[p], origin);
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
    float targetTime = times[target];
    if (targetTime == Infinity) {
      continue;
    }
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
      float d = vec3dot (planes[q], point) + planes[q][3];
      if (d > 0.001f) {
        gotFirstHit = false;
        break;
      }
    }
    if (gotFirstHit) {
      lastPlaneShot = target;
      return targetTime;
    }
  }

  return -1.0f;
}


void TetraBuilding::get3DNormal(const float* /*p*/, float* n) const
{
  // intersect() must be called on this obstacle
  // before this function can be used.
  memcpy (n, planes[lastPlaneShot], sizeof(float[3]));
  return;
}


bool TetraBuilding::inCylinder(const float* p,
                               float radius, float height) const
{
  // This is obviously not a real cylinder test, but this
  // particular collision test is used when we need speed.
  if (((p[0] + radius) < mins[0]) || ((p[0] - radius) > maxs[0]) ||
      ((p[1] + radius) < mins[1]) || ((p[1] - radius) > maxs[1]) ||
      ((p[2] + height) < mins[2]) || (p[2] > maxs[2])) {
    return false;
  }
  return true;
}


bool TetraBuilding::inBox(const float* p, float angle,
                          float dx, float dy, float height) const
{
//  angle = sqrtf ((dx * dx) + (dy * dy));
//  return inCylinder (p, angle, height);
  
  static const float axisNormals[3][3] = {
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f }
  };
  int v, a, tn = 0; // tn is "test number"

  // first, do a simple Z axis separation test  (one down, 24 to go)
  if (((p[2] + height) < mins[2]) || (p[2] > maxs[2])) {
    return false;
  }
  tn++;
  
  // translate both objects so that the
  // box is axis aligned and at the origin
  float newVerts[4][3];
  const float halfHeight = height / 2.0f;
  const float boxSizes[3] = { dx, dy, halfHeight };
  float cosVal = cos(-angle); // note the negative,
  float sinVal = sin(-angle); // we're reverse transforming
  for (v = 0; v < 4; v++) {
    const float sx = vertices[v][0] - p[0];
    const float sy = vertices[v][1] - p[1];
    newVerts[v][0] = (cosVal * sx) - (sinVal * sy);
    newVerts[v][1] = (cosVal * sy) + (sinVal * sx);
    newVerts[v][2] = vertices[v][2] - (p[2] + halfHeight);
  }
  
  // check the remaining 2 box plane tests (X & Y axis)
  projectTetra (axisTests[tn].tetraDists, axisNormals[0], newVerts);
  axisTests[tn].boxDist = boxSizes[0];
  if (checkTest(tn)) return false;
  tn++;

  projectTetra (axisTests[tn].tetraDists, axisNormals[1], newVerts);
  axisTests[tn].boxDist = boxSizes[1];
  if (checkTest(tn)) return false;
  tn++;

  // FIXME - could use the cosVal and sinVal values to
  //         rotate the current planes and save some time
  //         could also get these distances before rotating
  //         if the distances are adjusted for the box position.
  // remake the tetrahedron planes in their new positions
  float newNorms[4][3];
  makeNormal (newVerts[1], newVerts[2], newVerts[3], newNorms[0]);
  makeNormal (newVerts[0], newVerts[3], newVerts[2], newNorms[1]);
  makeNormal (newVerts[0], newVerts[1], newVerts[3], newNorms[2]);
  makeNormal (newVerts[0], newVerts[2], newVerts[1], newNorms[3]);
  
  // check the 4 tetrahedron plane tests
  projectOrigBox (&axisTests[tn].boxDist, newNorms[0], boxSizes);
  projectTetra (axisTests[tn].tetraDists, newNorms[0], newVerts);
  if (checkTest(tn)) return false;
  tn++;

  projectOrigBox (&axisTests[tn].boxDist, newNorms[1], boxSizes);
  projectTetra (axisTests[tn].tetraDists, newNorms[1], newVerts);
  if (checkTest(tn)) return false;
  tn++;

  projectOrigBox (&axisTests[tn].boxDist, newNorms[2], boxSizes);
  projectTetra (axisTests[tn].tetraDists, newNorms[2], newVerts);
  if (checkTest(tn)) return false;
  tn++;

  projectOrigBox (&axisTests[tn].boxDist, newNorms[3], boxSizes);
  projectTetra (axisTests[tn].tetraDists, newNorms[3], newVerts);
  if (checkTest(tn)) return false;
  tn++;

  // check for edge collisions  (3E * 6E = 18 expensive tests)
  float testNorm[3];
  for (a = 0; a < 3; a++) {
    
    vec3sub(testNorm, newVerts[0], newVerts[1]);
    vec3cross(axisTests[tn].normal, axisNormals[a], testNorm);
    projectOrigBox (&axisTests[tn].boxDist, axisTests[tn].normal, boxSizes);
    projectTetra (axisTests[tn].tetraDists, axisTests[tn].normal, newVerts);
    if (checkTest(tn)) return false;
    tn++;

    vec3sub(testNorm, newVerts[0], newVerts[2]);
    vec3cross(axisTests[tn].normal, axisNormals[a], testNorm);
    projectOrigBox (&axisTests[tn].boxDist, axisTests[tn].normal, boxSizes);
    projectTetra (axisTests[tn].tetraDists, axisTests[tn].normal, newVerts);
    if (checkTest(tn)) return false;
    tn++;
        
    vec3sub(testNorm, newVerts[0], newVerts[3]);
    vec3cross(axisTests[tn].normal, axisNormals[a], testNorm);
    projectOrigBox (&axisTests[tn].boxDist, axisTests[tn].normal, boxSizes);
    projectTetra (axisTests[tn].tetraDists, axisTests[tn].normal, newVerts);
    if (checkTest(tn)) return false;
    tn++;
        
    vec3sub(testNorm, newVerts[1], newVerts[2]);
    vec3cross(axisTests[tn].normal, axisNormals[a], testNorm);
    projectOrigBox (&axisTests[tn].boxDist, axisTests[tn].normal, boxSizes);
    projectTetra (axisTests[tn].tetraDists, axisTests[tn].normal, newVerts);
    if (checkTest(tn)) return false;
    tn++;

    vec3sub(testNorm, newVerts[2], newVerts[3]);
    vec3cross(axisTests[tn].normal, axisNormals[a], testNorm);
    projectOrigBox (&axisTests[tn].boxDist, axisTests[tn].normal, boxSizes);
    projectTetra (axisTests[tn].tetraDists, axisTests[tn].normal, newVerts);
    if (checkTest(tn)) return false;
    tn++;
        
    vec3sub(testNorm, newVerts[3], newVerts[1]);
    vec3cross(axisTests[tn].normal, axisNormals[a], testNorm);
    projectOrigBox (&axisTests[tn].boxDist, axisTests[tn].normal, boxSizes);
    projectTetra (axisTests[tn].tetraDists, axisTests[tn].normal, newVerts);
    if (checkTest(tn)) return false;
    tn++;
  }
      
  return true;  
}


bool TetraBuilding::inMovingBox(const float* oldP, float /*oldAngle*/,
                                const float* p, float angle,
                                float dx, float dy, float height) const
{
  float fakeHeight;
  float fakePos[3];
  if (p[2] < oldP[2]) {
    fakePos[2] = p[2];
    fakeHeight = height + (oldP[2] - p[2]);
  } else {
    fakePos[2] = oldP[2];
    fakeHeight = height + (p[2] - oldP[2]);
  }
  fakePos[1] = p[1];
  fakePos[2] = p[2];
  return inBox (fakePos, angle, dx, dy, fakeHeight);
}


/////////////////////////////////////////////////////////////
//  FIXME - everything after this point is currently JUNK! //
/////////////////////////////////////////////////////////////


void TetraBuilding::getNormal(const float* p, float* n) const
{
  p = p;
  n[0] = 0.0f;
  n[1] = 0.0f;
  n[2] = +1.0f;
}


bool TetraBuilding::getHitNormal(const float* pos1, float,
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


// This is only used when the player has an Oscillation Overthruster
// flag, and only after we already know that the tank is interfering
// with this tetrahedron, so it doesn't have to be particularly fast.
// As a note, some of the info from the original collision test might
// be handy here.
bool TetraBuilding::isCrossing(const float* p, float angle,
                               float dx, float dy, float height,
                               float* plane) const
{
  float corner[8][3]; // the box corners
  const float cosval = cos(angle);
  const float sinval = sin(angle);
  int bv, tp; // box vertices, tetra planes

  // make the box vertices
  corner[0][0] = (cosval * dx) - (sinval * dy);
  corner[1][0] = (cosval * dx) + (sinval * dy);
  corner[0][1] = (cosval * dy) + (sinval * dx);
  corner[1][1] = (cosval * dy) - (sinval * dx);
  for (bv = 0; bv < 2; bv++) {
    corner[bv + 2][0] = -corner[bv][0];
    corner[bv + 2][1] = -corner[bv][1];
  }
  for (bv = 0; bv < 4; bv++) {
    corner[bv][0] = p[0] + corner[bv][0];
    corner[bv][1] = p[1] + corner[bv][1];
    corner[bv][2] = p[2];
    corner[bv + 4][0] = corner[bv][0];
    corner[bv + 4][1] = corner[bv][1];
    corner[bv + 4][2] = corner[bv][2] + height;
  }

  // see if any tetra planes separate the box vertices
  bool done = false;
  for (tp = 0; tp < 4; tp++) {
    int splitdir = 0;
    for (bv = 0; bv < 8; bv++) {
      const float d = vec3dot(corner[bv], planes[tp]) + planes[tp][3];
      int newdir = (d > 0.0f) ? +1 : -1;

      if (bv == 0) {
        splitdir = newdir;
      }
      else if (splitdir != newdir) {
        done = true;
        break;
      }
    }
    if (done) {
      break;
    }
  }

  // copy the plane information if requested
  if (tp < 4) {
    if (plane != NULL) {
      memcpy (plane, planes[tp], sizeof(float[4]));
    }
    return true;
  }

  return false;
}


void *TetraBuilding::pack(void* buf)
{
  unsigned char planeflags = 0;   // 0-3 are visibility, 4-7 are colored
  unsigned char bytecolors[4][4]; // pack the colors into a 32bit format
  int v, c;
  
  // setup the planeflags and bytecolors bytes
  for (v = 0; v < 4; v++) {
    if (isVisiblePlane(v)) {
      planeflags = planeflags | (1 << v);
    }
    if (isColoredPlane(v)) {
      planeflags = planeflags | (1 << (v + 4));
    }
    for (c = 0; c < 4; c++) {
      bytecolors[v][c] = (unsigned char) colors[v][c];
    }
  }
  
  for (v = 0; v < 4; v++) {
    buf = nboPackVector(buf, vertices[v]);
  }
  for (v = 0; v < 4; v++) {
    for (c = 0; c < 4; c++) {
      buf = nboPackUByte(buf, bytecolors[v][c]);
    }
  }

  buf = nboPackUByte(buf, planeflags);
  
  unsigned char bitMask = 0;
  if (isDriveThrough())
    bitMask |= _DRIVE_THRU;
  if (isShootThrough())
    bitMask |= _SHOOT_THRU;
    
  buf = nboPackUByte(buf, bitMask);
  
  return buf;
}


void *TetraBuilding::unpack(void* buf)
{
  buf = buf;
  return buf;
}


int TetraBuilding::packSize()
{
  return 0;
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8

