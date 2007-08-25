/* bzflag
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#define CELLROAD  1
#define CELLROADX 2
#define CELLBASE  3
#define MATROAD   0
#define MATROADX  1
#define MATWALL   2
#define MATWALL2  3
#define MATMESH   4
#define MATROOF   5
#define MATROOFT  6
#define MATGLASS  7
#define MATGRASS  8
#define MATDOORW  9
#define MATDOORR  10
#define MAXMATERIALS 11

#define EPSILON 0.00000001f
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#include <vector>
#include <math.h>
#include <string>
#include <map>
#include "commandArgs.h"

extern int debugLevel;
extern std::string texturepath;

struct Vertex {
  float x, y, z;
 
  Vertex() : x(0.0f), y(0.0f), z(0.0f) {}
  Vertex(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
  Vertex(float v[3]) :x(v[0]), y(v[1]), z(v[2]) {}
  
  void add(const Vertex &v) { x += v.x; y += v.y; z += v.z; }
  Vertex operator+(const Vertex &v) { return Vertex(x + v.x, y + v.y, z + v.z); }

  void sub(const Vertex &v) { x -= v.x; y -= v.y; z -= v.z; }
  Vertex operator-(const Vertex &v) { return Vertex(x - v.x, y - v.y, z - v.z); }

  void mult(const Vertex &v) { x *= v.x; y *= v.y; z *= v.z; }
  Vertex operator*(const Vertex &v) { return Vertex(x * v.x, y * v.y, z * v.z); }

  void mult(const float f) { x *= f; y *= f; z *= f; }
  Vertex operator*(const float f) { return Vertex(x * f, y * f, z * f); }

  void div(const Vertex &v) { x /= v.x; y /= v.y; z /= v.z; }
  Vertex operator/(const Vertex &v) { return Vertex(x / v.x, y / v.y, z / v.z); }

  void div(const float f) { x /= f; y /= f; z /= f; }
  Vertex operator/(const float f) { return Vertex(x / f, y / f, z / f); }

  void normalize() { float l = length(); if (l == 0.0f) return; x/=l; y/=l; z/=l; }
  Vertex norm() { float l = length(); if (l == 0.0f) return Vertex(); return Vertex(x/l,y/l,z/l); }

  std::string toString() { char buffer[80]; sprintf(buffer, "[%.2f,%.2f,%.2f]",x,y,z); return std::string(buffer); }

  Vertex cross(const Vertex &v) { 
    return Vertex(
      y * v.z - v.y * z,
      v.x * z - x * v.z,
      x * v.y - v.x * y
    );
  }

  float dot(const Vertex &v) { return x*v.x + y*v.y + z*v.z; }

  float length() { return sqrtf(x*x + y*y + z*z); }
  float lengthsq() { return x*x + y*y + z*z; }

  void set(float v[3]) { x = v[0], y = v[1], z = v[2]; }
  void set(float _x, float _y, float _z) { x = _x, y = _y, z = _z; }
  
  float &operator[](int i) {
    switch (i) {
      case 0: return x; break;
      case 1: return y; break;
      case 2: return z; break;
      default: return x;
    }
  }
};

typedef std::vector<Vertex> VertexVector;
typedef VertexVector::iterator VertexVectIter;

struct TexCoord {
  float s, t;
 
  TexCoord() : s(0.0f), t(0.0f) {}
  TexCoord(float _s, float _t) :s(_s), t(_t) {}
  TexCoord(float v[2]) : s(v[0]), t(v[1]) {}
  
  void set(float v[2]) { s = v[0], t = v[1]; }
  void set(float _s, float _t) { s = _s, t = _t; }

  // This may be evil, but TexCoords do not need to be fully perfect :P
  bool operator==(const TexCoord& b) {
    return (fabs(s - b.s) < EPSILON && fabs(t - b.t) < EPSILON);
  }

  float &operator[](int i) {
    switch (i) { 
      case 0: return s; break;
      case 1: return t; break;
      default: return s;
    }
  }
};

typedef std::vector<TexCoord> TexCoordVector;
typedef TexCoordVector::iterator TexCoordVectIter;

struct ID4 {
  int a, b, c, d;
 
  ID4() : a(0), b(0), c(0), d(0) {}
  ID4(int _a, int _b, int _c, int _d) : a(_a), b(_b), c(_c), d(_d) {}
  ID4(int v[4]) : a(v[0]), b(v[1]), c(v[2]), d(v[3]) {}
  
  void set(int v[4]) { a = v[0], b = v[1], c = v[2], d = v[3]; }
  void set(int _a, int _b, int _c, int _d) { a = _a, b = _b, c = _c, d = _d; }
  
  int &operator[](int i) {
    switch (i) { 
      case 0: return a; break;
      case 1: return b; break;
      case 2: return c; break;
      case 3: return d; break;
      default: return a;
    }
  }
};

typedef std::vector<int> IntVector;
typedef std::vector<std::string> StringVector;
typedef std::vector<bool> BoolVector;
typedef std::map<std::string,float> AttributeMap;

struct DiscreetMapNode {
  int type;
  int zone;
};

struct GridInfo {
  int size;
  int sizeX,sizeY;
  int stepX,stepY;
};

struct Coord2D {
  int x;
  int y;
  Coord2D() { x=0; y=0; };
  Coord2D(int ax, int ay) : x(ax), y(ay) {};
  const Coord2D& operator += (const Coord2D& b) {
    x += b.x;
    y += b.y;
    return *this;
  }
  const Coord2D& operator -= (const Coord2D& b) {
    x -= b.x;
    y -= b.y;
    return *this;
  }	
  bool operator==(const Coord2D& b) {
    return (x == b.x && y == b.y);
  }
  bool operator != (const Coord2D& b) { return !operator==(b); }
};

inline int modprev(int x, int mod) { if (x == 0) return mod-1; else return x-1; }
inline int modnext(int x, int mod) { if (x == mod-1) return 0; else return x+1; }

inline int randomInt01() { return rand()%2; }
inline int randomInt(int range) { if (range == 0) return 0; return rand()%range; }
inline int randomIntRange(int min, int max) { return randomInt(max-min)+min; }
inline int randomIntRangeStep(int min, int max, int step) { if (step == 0) return 0; int steps = int((max-min) / step);  return randomInt(steps+1)*step+min; }
inline bool randomBool() { return rand()%2 == 0; }
inline bool randomChance(int chance) { return randomInt(100) < chance; }
inline float randomFloat01() { return (float)(rand()) / (float)(RAND_MAX); }
inline float randomFloat(float range) { return randomFloat01()*range; }
inline float randomFloatRange(float min, float max) { return randomFloat(max-min)+min; }
inline float randomFloatRangeStep(float min, float max, float step) { if (step == 0) return 0.0; int steps = int((max-min) / step); return randomInt(steps+1)*step + min; }
inline int round(float f) { return int(f+0.5f); }

inline float fsign(float f) { if (f == 0.0f) return 0.0f; if (f < 0.0f) return -1.0f; else return 1.0f; }

inline float snap(float f,float snapval) { return float(round(f/snapval))*snapval; }
inline float refinesnap(float oldsnap, float max) { return (max/float(round(max/oldsnap))); }

inline float minf(float a,float b) { if (a < b) return a; else return b; }
inline float maxf(float a,float b) { if (a > b) return a; else return b; }

inline bool iszero(float f) { return fabs(f) < EPSILON; }

inline bool inrange(float a, float r1, float r2) { 
  if (r2 > r1) return a > r1-EPSILON && a < r2+EPSILON; 
          else return a > r2-EPSILON && a < r1+EPSILON; 
}

inline bool commonrange(float a1, float a2, float b1, float b2, float &na2) { 
  float mina = minf(a1,a2);
  float maxa = maxf(a1,a2);
  if (maxa < b1+EPSILON && maxa < b2+EPSILON) return false;
  if (mina > b1-EPSILON && mina > b2-EPSILON) return false;
  if (inrange(a2,b1,b2)) {
    na2 = a2; 
  } else {
    if (a2 > a1) na2 = max(b1,b2); else na2 = min(b1,b2);
  }
  return true;
}

inline bool intersectZ(Vertex A, Vertex B, Vertex C, Vertex D, Vertex& P) {
  float d = ((B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x));
  float r = ((A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y));
  float s = ((A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y));
  if (iszero(d)) {
    if (iszero(r)) { // parallel and coincident
      float nb;
      if (iszero(A.x-C.x)) {                   // parallel on X
        if (commonrange(A.x,B.x,C.x,D.x,nb)) { // AB and CD have common point
          P = B;
          P.x = nb;
          return true;
        } else return false;
      } else {                                 // parallel on Y
        if (commonrange(A.y,B.y,C.y,D.y,nb)) { // AB and CD have common point
          P = B;
          P.y = nb;
          return true;
        } else return false;
      }
    } else return false; // parallel but not coincident
  }
  r = r/d;
  s = s/d;
  if (r > 0-EPSILON && r < 1+EPSILON && s > 0-EPSILON && s < 1+EPSILON) {
    P.x = A.x+r*(B.x-A.x);
    P.y = A.y+r*(B.y-A.y);
    P.z = A.z;
    return true;
  } else return false;
}
inline bool samepointZ(Vertex A, Vertex B) {
  return (fabs(A.x-B.x) < EPSILON && fabs(A.y-B.y) < EPSILON);
}

#endif /* __COMMON_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
