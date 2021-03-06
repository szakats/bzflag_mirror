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

#include "common.h"
#include <math.h>
#include "global.h"
#include "Pack.h"
#include "BoxBuilding.h"
#include "Intersect.h"
#include "MeshTransform.h"


const char* BoxBuilding::typeName = "BoxBuilding";


BoxBuilding::BoxBuilding() : Obstacle(), noNodes(false)
{
}


BoxBuilding::BoxBuilding(const fvec3& p, float a, float w, float b, float h,
                         unsigned char drive, unsigned char shoot, bool rico,
                         bool invisible)
: Obstacle(p, a, w, b, h, drive, shoot, rico)
, noNodes(invisible)
{
  finalize();
  return;
}


BoxBuilding::~BoxBuilding()
{
  // do nothing
}


void BoxBuilding::finalize()
{
  Obstacle::setExtents();
  return;
}


Obstacle* BoxBuilding::copyWithTransform(const MeshTransform& xform) const
{
  fvec3 newPos = pos;
  fvec3 newSize = size;
  float newAngle = angle;

  MeshTransform::Tool tool(xform);
  bool flipped;
  tool.modifyOldStyle(newPos, newSize, newAngle, flipped);

  BoxBuilding* copy =
    new BoxBuilding(newPos, newAngle, newSize[0], newSize[1], newSize[2],
                    driveThrough, shootThrough, ricochet, noNodes);
  return copy;
}


const char* BoxBuilding::getType() const
{
  return typeName;
}


const char* BoxBuilding::getClassName() // const
{
  return typeName;
}


float BoxBuilding::intersect(const Ray& r) const
{
  return Intersect::timeRayHitsBlock(r, getPosition(), getRotation(),
                                     getWidth(), getBreadth(), getHeight());
}


void BoxBuilding::getNormal(const fvec3& p, fvec3& n) const
{
  Intersect::getNormalRect(p, getPosition(), getRotation(), getWidth(), getBreadth(), n);
}


void BoxBuilding::get3DNormal(const fvec3& p, fvec3& n) const
{
  // This bit of cruft causes bullets to bounce of buildings in the z direction
  if (fabs(p[2] - getPosition()[2]) < Epsilon) {
    n[0] = 0.0f;
    n[1] = 0.0f;
    n[2] = -1.0f;
  }
  else if (fabs(p[2] - (getPosition()[2] + getHeight())) < Epsilon) {
    n[0] = 0.0f;
    n[1] = 0.0f;
    n[2] = 1.0f;
  } // end cruftiness
  else {
    getNormal(p, n);
  }
}


bool BoxBuilding::inCylinder(const fvec3& p, float radius, float height) const
{
  return (p[2] < (getPosition()[2] + getHeight()))
  && ((p[2]+height) >= getPosition()[2])
  &&     Intersect::testRectCircle(getPosition(), getRotation(),
                                   getWidth(), getBreadth(), p, radius);
}


bool BoxBuilding::inBox(const fvec3& p, float a,
                        float dx, float dy, float height) const
{
  return (p[2] < (getPosition()[2] + getHeight()))
  &&     ((p[2]+height) >= getPosition()[2])
  &&     Intersect::testRectRect(getPosition(), getRotation(),
                                 getWidth(), getBreadth(), p, a, dx, dy);
}


bool BoxBuilding::inMovingBox(const fvec3& oldP, float,
                              const fvec3& p, float a,
                              float dx, float dy, float height) const
{
  float higherZ;
  float lowerZ;
  if (oldP[2] > p[2]) {
    higherZ = oldP[2];
    lowerZ  = p[2];
  } else {
    higherZ = p[2];
    lowerZ  = oldP[2];
  }
  if (lowerZ >= (getPosition()[2] + getHeight()))
    return false;
  if ((higherZ + height) < getPosition()[2])
    return false;
  return Intersect::testRectRect(getPosition(), getRotation(), getWidth(), getBreadth(),
		                 p, a, dx, dy);
}


