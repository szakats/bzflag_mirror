/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
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
#include <iostream>
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

typedef std::vector<Vertex> VertexVector;
typedef VertexVector::iterator VertexVectIter;

typedef std::vector<TexCoord> TexCoordVector;
typedef TexCoordVector::iterator TexCoordVectIter;

#endif /* __GLOBALS_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
