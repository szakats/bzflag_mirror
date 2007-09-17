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

#define EPSILON 0.001
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
  double x, y, z;
 
  Vertex() : x(0.0), y(0.0), z(0.0) {}
  Vertex(double _x, double _y, double _z) :x(_x), y(_y), z(_z) {}
  Vertex(double v[3]) :x(v[0]), y(v[1]), z(v[2]) {}
  
  void add(const Vertex &v) { x += v.x; y += v.y; z += v.z; }
  Vertex operator+(const Vertex &v) { return Vertex(x + v.x, y + v.y, z + v.z); }

  void sub(const Vertex &v) { x -= v.x; y -= v.y; z -= v.z; }
  Vertex operator-(const Vertex &v) { return Vertex(x - v.x, y - v.y, z - v.z); }

  void mult(const Vertex &v) { x *= v.x; y *= v.y; z *= v.z; }
  Vertex operator*(const Vertex &v) { return Vertex(x * v.x, y * v.y, z * v.z); }

  void mult(const double f) { x *= f; y *= f; z *= f; }
  Vertex operator*(const double f) { return Vertex(x * f, y * f, z * f); }

  void div(const Vertex &v) { x /= v.x; y /= v.y; z /= v.z; }
  Vertex operator/(const Vertex &v) { return Vertex(x / v.x, y / v.y, z / v.z); }

  void div(const double f) { x /= f; y /= f; z /= f; }
  Vertex operator/(const double f) { return Vertex(x / f, y / f, z / f); }

  void normalize() { double l = length(); if (l == 0.0) return; x/=l; y/=l; z/=l; }
  Vertex norm() { double l = length(); return (l == 0.0) ? Vertex() : Vertex(x/l,y/l,z/l); }

  std::string toString() { char buffer[80]; sprintf(buffer, "[%.4f,%.4f,%.4f]",x,y,z); return std::string(buffer); }

  Vertex cross(const Vertex &v) { 
    return Vertex(
      y * v.z - v.y * z,
      v.x * z - x * v.z,
      x * v.y - v.x * y
    );
  }

  double dot(const Vertex &v) { return x*v.x + y*v.y + z*v.z; }

  double length() { return sqrt(x*x + y*y + z*z); }
  double lengthsq() { return x*x + y*y + z*z; }

  void set(double v[3]) { x = v[0], y = v[1], z = v[2]; }
  void set(double _x, double _y, double _z) { x = _x, y = _y, z = _z; }
  
  double &operator[](int i) {
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
  double s, t;
 
  TexCoord() : s(0.0), t(0.0) {}
  TexCoord(double _s, double _t) :s(_s), t(_t) {}
  TexCoord(double v[2]) : s(v[0]), t(v[1]) {}
  
  void set(double v[2]) { s = v[0], t = v[1]; }
  void set(double _s, double _t) { s = _s, t = _t; }

  // This may be evil, but TexCoords do not need to be fully perfect :P
  bool operator==(const TexCoord& b) {
    return (fabs(s - b.s) < EPSILON && fabs(t - b.t) < EPSILON);
  }

  double &operator[](int i) {
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
typedef std::vector<double> DoubleVector;
typedef std::map<std::string,double> AttributeMap;

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

inline int modprev(int x, int mod) { return (x == 0) ? mod-1 : x-1; }
inline int modnext(int x, int mod) { return (x == mod-1) ? 0 : x+1; }

inline int randomInt01() { return rand()%2; }
inline int randomInt(int range) { return (range == 0) ? 0 : rand()%range; }
inline int randomIntRange(int min, int max) { return randomInt(max-min)+min; }
inline int randomIntRangeStep(int min, int max, int step) { if (step == 0) return 0; int steps = int((max-min) / step);  return randomInt(steps+1)*step+min; }
inline bool randomBool() { return rand()%2 == 0; }
inline bool randomChance(int chance) { return randomInt(100) < chance; }
inline double randomdouble01() { return (double)(rand()) / (double)(RAND_MAX); }
inline double randomdouble(double range) { return randomdouble01()*range; }
inline double randomdoubleRange(double min, double max) { return randomdouble(max-min)+min; }
inline double randomdoubleRangeStep(double min, double max, double step) { if (step == 0) return 0.0; int steps = int((max-min) / step); return randomInt(steps+1)*step + min; }
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

#endif /* __COMMON_H__ */

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
