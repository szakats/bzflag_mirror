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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <vector>
#include <string>
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>

typedef std::string String;
typedef std::vector<int> IntVector;
typedef std::vector<std::string> StringVector;
typedef std::vector<bool> BoolVector;
typedef std::vector<double> DoubleVector;
typedef std::map<std::string,double> AttributeMap;
typedef std::ostream OutStream;
typedef std::ofstream OutFileStream;

#define BZWGMajorVersion 0
#define BZWGMinorVersion 1
#define BZWGRevision     3
#define BZWGBuildState "development"

#define CELLROAD  1
#define CELLROADX 2
#define CELLBASE  3

#define EPSILON 0.001
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#ifdef _USE_GNU_DELIMS
#define argumentDeliminator "--"
#else
#define argumentDeliminator "-"
#endif //_USE_GNU_DELIMS


extern int debugLevel;

#include "Vector2D.h"
#include "Vector3D.h"

typedef Vector3D<double> Vertex;
typedef Vector2D<double> TexCoord;
typedef Vector2D<int> Coord2D;

typedef std::vector<Vertex> VertexVector;
typedef VertexVector::iterator VertexVectIter;

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

inline int modprev(int x, int mod) { return (x == 0) ? mod-1 : x-1; }
inline int modnext(int x, int mod) { return (x == mod-1) ? 0 : x+1; }

inline int roundToInt(double f) { return int(f+0.5f); }

inline double fsign(double f) { if (f == 0.0) return 0.0; return (f < 0.0) ? -1.0 : 1.0; }

inline double snap(double f,double snapval) { return double(roundToInt(f/snapval))*snapval; }
inline double refinesnap(double oldsnap, double max) { return (max/double(roundToInt(max/oldsnap))); }

inline double minf(double a,double b) { return a < b ? a : b; }
inline double maxf(double a,double b) { return a > b ? a : b; }

inline bool iszero(double f) { return fabs(f) < EPSILON; }

inline bool inrange(double a, double r1, double r2) { 
  return (r2 > r1) ? (a > r1-EPSILON && a < r2+EPSILON) : (a > r2-EPSILON && a < r1+EPSILON); 
}

inline bool commonrange(double a1, double a2, double b1, double b2, double &c1, double &c2) { 
  double mina = minf(a1,a2);
  double maxa = maxf(a1,a2);
  double minb = minf(b1,b2);
  double maxb = maxf(b1,b2);
  if (maxa < minb-EPSILON || mina > maxb+EPSILON) return false;
  c1 = maxf(mina,minb);
  c2 = minf(maxa,maxb);
  return true;
}

inline int intersectZ(Vertex A, Vertex B, Vertex C, Vertex D, Vertex& P1, Vertex& P2) {
  double d = ((B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x));
  double r = ((A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y));
  double s = ((A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y));
  if (iszero(d)) {
    if (iszero(r)) { // parallel and coincident
      double e;
      double f;
      if (iszero(A.y-C.y)&&iszero(B.y-D.y)) {                   // parallel on X
        if (commonrange(A.x,B.x,C.x,D.x,e,f)) { // AB and CD have common point
          P1 = P2 = A;
          P1.x = e;
          P2.x = f;
          return iszero(e-f) ? 1 : 2;
        } else return 0;
      } else {                                 // parallel on Y
        if (commonrange(A.y,B.y,C.y,D.y,e,f)) { // AB and CD have common point
          P1 = P2 = A;
          P1.y = e;
          P2.y = f;
          return iszero(e-f) ? 1 : 2;
        } else return 0;
      }
    } else return 0; // parallel but not coincident
  }
  r /= d;
  s /= d;
  if (r > 0-EPSILON && r < 1+EPSILON && s > 0-EPSILON && s < 1+EPSILON) {
    P1.x = A.x+r*(B.x-A.x);
    P1.y = A.y+r*(B.y-A.y);
    P1.z = A.z;
    return 1;
  } else return 0;
}

inline bool samepointZ(Vertex A, Vertex B) {
  return (fabs(A.x-B.x) < EPSILON && fabs(A.y-B.y) < EPSILON);
}

inline bool sameTexCoord(TexCoord A, TexCoord B) {
  return (fabs(A.x-B.x) < EPSILON && fabs(A.y-B.y) < EPSILON);
}

#endif /* __GLOBALS_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
