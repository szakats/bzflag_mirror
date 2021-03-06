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

// bzflag common header
#include "common.h"

// interface header
#include "FlagSceneNode.h"

// system headers
#include <stdlib.h>
#include <math.h>

// common headers
#include "bzfgl.h"
#include "vectors.h"
#include "OpenGLGState.h"
#include "OpenGLMaterial.h"
#include "ViewFrustum.h"
#include "SceneRenderer.h"
#include "StateDatabase.h"
#include "BZDBCache.h"


// FIXME - flag geometry would benefit from VBOs
// FIXME - DL/VBO's for the "fancy pole"? (alpha is the prob)
// FIXME - pole visibility based on LOD is not quite right
// FIXME - had to remove the LOD'ed spherical pole and cylindrical pole cap


static const int maxFlagLOD   = (maxFlagLODs - 1);
static const int maxFlagQuads = (1 << maxFlagLOD);
static const int maxFlagVerts = 2 * (maxFlagQuads + 1);

static bool geoPole = false;	// draw the pole as quads
static bool realFlag = false;	// don't use billboarding

static const float Unit = 0.8f;
static const float Width = 1.5f * Unit;
static const float Height = Unit;

static const fvec4 specular(0.3f, 0.3f, 0.3f, 1.0f);
static const fvec4 emission(0.0f, 0.0f, 0.0f, 1.0f);


//============================================================================//
//
// FlagPhase  (local class)
//

static const int maxFlagPhases = 8;


class FlagPhase {
  public:

    static FlagPhase*	getPhase();
    static void		setTimeStep(float dt);
    static void		updatePhases(float waveSpeed);

    // return the triangle count
    int render(int lod) const;
    int renderShadow(int lod) const;

    void updateMaxLOD(int);

  private:
    FlagPhase();
    ~FlagPhase();

    void update(float dt);

  private:
    int maxLOD;
    int activeLOD;

    float ripple1;
    float ripple2;

    fvec3 verts[maxFlagVerts];
    fvec3 norms[maxFlagVerts];
    fvec2 txcds[maxFlagVerts];

  private:
    static void makeIndices();
    static void freeIndices();

  private:
    static FlagPhase phases[maxFlagPhases];

    static GLushort* indices[maxFlagLODs];
    static int elementCounts[maxFlagLODs];

    static int counter;
    static float timeStep;
    static const float RippleSpeed1;
    static const float RippleSpeed2;
};


//============================================================================//
//
// FlagPhase static data
//


GLushort* FlagPhase::indices[maxFlagLODs] = { NULL };
int FlagPhase::elementCounts[maxFlagLODs];

FlagPhase FlagPhase::phases[maxFlagPhases];

int FlagPhase::counter = 0;
float FlagPhase::timeStep = 0.0f;
const float FlagPhase::RippleSpeed1 = (float)(2.4 * M_PI);
const float FlagPhase::RippleSpeed2 = (float)(1.724 * M_PI);


//============================================================================//
//
// FlagPhase static functions
//

FlagPhase* FlagPhase::getPhase()
{
  counter = (counter + 1) % maxFlagPhases;
  return &phases[counter];
}


void FlagPhase::updatePhases(float waveSpeed)
{
  // not really required
  if (BZDBCache::maxFlagLOD < 0) {
    BZDB.setInt("maxFlagLOD", 0);
  } else if (BZDBCache::maxFlagLOD > maxFlagLOD) {
    BZDB.setInt("maxFlagLOD", maxFlagLOD);
  }

  for (int i = 0; i < maxFlagPhases; i++) {
    phases[i].update(timeStep * waveSpeed);
  }
  return;
}


void FlagPhase::setTimeStep(float _dt)
{
  timeStep = _dt;
  return;
}


