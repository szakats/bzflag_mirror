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
#include "MathUtils.h"

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

#endif /* __GLOBALS_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