bool BoxBuilding::isCrossing(const fvec3& p, float a,
                             float dx, float dy, float height, fvec4* planePtr) const
{
  // if not inside or contained then not crossing
  if (!inBox(p, a, dx, dy, height) ||
	Intersect::testRectInRect(getPosition(), getRotation(),
	                          getWidth(), getBreadth(), p, a, dx, dy)) {
    return false;
  }
  if (!planePtr) {
    return true;
  }
  fvec4& plane = *planePtr;

  // it's crossing -- choose which wall is being crossed (this
  // is a guestimate, should really do a careful test).  just
  // see which wall the point is closest to.
  const fvec3& p2 = getPosition();
  const float a2 = getRotation();
  const float c = cosf(-a2), s = sinf(-a2);
  const float x = c * (p[0] - p2[0]) - s * (p[1] - p2[1]);
  const float y = c * (p[1] - p2[1]) + s * (p[0] - p2[0]);
  float pw[2];
  if (fabsf(fabsf(x) - getWidth()) < fabsf(fabsf(y) - getBreadth())) {
    plane[0] = ((x < 0.0) ? -cosf(a2) : cosf(a2));
    plane[1] = ((x < 0.0) ? -sinf(a2) : sinf(a2));
    pw[0] = p2[0] + getWidth() * plane[0];
    pw[1] = p2[1] + getWidth() * plane[1];
  }
  else {
    plane[0] = ((y < 0.0) ? sinf(a2) : -sinf(a2));
    plane[1] = ((y < 0.0) ? -cosf(a2) : cosf(a2));
    pw[0] = p2[0] + getBreadth() * plane[0];
    pw[1] = p2[1] + getBreadth() * plane[1];
  }

  // now finish off plane equation
  plane[2] = 0.0;
  plane[3] = -(plane[0] * pw[0] + plane[1] * pw[1]);
  return true;
}


bool BoxBuilding::getHitNormal(const fvec3& pos1, float azimuth1,
                               const fvec3& pos2, float azimuth2,
                               float width, float breadth, float,
                               fvec3& normal) const
{
  return Obstacle::getHitNormal(pos1, azimuth1, pos2, azimuth2, width, breadth,
			getPosition(), getRotation(), getWidth(), getBreadth(),
			getHeight(), normal) >= 0.0f;
}


void BoxBuilding::getCorner(int index, fvec3& _pos) const
{
  const fvec3& base = getPosition();
  const float c = cosf(getRotation());
  const float s = sinf(getRotation());
  const float w = getWidth();
  const float h = getBreadth();
  switch (index & 3) {
    case 0:
      _pos[0] = base[0] + c * w - s * h;
      _pos[1] = base[1] + s * w + c * h;
      break;
    case 1:
      _pos[0] = base[0] - c * w - s * h;
      _pos[1] = base[1] - s * w + c * h;
      break;
    case 2:
      _pos[0] = base[0] - c * w + s * h;
      _pos[1] = base[1] - s * w - c * h;
      break;
    case 3:
      _pos[0] = base[0] + c * w + s * h;
      _pos[1] = base[1] + s * w - c * h;
      break;
  }
  _pos[2] = base[2];
  if (index >= 4)
    _pos[2] += getHeight();
}


bool BoxBuilding::isFlatTop() const
{
  return true;
}


void* BoxBuilding::pack(void* buf) const
{
  buf = nboPackFVec3(buf, pos);
  buf = nboPackFloat(buf, angle);
  buf = nboPackFVec3(buf, size);

  unsigned char stateByte = 0;
  stateByte |= isDriveThrough() ? _DRIVE_THRU : 0;
  stateByte |= isShootThrough() ? _SHOOT_THRU : 0;
  stateByte |= canRicochet()    ? _RICOCHET   : 0;
  buf = nboPackUInt8(buf, stateByte);

  return buf;
}


void* BoxBuilding::unpack(void* buf)
{
  buf = nboUnpackFVec3(buf, pos);
  buf = nboUnpackFloat(buf, angle);
  buf = nboUnpackFVec3(buf, size);

  unsigned char stateByte;
  buf = nboUnpackUInt8(buf, stateByte);
  driveThrough = (stateByte & _DRIVE_THRU) != 0 ? 0xFF : 0;
  shootThrough = (stateByte & _SHOOT_THRU) != 0 ? 0xFF : 0;
  ricochet     = (stateByte & _RICOCHET)   != 0;

  finalize();

  return buf;
}


int BoxBuilding::packSize() const
{
  int fullSize = 0;
  fullSize += sizeof(fvec3);   // pos
  fullSize += sizeof(fvec3);   // size
  fullSize += sizeof(float);   // rotation
  fullSize += sizeof(uint8_t); // state bits
  return fullSize;
}