void FlagPhase::makeIndices()
{
  // NOTE: binary interleaved patterns for different LODs
  //       - used as element indices to glDrawElements
  //       - used as reverse mapping indices (it works out that way)

  int quads = 1; // doubled at the end of the 'for' loop

  for (int i = 0; i < maxFlagLODs; i++) {

    const int elements = 2 * ((1 << i) + 1);
    elementCounts[i] = elements;

    indices[i] = new GLushort[elements];
    indices[i][0] = 0;
    indices[i][1] = 1;
    indices[i][elements - 2] = 2;
    indices[i][elements - 1] = 3;

    int skip = quads;
    int element = 2;
    while (skip > 1) {
      int pos = skip / 2;
      while (pos <= quads) {
	indices[i][pos*2] = (element * 2);
	indices[i][pos*2+1] = (element * 2) + 1;
	element++;
	pos = pos + skip;
      }
      skip = skip / 2;
    }

    quads *= 2;
  }

  return;
}


void FlagPhase::freeIndices()
{
  for (int i = 0; i < maxFlagLODs; i++) {
    delete[] indices[i];
  }
  return;
}


//============================================================================//
//
// FlagPhase member functions
//

FlagPhase::FlagPhase()
{
  maxLOD = -1;
  activeLOD = 0;

  const float myFrac = (float)(this - phases) / (float)maxFlagPhases;
  ripple1 = (float)(1.0 * M_PI * myFrac);
  ripple2 = (float)(2.0 * M_PI * myFrac);

  if (this == phases) {
    makeIndices();
  }

  return;
}


FlagPhase::~FlagPhase()
{
  if (this == phases) {
    freeIndices();
  }
  return;
}


inline void FlagPhase::updateMaxLOD(int lod)
{
  if (lod > maxLOD) {
    maxLOD = lod;
  }
  return;
}


inline int FlagPhase::render(int lod) const
{
  if (lod > activeLOD) {
    lod = activeLOD;
  }

  const int count = elementCounts[lod];
  glVertexPointer(3, GL_FLOAT, 0, verts);
  glNormalPointer(GL_FLOAT, 0, norms);
  glTexCoordPointer(2, GL_FLOAT, 0, txcds);

  if (GLEW_EXT_draw_range_elements)
    glDrawRangeElements(GL_QUAD_STRIP, 0, count - 1, count, GL_UNSIGNED_SHORT, indices[lod]);
  else
    glDrawElements(GL_QUAD_STRIP, count, GL_UNSIGNED_SHORT, indices[lod]);

  return count;
}


inline int FlagPhase::renderShadow(int lod) const
{
  if (lod > activeLOD) {
    lod = activeLOD;
  }

  const int count = elementCounts[lod];
  glVertexPointer(3, GL_FLOAT, 0, verts);

  if (GLEW_EXT_draw_range_elements)
    glDrawRangeElements(GL_QUAD_STRIP, 0, count - 1, count, GL_UNSIGNED_SHORT, indices[lod]);
  else
    glDrawElements(GL_QUAD_STRIP, count, GL_UNSIGNED_SHORT, indices[lod]);

  return count;
}


