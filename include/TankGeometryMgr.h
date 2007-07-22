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

/* TankGeometryMgr:
 *	Generates the display lists for TankSceneNodes
 */

#ifndef	BZF_TANK_GEOMETRY_MGR_H
#define	BZF_TANK_GEOMETRY_MGR_H

#include "common.h"
#include "SceneNode.h"


namespace TankGeometryEnums {

  enum TankShadow {
    ShadowOff = 0,
    ShadowOn,
    LastTankShadow
  };

  enum TankLOD {
    LowTankLOD = 0,
    MedTankLOD,
    HighTankLOD,
    LastTankLOD
  };

  enum TankSize {
    Normal = 0,
    Obese,
    Tiny,
    Narrow,
    Thief,
    LastTankSize
  };

  enum TankPart {
    Body = 0,
    Barrel,
    Turret,
    LeftCasing,
    RightCasing,

    LeftTread, // animated parts
    RightTread,
    LeftWheel0,
    LeftWheel1,
    LeftWheel2,
    LeftWheel3,
    RightWheel0,
    RightWheel1,
    RightWheel2,
    RightWheel3,

    LastTankPart,
    BasicTankParts = LeftTread,
    LowTankParts = LeftTread,
    MedTankParts = LeftTread,
    HighTankParts = LastTankPart
  };
}


namespace TankGeometryMgr {

  void init();
  void kill();
  void buildLists();
  void deleteLists();

  GLuint getPartList(TankGeometryEnums::TankShadow shadow,
		     TankGeometryEnums::TankPart part,
		     TankGeometryEnums::TankSize size,
		     TankGeometryEnums::TankLOD lod);

  int getPartTriangleCount(TankGeometryEnums::TankShadow shadow,
			   TankGeometryEnums::TankPart part,
			   TankGeometryEnums::TankSize size,
			   TankGeometryEnums::TankLOD lod);

  const float* getScaleFactor(TankGeometryEnums::TankSize size);
}


namespace TankGeometryUtils {

  enum TreadStyle {
    Covered = 0,
    Exposed = 1
  };
  void setTreadStyle(int style);

  // degrees / meter
  float getWheelScale();
  // texcoords / meter
  float getTreadScale();
  // texcoords
  float getTreadTexLen();

  // help to scale vertices and normals
  inline void doVertex3f(GLfloat x, GLfloat y, GLfloat z);
  inline void doNormal3f(GLfloat x, GLfloat y, GLfloat z);
  inline void doTexCoord2f(GLfloat x, GLfloat y);

  //
  // NOTE:  these all return their triangle count
  //

  // lowtank geometry builder
  int buildLowBody (void);
  int buildLowTurret (void);
  int buildLowLCasing (void);
  int buildLowRCasing (void);
  int buildLowBarrel (void);

  // medtank geometry builder
  int buildMedBody (void);
  int buildMedTurret (void);
  int buildMedLCasing (void);
  int buildMedRCasing (void);
  int buildMedBarrel (void);

  // hightank geometry builder
  int buildHighBody (void);
  int buildHighBarrel (void);
  int buildHighTurret (void);
  int buildHighLCasing (void);
  int buildHighRCasing (void);

  // animated geometry builder
  int buildHighLCasingAnim (void);
  int buildHighRCasingAnim (void);
  int buildHighLTread (int divs);
  int buildHighRTread (int divs);
  int buildHighLWheel (int wheel, float angle, int divs);
  int buildHighRWheel (int wheel, float angle, int divs);

  extern const float* currentScaleFactor;
  extern TankGeometryEnums::TankShadow shadowMode;
}



// TankGeometryUtils Functions
// ---------------------------

inline 
void TankGeometryUtils::doVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
  const float* scale = currentScaleFactor;
  x = x * scale[0];
  y = y * scale[1];
  z = z * scale[2];
  glVertex3f(x, y, z);
  return;
}


inline
void TankGeometryUtils::doNormal3f(GLfloat x, GLfloat y, GLfloat z)
{
  if (shadowMode == TankGeometryEnums::ShadowOn) {
    return;
  }
  const float* scale = currentScaleFactor;
  GLfloat sx = x * scale[0];
  GLfloat sy = y * scale[1];
  GLfloat sz = z * scale[2];
  const GLfloat d = sqrtf ((sx * sx) + (sy * sy) + (sz * sz));
  if (d > 1.0e-5f) {
    x *= scale[0] / d;
    y *= scale[1] / d;
    z *= scale[2] / d;
  }
  glNormal3f(x, y, z);
  return;
}


inline
void TankGeometryUtils::doTexCoord2f(GLfloat x, GLfloat y)
{
  if (shadowMode == TankGeometryEnums::ShadowOn) {
    return;
  }
  glTexCoord2f(x, y);
  return;
}


#endif // BZF_TANK_GEOMETRY_MGR_H


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