void BoxBuilding::print(std::ostream& out, const std::string& indent) const
{
  out << indent << "box" << std::endl;
  const fvec3& myPos = getPosition();
  out << indent << "  position " << myPos[0] << " " << myPos[1] << " "
				 << myPos[2] << std::endl;
  out << indent << "  size " << getWidth() << " " << getBreadth()
			     << " " << getHeight() << std::endl;
  out << indent << "  rotation " << ((getRotation() * 180.0) / M_PI)
				 << std::endl;
  if (isPassable()) {
    out << indent << "  passable" << std::endl;
  } else {
    if (isDriveThrough()) {
      out << indent << "  drivethrough" << std::endl;
    }
    if (isShootThrough()) {
      out << indent << "  shootthrough" << std::endl;
    }
  }
  if (canRicochet()) {
    out << indent << "  ricochet" << std::endl;
  }
  out << indent << "end" << std::endl << std::endl;
  return;
}


static void outputFloat(std::ostream& out, float value)
{
  char buffer[32];
  snprintf(buffer, 30, " %.8f", value);
  out << buffer;
  return;
}

void BoxBuilding::printOBJ(std::ostream& out, const std::string& /*indent*/) const
{
  int i;
  fvec3 verts[8] = {
    fvec3(-1.0f, -1.0f, 0.0f),
    fvec3(+1.0f, -1.0f, 0.0f),
    fvec3(+1.0f, +1.0f, 0.0f),
    fvec3(-1.0f, +1.0f, 0.0f),
    fvec3(-1.0f, -1.0f, 1.0f),
    fvec3(+1.0f, -1.0f, 1.0f),
    fvec3(+1.0f, +1.0f, 1.0f),
    fvec3(-1.0f, +1.0f, 1.0f)
  };
  fvec3 norms[6] = {
    fvec3(0.0f, -1.0f, 0.0f), fvec3(+1.0f, 0.0f, 0.0f),
    fvec3(0.0f, +1.0f, 0.0f), fvec3(-1.0f, 0.0f, 0.0f),
    fvec3(0.0f, 0.0f, -1.0f), fvec3(0.0f, 0.0f, +1.0f)
  };
  const fvec3& s = getSize();
  const float k = 1.0f / 8.0f;
  fvec2 txcds[8] = {
    fvec2(0.0f, 0.0f), fvec2(k*s[0], 0.0f), fvec2(k*s[0], k*s[2]),
    fvec2(0.0f, k*s[2]), fvec2(k*s[1], 0.0f), fvec2(k*s[1], k*s[2]),
    fvec2(k*s[0], k*s[1]), fvec2(0.0f, k*s[1])
  };
  MeshTransform xform;
  const float degrees = getRotation() * (float)(180.0 / M_PI);
  const fvec3 zAxis(0.0f, 0.0f, +1.0f);
  xform.addScale(s);
  xform.addSpin(degrees, zAxis);
  xform.addShift(getPosition());
  xform.finalize();
  MeshTransform::Tool xtool(xform);
  for (i = 0; i < 8; i++) {
    xtool.modifyVertex(verts[i]);
  }
  for (i = 0; i < 6; i++) {
    xtool.modifyNormal(norms[i]);
  }

  out << "# OBJ - start box" << std::endl;
  out << "o bzbox_" << getObjCounter() << std::endl;

  for (i = 0; i < 8; i++) {
    out << "v";
    outputFloat(out, verts[i][0]);
    outputFloat(out, verts[i][1]);
    outputFloat(out, verts[i][2]);
    out << std::endl;
  }
  for (i = 0; i < 8; i++) {
    out << "vt";
    outputFloat(out, txcds[i][0]);
    outputFloat(out, txcds[i][1]);
    out << std::endl;
  }
  for (i = 0; i < 6; i++) {
    out << "vn";
    outputFloat(out, norms[i][0]);
    outputFloat(out, norms[i][1]);
    outputFloat(out, norms[i][2]);
    out << std::endl;
  }
  out << "usemtl boxtop" << std::endl;
  out << "f -5/-8/-2 -6/-7/-2 -7/-2/-2 -8/-1/-2" << std::endl;
  out << "f -4/-8/-1 -3/-7/-1 -2/-2/-1 -1/-1/-1" << std::endl;
  out << "usemtl boxwall" << std::endl;
  out << "f -8/-8/-6 -7/-7/-6 -3/-6/-6 -4/-5/-6" << std::endl;
  out << "f -7/-8/-5 -6/-4/-5 -2/-3/-5 -3/-5/-5" << std::endl;
  out << "f -6/-8/-4 -5/-7/-4 -1/-6/-4 -2/-5/-4" << std::endl;
  out << "f -5/-8/-3 -8/-4/-3 -4/-3/-3 -1/-5/-3" << std::endl;

  out << std::endl;

  incObjCounter();

  return;
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