void FlagPhase::update(float dt)
{
  if (maxLOD < 0) {
    return; // no flags in the current view use this phase
  }

  activeLOD = maxLOD;
  if (activeLOD > BZDBCache::maxFlagLOD) {
    activeLOD = BZDBCache::maxFlagLOD;
  }
  maxLOD = -1; // reset it

  const int quads = (1 << activeLOD);

  const GLushort* lookup = indices[activeLOD];

  int i;

  ripple1 += dt * RippleSpeed1;
  if (ripple1 >= (float)(2.0 * M_PI)) {
    ripple1 -= (float)(2.0 * M_PI);
  }

  ripple2 += dt * RippleSpeed2;
  if (ripple2 >= (float)(2.0 * M_PI)) {
    ripple2 -= (float)(2.0 * M_PI);
  }

  float sinRipple2  = sinf(ripple2);
  float sinRipple2S = sinf((float)(ripple2 + (1.16 * M_PI)));
  float	wave0[maxFlagQuads + 1];
  float	wave1[maxFlagQuads + 1];
  float	wave2[maxFlagQuads + 1];

  for (i = 0; i <= quads; i++) {
    const float x      = float(i) / float(quads);
    const float damp   = 0.1f * x;
    const float angle1 = (float)(ripple1 - (4.0 * M_PI * x));
    const float angle2 = (float)(angle1 - (0.28 * M_PI));

    wave0[i] = damp * sinf(angle1);
    wave1[i] = damp * (sinf(angle2) + sinRipple2S);
    wave2[i] = wave0[i] + damp * sinRipple2;
  }

  float base = BZDBCache::flagPoleSize;
  for (i = 0; i <= quads; i++) {
    const float x      = float(i) / float(quads);
    const float shift1 = wave0[i];

    const int it = lookup[(i * 2)];
    const int ib = lookup[(i * 2) + 1];

    verts[it].x = Width * x;
    verts[ib].x = Width * x;
    if (realFlag) {
      // flag pole is Z axis
      verts[it].y = wave1[i];
      verts[ib].y = wave2[i];
      verts[it].z = base + Height - shift1;
      verts[ib].z = base - shift1;
    } else {
      // flag pole is Y axis
      verts[it].y = base + Height - shift1;
      verts[ib].y = base - shift1;
      verts[it].z = wave1[i];
      verts[ib].z = wave2[i];
    }
    txcds[it].x = x;
    txcds[ib].x = x;
    txcds[it].y = 1.0f;
    txcds[ib].y = 0.0f;
  }

  const fvec3 negY(0.0f, -1.0f, 0.0f);

  // generate the lighting normals
  if (realFlag && BZDBCache::lighting) {
    fvec3  upEdges[maxFlagQuads + 1];
    fvec3 topEdges[maxFlagQuads + 1];
    fvec3 botEdges[maxFlagQuads + 1];
    for (i = 0; i < quads; i++) {
      const int ue = lookup[(i * 2)];
      const int us = lookup[(i * 2) + 1];
      upEdges[i]  = verts[ue] - verts[us];
      const int ts = lookup[(i * 2)];
      const int te = lookup[(i * 2) + 2];
      topEdges[i] = verts[te] - verts[ts];
      const int bs = lookup[(i * 2) + 1];
      const int be = lookup[(i * 2) + 2 + 1];
      botEdges[i] = verts[be] - verts[bs];
    }
    norms[0] = negY;
    const int lastTop = lookup[(quads * 2)];
    const int lastBot = lookup[(quads * 2) + 1];
    norms[lastTop] = negY;
    norms[lastBot] = negY;
    for (i = 1; i < quads; i++) {
      fvec3 n0, n1, na;

      n0 = fvec3::cross(topEdges[i - 1], upEdges[i]);
      n1 = fvec3::cross(topEdges[i],     upEdges[i]);
      na = n0 + n1;
      const int it = lookup[(i * 2)];
      norms[it] = fvec3::normalize(na) ? na : negY;

      n0 = fvec3::cross(botEdges[i - 1], upEdges[i]);
      n1 = fvec3::cross(botEdges[i],     upEdges[i]);
      na = n0 + n1;
      const int ib = lookup[(i * 2) + 1];
      norms[ib] = fvec3::normalize(na) ? na : negY;
    }
  }
  return;
}


//============================================================================//
//
// FlagSceneNode
//

// length per pixel thresholds (last one is ignored)
const float FlagSceneNode::lodLengths[maxFlagLODs] = {
  0.6400f, 0.3200f, 0.1600f,
  0.0800f, 0.0400f, 0.0200f,
  0.0100f, 0.0050f, 0.0025f
};

const int FlagSceneNode::minPoleLOD = 3;


FlagSceneNode::FlagSceneNode(const fvec3& pos) : renderNode(this)
{
  phase = FlagPhase::getPhase();

  lod = 0;
  shadowLOD = 0;

  angle = 0.0f;
  tilt = 0.0f;
  hscl = 1.0f;
  flat = false;
  translucent = false;
  texturing = false;
  useColor = true;
  setColor(1.0f, 1.0f, 1.0f, 1.0f);
  whiteColor = fvec4(1.0f, 1.0f, 1.0f, 1.0f);
  color = &realColor;
  setCenter(pos);
  setRadius(6.0f * Unit * Unit);

  return;
}


FlagSceneNode::~FlagSceneNode()
{
  return;
}


void FlagSceneNode::setTimeStep(float dt)
{
  // this is stupid
  FlagPhase::setTimeStep(dt);
  return;
}


void FlagSceneNode::waveFlags(float waveSpeed)
{
  // This should be done after the flags
  // have passed through addRenderNodes(),
  // but before they are rendered.
  FlagPhase::updatePhases(waveSpeed);
  return;
}


void FlagSceneNode::move(const fvec3& pos)
{
  setCenter(pos);
  return;
}


void FlagSceneNode::setAngle(float _angle)
{
  angle = (float)(_angle * RAD2DEG);
  tilt = 0.0f;
  hscl = 1.0f;
  return;
}


void FlagSceneNode::setWind(const fvec3& wind, float dt)
{
  if (!realFlag) {
    angle = atan2f(wind[1], wind[0]) * RAD2DEGf;
    tilt = 0.0f;
    hscl = 1.0f;
  } else {
    // the angle points from the end of the flag to the pole
    const float cos_val = cosf(angle * DEG2RADf);
    const float sin_val = sinf(angle * DEG2RADf);
    const float force = (wind[0] * sin_val) - (wind[1] * cos_val);
    const float angleScale = 25.0f;
    angle = fmodf(angle + (force * dt * angleScale), 360.0f);

    const float horiz = sqrtf((wind[0] * wind[0]) + (wind[1] * wind[1]));
    const float it = -0.75f; // idle tilt
    const float tf = +5.00f; // tilt factor
    const float desired = (wind[2] / (horiz + tf)) +
			  (it * (1.0f - horiz / (horiz + tf)));

    const float tt = dt * 5.0f;
    tilt = (tilt * (1.0f - tt)) + (desired * tt);

    const float maxTilt = 1.5f;
    if (tilt > +maxTilt) {
      tilt = +maxTilt;
    } else if (tilt < -maxTilt) {
      tilt = -maxTilt;
    }
    hscl = 1.0f / sqrtf(1.0f + (tilt * tilt));
  }
  return;
}


void FlagSceneNode::setFlat(bool value)
{
  flat = value;
  return;
}


void FlagSceneNode::setUseColor(bool value)
{
  useColor = value;
  return;
}


void FlagSceneNode::setAlpha(float a)
{
  realColor.a = a;
  whiteColor.a = a;
  translucent = (a != 1.0f);
  return;
}


void FlagSceneNode::setColor(float r, float g, float b, float a)
{
  realColor = fvec4(r, g, b, a);
  whiteColor.a = a;
  translucent = (a != 1.0f);
  return;
}


void FlagSceneNode::setColor(const fvec4& rgba)
{
  realColor = rgba;
  whiteColor.a = rgba.a;
  translucent = (rgba.a != 1.0f);
  return;
}


void FlagSceneNode::setTexture(int texture)
{
  OpenGLGStateBuilder builder(gstate);
  builder.setTexture(texture);
  builder.enableTexture(texture>=0);
  gstate = builder.getState();
  return;
}


void FlagSceneNode::notifyStyleChange()
{
  const int quality = RENDERER.useQuality();
  geoPole = (quality >= _MEDIUM_QUALITY);
  realFlag = (quality >= _EXPERIMENTAL_QUALITY);

  texturing = BZDBCache::texture && BZDBCache::blend; // FIXME -- alphatresh'ed
  OpenGLGStateBuilder builder(gstate);
  builder.enableTexture(texturing);

  if (texturing) {
    if (useColor) {
      color = &realColor;
    } else {
      color = &whiteColor;
    }
  } else {
    color = &realColor;
  }

  if (translucent) {
    if (BZDBCache::blend) {
      builder.setBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      builder.setStipple(1.0f);
    } else if (translucent) {
      builder.resetBlending();
      builder.setStipple(0.5f);
    }
    builder.resetAlphaFunc();
  } else {
    builder.resetBlending();
    builder.setStipple(1.0f);
    if (texturing) {
      builder.setAlphaFunc(GL_GEQUAL, 0.9f);
    } else {
      builder.resetAlphaFunc();
    }
  }

  if (realFlag && BZDBCache::lighting) {
    OpenGLMaterial glmat(specular, emission, 64.0f);
    builder.setMaterial(glmat, quality > _LOW_QUALITY);
  } else {
    builder.enableMaterial(false);
  }

  if (!flat && !realFlag) {
    builder.setCulling(GL_BACK);
  } else {
    builder.setCulling(GL_NONE);
  }

  gstate = builder.getState();

  return;
}


inline int FlagSceneNode::calcLOD(const SceneRenderer& renderer)
{
  const ViewFrustum& vf = renderer.getViewFrustum();
  const fvec3& pos = getSphere().xyz();
  const fvec3& eye = vf.getEye();
  const fvec3& dir = vf.getDirection();
  const float dist = fvec3::dot(dir, pos - eye);

  const float lpp = dist * renderer.getLengthPerPixel();

  int i;
  for (i = 0; i < maxFlagLOD; i++) {
    if (lpp > lodLengths[i]) {
      return i;
    }
  }
  return i;
}


inline int FlagSceneNode::calcShadowLOD(const SceneRenderer& renderer)
{
  const fvec3& pos = getSphere().xyz();
  const fvec3& eye = renderer.getViewFrustum().getEye();
  const fvec3* dir = renderer.getSunDirection();
  const float dist = fvec3::cross(pos - eye, *dir).length();

  const float lpp = dist * renderer.getLengthPerPixel();

  int i;
  for (i = 0; i < maxFlagLOD; i++) {
    if (lpp > lodLengths[i]) {
      return i;
    }
  }
  return i;
}


void FlagSceneNode::addRenderNodes(SceneRenderer& renderer)
{
  renderer.addRenderNode(&renderNode, &gstate);
  lod = calcLOD(renderer);
  phase->updateMaxLOD(lod);
  return;
}


void FlagSceneNode::addShadowNodes(SceneRenderer& renderer)
{
  renderer.addShadowNode(&renderNode);
  shadowLOD = calcShadowLOD(renderer);
  phase->updateMaxLOD(shadowLOD);
  return;
}


bool FlagSceneNode::cullShadow(int planeCount, const fvec4* planes) const
{
  const fvec4& s = getSphere();
  for (int i = 0; i < planeCount; i++) {
    const float d = planes[i].planeDist(s.xyz());
    if ((d < 0.0f) && ((d * d) > s.w)) {
      return true;
    }
  }
  return false;
}


//============================================================================//
//
// FlagSceneNode::FlagRenderNode
//

FlagSceneNode::FlagRenderNode::FlagRenderNode(const FlagSceneNode* _sceneNode)
{
  sceneNode = _sceneNode;
  isShadow = false;
  return;
}


FlagSceneNode::FlagRenderNode::~FlagRenderNode()
{
  return;
}


void FlagSceneNode::FlagRenderNode::renderFancyPole()
{
  const bool lighting = realFlag && BZDBCache::lighting;
  const float poleWidth = BZDBCache::flagPoleWidth;
  const float base = BZDBCache::flagPoleSize;

  const float pw = poleWidth;
  const float pw2 = 2.0f * poleWidth;
  const float topHeight = base + Height;
  const float baseHeight = pw2;
  const float sqrt1_2 = 0.70710678f;
  const float sqrt1_3 = 0.57735027f;

  // the pole base
  if (!isShadow) {
    glColor4f(0.25f, 0.25f, 0.5f, sceneNode->color->a); // blue
  }
  glBegin(GL_QUAD_STRIP);
  {
    glVertex3f(-pw2, 0.0f, baseHeight);
    glVertex3f(-pw2, 0.0f, 0.0f);
    glNormal3f(-sqrt1_2, -sqrt1_2, 0.0f);
    glVertex3f(0.0f, -pw2, baseHeight);
    glVertex3f(0.0f, -pw2, 0.0f);
    glNormal3f(+sqrt1_2, -sqrt1_2, 0.0f);
    glVertex3f(+pw2, 0.0f, baseHeight);
    glVertex3f(+pw2, 0.0f, 0.0f);
    glNormal3f(+sqrt1_2, +sqrt1_2, 0.0f);
    glVertex3f(0.0f, +pw2, baseHeight);
    glVertex3f(0.0f, +pw2, 0.0f);
    glNormal3f(-sqrt1_2, +sqrt1_2, 0.0f);
    glVertex3f(-pw2, 0.0f, baseHeight);
    glVertex3f(-pw2, 0.0f, 0.0f);
  }
  glEnd();
  glBegin(GL_QUADS);
  {
    glVertex3f(-pw2, 0.0f, baseHeight);
    glVertex3f(0.0f, -pw2, baseHeight);
    glVertex3f(+pw2, 0.0f, baseHeight);
    glVertex3f(0.0f, +pw2, baseHeight);
  }
  glEnd();
  addTriangleCount(10);

  // the pole cap
  if (!isShadow) {
    glColor4f(0.5f, 0.5f, 0.25f, sceneNode->color->a); // yellow
    if (lighting) {
      const float yellow[4] = {0.4f, 0.4f, 0.2f, 1.0f};
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
    }
  }
  const float bot = topHeight;
  const float mid = topHeight + pw;
  const float peak = topHeight + (3.0f * pw);
  glBegin(GL_QUAD_STRIP);
  {
    glVertex3f(-pw, 0.0f, bot);
    glVertex3f(-pw2, 0.0f, mid);
    glNormal3f(-sqrt1_3, -sqrt1_3, -sqrt1_3);
    glVertex3f(0.0f, -pw, bot);
    glVertex3f(0.0f, -pw2, mid);
    glNormal3f(+sqrt1_3, -sqrt1_3, -sqrt1_3);
    glVertex3f(+pw, 0.0f, bot);
    glVertex3f(+pw2, 0.0f, mid);
    glNormal3f(+sqrt1_3, +sqrt1_3, -sqrt1_3);
    glVertex3f(0.0f, +pw, bot);
    glVertex3f(0.0f, +pw2, mid);
    glNormal3f(-sqrt1_3, +sqrt1_3, -sqrt1_3);
    glVertex3f(-pw, 0.0f, bot);
    glVertex3f(-pw2, 0.0f, mid);
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  {
    glVertex3f(0.0f, 0.0f, peak);
    glVertex3f(-pw2, 0.0f, mid);
    glNormal3f(-sqrt1_3, -sqrt1_3, +sqrt1_3);
    glVertex3f(0.0f, -pw2, mid);
    glNormal3f(+sqrt1_3, -sqrt1_3, +sqrt1_3);
    glVertex3f(+pw2, 0.0f, mid);
    glNormal3f(+sqrt1_3, +sqrt1_3, +sqrt1_3);
    glVertex3f(0.0f, +pw2, mid);
    glNormal3f(-sqrt1_3, +sqrt1_3, +sqrt1_3);
    glVertex3f(-pw2, 0.0f, mid);
  }
  glEnd();
  addTriangleCount(12);

  // the pole
  if (!isShadow) {
    glColor4f(0.1f, 0.1f, 0.1f, sceneNode->color->a); // dark grey
    if (lighting) {
      const float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    }
  }
  glBegin(GL_QUAD_STRIP);
  {
    glVertex3f(-poleWidth, 0.0f, baseHeight);
    glVertex3f(-poleWidth, 0.0f, topHeight);
    glNormal3f(-sqrt1_2, -sqrt1_2, 0.0f);
    glVertex3f(0.0f, -poleWidth, baseHeight);
    glVertex3f(0.0f, -poleWidth, topHeight);
    glNormal3f(+sqrt1_2, -sqrt1_2, 0.0f);
    glVertex3f(+poleWidth, 0.0f, baseHeight);
    glVertex3f(+poleWidth, 0.0f, topHeight);
    glNormal3f(+sqrt1_2, +sqrt1_2, 0.0f);
    glVertex3f(0.0f, +poleWidth, baseHeight);
    glVertex3f(0.0f, +poleWidth, topHeight);
    glNormal3f(-sqrt1_2, +sqrt1_2, 0.0f);
    glVertex3f(-poleWidth, 0.0f, baseHeight);
    glVertex3f(-poleWidth, 0.0f, topHeight);
  }
  glEnd();
  addTriangleCount(8);
  if (!isShadow && lighting) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  }
  return;
}


void FlagSceneNode::FlagRenderNode::render()
{
  float base = BZDBCache::flagPoleSize;
  float poleWidth = BZDBCache::flagPoleWidth;
  const bool flat = sceneNode->flat;
  const bool texturing = sceneNode->texturing;
  const bool translucent = sceneNode->translucent;
  const fvec4& sphere = sceneNode->getSphere();
  const FlagPhase* phase = sceneNode->phase;
  const int lod = isShadow ? sceneNode->shadowLOD : sceneNode->lod;

  if (!isShadow) {
    glColor4fv(*sceneNode->color);
    if (!BZDBCache::blend && (translucent || texturing)) {
      myStipple(sceneNode->color->a);
    }
  }

  glPushMatrix();
  {
    glTranslatef(sphere.x, sphere.y, sphere.z);

    if (realFlag) {

      // the flag
      if (!isShadow) {
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
      }
      glRotatef(sceneNode->angle + 180.0f, 0.0f, 0.0f, 1.0f);
      if (flat) {
	glScalef(1.0f, 0.0f, 1.0f); // flatten along Y axis
      }
      const float tilt = sceneNode->tilt;
      const float hscl = sceneNode->hscl;
      static float shear[16] = { hscl, 0.0f, tilt, 0.0f,
                                 0.0f, 1.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 1.0f };
      shear[0] = hscl; // maintains the flag length
      shear[2] = tilt; // pulls the flag up or down
      glPushMatrix();
      glMultMatrixf(shear);
      if (!isShadow) {
	addTriangleCount(phase->render(lod));
      } else {
	addTriangleCount(phase->renderShadow(lod));
      }
      glPopMatrix();

      if (!isShadow) {
	glDisable(GL_NORMALIZE);
	glShadeModel(GL_FLAT);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	if (texturing) {
	  glDisable(GL_TEXTURE_2D);
	}
      }

      // draw the pole, if close enough
      if (lod >= minPoleLOD) {
	renderFancyPole();
      }
    }
    else {
      if (!flat) {
	RENDERER.getViewFrustum().executeBillboard();
	if (!isShadow) {
	  addTriangleCount(phase->render(lod));
	} else {
	  addTriangleCount(phase->renderShadow(lod));
	}
      }
      else {
	glRotatef(sceneNode->angle + 180.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	{
	  glTexCoord2f(0.0f, 0.0f);
	  glVertex3f(0.0f, base, 0.0f);
	  glTexCoord2f(1.0f, 0.0f);
	  glVertex3f(Width, base, 0.0f);
	  glTexCoord2f(1.0f, 1.0f);
	  glVertex3f(Width, base + Height, 0.0f);
	  glTexCoord2f(0.0f, 1.0f);
	  glVertex3f(0.0f, base + Height, 0.0f);
	}
	glEnd();
	addTriangleCount(2);
      }

      if (!isShadow) {
	glColor4f(0.0f, 0.0f, 0.0f, sceneNode->color->a);
	if (texturing) {
	  glDisable(GL_TEXTURE_2D);
	}
      }

      if (geoPole) {
	glBegin(GL_QUADS);
	{
	  glVertex3f(-poleWidth, 0.0f, 0.0f);
	  glVertex3f(+poleWidth, 0.0f, 0.0f);
	  glVertex3f(+poleWidth, base + Height, 0.0f);
	  glVertex3f(-poleWidth, base + Height, 0.0f);
	}
	glEnd();
	addTriangleCount(2);
      } else {
	glBegin(GL_LINE_STRIP);
	{
	  glVertex3f(0.0f, 0.0f, 0.0f);
	  glVertex3f(0.0f, base + Height, 0.0f);
	}
	glEnd();
	addTriangleCount(1);
      }
    }
  }
  glPopMatrix();

  if (!isShadow) {
    if (texturing) {
      glEnable(GL_TEXTURE_2D);
    }
    if (!BZDBCache::blend && translucent) {
      myStipple(0.5f);
    }
  }

  return;
}


void FlagSceneNode::FlagRenderNode::renderShadow()
{
  isShadow = true;
  render();
  isShadow = false;
  return;
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
