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

// interface header
#include "BackgroundRenderer.h"

// system headers
#include <string.h>

// common headers
#include "OpenGLMaterial.h"
#include "TextureManager.h"
#include "BZDBCache.h"
#include "BzMaterial.h"
#include "TextureMatrix.h"
#include "ParseColor.h"
#include "BZDBCache.h"

// local headers
#include "Daylight.h"
#include "stars.h"
#include "MainWindow.h"
#include "SceneNode.h"
#include "EffectsRenderer.h"

static const fvec3 squareShape[4] = {
  fvec3(+1.0f, +1.0f, 0.0f),
  fvec3(-1.0f, +1.0f, 0.0f),
  fvec3(-1.0f, -1.0f, 0.0f),
  fvec3(+1.0f, -1.0f, 0.0f)
};


fvec3			BackgroundRenderer::skyPyramid[5];
const float		BackgroundRenderer::cloudRepeats = 3.0f;
static const int	NumMountainFaces = 16;

fvec4 BackgroundRenderer::groundColor[4];
fvec4 BackgroundRenderer::groundColorInv[4];

const fvec4 BackgroundRenderer::defaultGroundColor[4] = {
  fvec4(0.0f, 0.35f, 0.0f, 1.0f),
  fvec4(0.0f, 0.20f, 0.0f, 1.0f),
  fvec4(1.0f, 1.00f, 1.0f, 1.0f),
  fvec4(1.0f, 1.00f, 1.0f, 1.0f)
};
const fvec4 BackgroundRenderer::defaultGroundColorInv[4] = {
  fvec4(0.35f, 0.00f, 0.35f, 1.0f),
  fvec4(0.20f, 0.00f, 0.20f, 1.0f),
  fvec4(1.00f, 1.00f, 1.00f, 1.0f),
  fvec4(1.00f, 1.00f, 1.00f, 1.0f)
};
const fvec4 BackgroundRenderer::receiverColor    = fvec4(0.3f, 0.55f, 0.3f, 1.0f);
const fvec4 BackgroundRenderer::receiverColorInv = fvec4(0.55f, 0.3f, 0.55f, 1.0f);


BackgroundRenderer::BackgroundRenderer(const SceneRenderer&)
: blank(false)
, invert(false)
, style(0)
, gridSpacing(60.0f)	// meters
, gridCount(4.0f)
, mountainsAvailable(false)
, numMountainTextures(0)
, mountainsGState(NULL)
, cloudDriftU(0.0f)
, cloudDriftV(0.0f)
{
  static bool init = false;
  OpenGLGStateBuilder gstate;
  static const fvec4 black(0.0f, 0.0f, 0.0f, 1.0f);
  static const fvec4 white(1.0f, 1.0f, 1.0f, 1.0f);
  OpenGLMaterial defaultMaterial(black, black, 0.0f);
  OpenGLMaterial rainMaterial(white, white, 0.0f);

  int i;

  lastRenderer = NULL;

  //display lists
  sunXFormList = _INVALID_LIST;
  moonList = _INVALID_LIST;
  starXFormList = _INVALID_LIST;
  lowGroundList	= DisplayListSystem::Instance().newList(this);
  mediumGroundList = DisplayListSystem::Instance().newList(this);
  cloudsList = DisplayListSystem::Instance().newList(this);
  // initialize global to class stuff
  if (!init) {
    init = true;
    resizeSky();
  }

  // initialize the celestial vectors
  static const fvec3 up(0.0f, 0.0f, 1.0f);
  sunDirection = up;
  moonDirection = up;

  // make ground materials
  setupSkybox();
  setupGroundMaterials();

  TextureManager &tm = TextureManager::instance();

  // make grid stuff
  gstate.reset();
  gstate.setBlending();
  gstate.setSmoothing();
  gridGState = gstate.getState();

  // make receiver stuff
  gstate.reset();
  gstate.setShading();
  gstate.setBlending((GLenum)GL_SRC_ALPHA, (GLenum)GL_ONE);
  receiverGState = gstate.getState();

  // sun shadow stuff
  gstate.reset();
  gstate.setStipple(0.5f);
  gstate.setCulling((GLenum)GL_NONE);
  sunShadowsGState = gstate.getState();

  // sky stuff
  gstate.reset();
  gstate.setShading();
  skyGState = gstate.getState();
  gstate.reset();
  sunGState = gstate.getState();
  gstate.reset();
  gstate.setBlending((GLenum)GL_ONE, (GLenum)GL_ONE);

  gstate.reset();
  starGState[0] = gstate.getState();
  gstate.reset();
  gstate.setBlending();
  gstate.setSmoothing();
  starGState[1] = gstate.getState();

  // make cloud stuff
  cloudsAvailable = false;
  int cloudsTexture = tm.getTextureID( "clouds" );
  if (cloudsTexture >= 0) {
    cloudsAvailable = true;
    gstate.reset();
    gstate.setShading();
    gstate.setBlending((GLenum)GL_SRC_ALPHA, (GLenum)GL_ONE_MINUS_SRC_ALPHA);
    gstate.setMaterial(defaultMaterial, RENDERER.useQuality() > _LOW_QUALITY);
    gstate.setTexture(cloudsTexture);
    gstate.setAlphaFunc();
    cloudsGState = gstate.getState();
  }

  // rain stuff
  weather.init();
  // effects
  EFFECTS.init();

  // make mountain stuff
  mountainsAvailable = false;
  {
    int mountainTexture = -1;
    int width = 0;
    int height = 0;
    numMountainTextures = 0;
    bool done = false;
    while (!done) {
      char text[256];
      sprintf (text, "mountain%d", numMountainTextures + 1);
      mountainTexture = tm.getTextureID (text, false);
      if (mountainTexture >= 0) {
	const ImageInfo & info = tm.getInfo (mountainTexture);
	height = info.y;
	width += info.x;
	numMountainTextures++;
      } else {
	done = true;
      }
    }

    if (numMountainTextures > 0) {
      mountainsAvailable = true;

      // prepare common gstate
      gstate.reset ();
      gstate.setShading ();
      gstate.setBlending ();
      gstate.setMaterial (defaultMaterial, RENDERER.useQuality() > _LOW_QUALITY);
      gstate.setAlphaFunc ();

      if (numMountainTextures > 1) {
	width -= 2 * numMountainTextures;
      }
      // find power of two at least as large as height
      int scaledHeight = 1;
      while (scaledHeight < height) {
	scaledHeight <<= 1;
      }

      // choose minimum width
      int minWidth = scaledHeight;
      if (minWidth > scaledHeight) {
	minWidth = scaledHeight;
      }
      mountainsMinWidth = minWidth;

      // prepare each texture
      mountainsGState = new OpenGLGState[numMountainTextures];
      mountanLists.clear();
      for (i = 0; i < numMountainTextures; i++) {
	char text[256];
	sprintf (text, "mountain%d", i + 1);
	gstate.setTexture (tm.getTextureID (text));
	mountainsGState[i] = gstate.getState ();
	mountanLists.push_back(DisplayListSystem::Instance().newList(this));
      }
    }
  }


  // reset the sky color when it changes
  BZDB.addCallback("_skyColor", bzdbCallback, this);
  BZDB.addCallback("_cloudHeightMult", bzdbCallback, this);

  notifyStyleChange();
}


BackgroundRenderer::~BackgroundRenderer()
{
  DisplayListSystem &ds = DisplayListSystem::Instance();

  ds.freeList(sunXFormList);
  ds.freeList(moonList);
  ds.freeList(starXFormList);
  ds.freeList(lowGroundList);
  ds.freeList(mediumGroundList);
  ds.freeList(cloudsList);
  for ( unsigned int i = 0; i < (unsigned int)mountanLists.size(); i++ )
    ds.freeList(mountanLists[i]);

  mountanLists.clear();

  BZDB.removeCallback("_skyColor", bzdbCallback, this);
  delete[] mountainsGState;
}


void BackgroundRenderer::bzdbCallback(const std::string& name, void* data)
{
  BackgroundRenderer* br = (BackgroundRenderer*) data;
  if (name == "_skyColor")
    br->setSkyColors();
  else if (name == "_cloudHeightMult")
    br->resetCloudList();
  return;
}


void BackgroundRenderer::resetCloudList()
{
  if (cloudsList > 0) {
    DisplayListSystem::Instance().freeList(cloudsList);
    cloudsList = DisplayListSystem::Instance().newList(this);
  }
}


void BackgroundRenderer::setupGroundMaterials()
{
  TextureManager &tm = TextureManager::instance();

  // see if we have a map specified material
  const BzMaterial* bzmat = MATERIALMGR.findMaterial("GroundMaterial");

  groundTextureID = -1;
  groundTextureMatrix = NULL;

  if (bzmat == NULL) {
    // default ground material
    memcpy(groundColor, defaultGroundColor, sizeof(fvec4[4]));
    groundTextureID = tm.getTextureID(BZDB.get("stdGroundTexture"), true);
  } else {
    // map specified material
    ((BzMaterial*)bzmat)->setReference();
    for (int i = 0; i < 4; i++) {
      groundColor[i] = bzmat->getDiffuse();
    }
    if (bzmat->getTextureCount() > 0) {
      groundTextureID = tm.getTextureID(bzmat->getTextureLocal(0), false);
      if (groundTextureID < 0) {
	// use the default as a backup (default color too)
	memcpy(groundColor, defaultGroundColor, sizeof(fvec4[4]));
	groundTextureID = tm.getTextureID(BZDB.get("stdGroundTexture"), true);
      } else {
	// only apply the texture matrix if the texture is valid
	const int texMatId = bzmat->getTextureMatrix(0);
	const TextureMatrix* texmat = TEXMATRIXMGR.getMatrix(texMatId);
	if (texmat != NULL) {
	  groundTextureMatrix = texmat->getMatrix();
	}
      }
    }
  }

  static const fvec4 black(0.0f, 0.0f, 0.0f, 1.0f);
  OpenGLMaterial defaultMaterial(black, black, 0.0f);

  OpenGLGStateBuilder gb;

  // ground gstates
  bool quality = RENDERER.useQuality() > _LOW_QUALITY;
  gb.reset();
  groundGState[0] = gb.getState();
  gb.reset();
  gb.setMaterial(defaultMaterial, quality);
  groundGState[1] = gb.getState();
  gb.reset();
  gb.setTexture(groundTextureID);
  gb.setTextureMatrix(groundTextureMatrix);
  groundGState[2] = gb.getState();
  gb.reset();
  gb.setMaterial(defaultMaterial, quality);
  gb.setTexture(groundTextureID);
  gb.setTextureMatrix(groundTextureMatrix);
  groundGState[3] = gb.getState();


  // default inverted ground material
  int groundInvTextureID = -1;
  memcpy(groundColorInv, defaultGroundColorInv, sizeof(fvec4[4]));
  if (groundInvTextureID < 0) {
    groundInvTextureID = tm.getTextureID(BZDB.get("zoneGroundTexture"), false);
  }

  // inverted ground gstates
  gb.reset();
  invGroundGState[0] = gb.getState();
  gb.reset();
  gb.setMaterial(defaultMaterial, quality);
  invGroundGState[1] = gb.getState();
  gb.reset();
  gb.setTexture(groundInvTextureID);
  invGroundGState[2] = gb.getState();
  gb.reset();
  gb.setMaterial(defaultMaterial, quality);
  gb.setTexture(groundInvTextureID);
  invGroundGState[3] = gb.getState();

  return;
}


void BackgroundRenderer::notifyStyleChange()
{
  if (BZDBCache::texture) {
    styleIndex = BZDBCache::lighting ? 3 : 2;
  } else {
    styleIndex = BZDBCache::lighting ? 1 : 0;
  }

  // some stuff is drawn only for certain states
  cloudsVisible = (styleIndex >= 2 && cloudsAvailable && BZDBCache::blend);
  mountainsVisible = (styleIndex >= 2 && mountainsAvailable);
  shadowsVisible = BZDB.isTrue("shadows");
  starGStateIndex = BZDB.isTrue("smooth");

  // fixup gstates
  OpenGLGStateBuilder gstate;
  gstate.reset();
  if (BZDB.isTrue("smooth")) {
    gstate.setBlending();
    gstate.setSmoothing();
  }
  gridGState = gstate.getState();
}


void BackgroundRenderer::resize()
{
  resizeSky();
}


void BackgroundRenderer::setCelestial(const SceneRenderer& renderer,
				      const float sunDir[3],
				      const float moonDir[3])
{
  // set sun and moon positions
  sunDirection[0] = sunDir[0];
  sunDirection[1] = sunDir[1];
  sunDirection[2] = sunDir[2];
  moonDirection[0] = moonDir[0];
  moonDirection[1] = moonDir[1];
  moonDirection[2] = moonDir[2];

  makeCelestialLists(renderer);

  return;
}


void BackgroundRenderer::buildMountain(unsigned int index)
{
  if (numMountainTextures == 0)
    return;

  float worldSize = BZDBCache::worldSize;

  // prepare display lists.  need at least NumMountainFaces, but
  // we also need a multiple of the number of subtextures.  put
  // all the faces using a given texture into the same list.
  const int numFacesPerTexture = (NumMountainFaces + numMountainTextures - 1) / numMountainTextures;
  const float angleScale = (float)(M_PI / (numMountainTextures * numFacesPerTexture));
  int n = numFacesPerTexture / 2 + (numFacesPerTexture * index);
  float hightScale = mountainsMinWidth / 256.0f;

  int i = 0;
  glBegin(GL_TRIANGLE_STRIP); {
    for (i = 0; i <= numFacesPerTexture; i++) {
      const float angle = angleScale * (float)(i + n);
      float frac = (float)i / (float)numFacesPerTexture;
      if (numMountainTextures != 1)
	frac = (frac * (float)(mountainsMinWidth - 2) + 1.0f) / (float)mountainsMinWidth;
      glNormal3f((float)(-M_SQRT1_2 * cosf(angle)), (float)(-M_SQRT1_2 * sinf(angle)), (float)M_SQRT1_2);
      glTexCoord2f(frac, 0.02f);
      glVertex3f(2.25f * worldSize * cosf(angle), 2.25f * worldSize * sinf(angle), 0.0f);
      glTexCoord2f(frac, 0.99f);
      glVertex3f(2.25f * worldSize * cosf(angle), 2.25f * worldSize * sinf(angle), 0.45f * worldSize * hightScale);
    }
  } glEnd();

  glBegin(GL_TRIANGLE_STRIP); {
    for (i = 0; i <= numFacesPerTexture; i++) {
      const float angle = (float)(M_PI + angleScale * (double)(i + n));
      float frac = (float)i / (float)numFacesPerTexture;

      if (numMountainTextures != 1)
	frac = (frac * (float)(mountainsMinWidth - 2) + 1.0f) / (float)mountainsMinWidth;

      glNormal3f((float)(-M_SQRT1_2 * cosf(angle)), (float)(-M_SQRT1_2 * sinf(angle)), (float)M_SQRT1_2);
      glTexCoord2f(frac, 0.02f);
      glVertex3f(2.25f * worldSize * cosf(angle), 2.25f * worldSize * sinf(angle),  0.0f);
      glTexCoord2f(frac, 0.99f);
      glVertex3f(2.25f * worldSize * cosf(angle), 2.25f * worldSize * sinf(angle), 0.45f * worldSize*hightScale);
    }
  } glEnd();
}


void BackgroundRenderer::setSkyColors()
{
  // change sky colors according to the sun position
  fvec4 colors[4];
  Daylight::getSkyColor(sunDirection, colors);
  skyZenithColor      = colors[0];
  skySunDirColor      = colors[1];
  skyAntiSunDirColor  = colors[2];
  skyCrossSunDirColor = colors[3];
  return;
}


void BackgroundRenderer::buildGeometry(GLDisplayList displayList)
{
  if (!lastRenderer) {
    return;
  }

  // compute display list for moon
  float coverage = fvec3::dot(moonDirection, sunDirection);
  // hack coverage to lean towards full
  coverage = (coverage < 0.0f) ? -sqrtf(-coverage) : coverage * coverage;
  const float worldSize = BZDBCache::worldSize;
  const float moonRadius = 2.0f * worldSize * atanf(1.0f * DEG2RADf);
  // limbAngle is dependent on moon position but sun is so much farther
  // away that the moon's position is negligible.  rotate sun and moon
  // so that moon is on the horizon in the +x direction, then compute
  // the angle to the sun position in the yz plane.
  fvec3 sun2;
  const float moonAzimuth = atan2f(moonDirection.y, moonDirection.x);
  const float moonAltitude = asinf(moonDirection.z);
  sun2.x = sunDirection.x * cosf(moonAzimuth) + sunDirection.y * sinf(moonAzimuth);
  sun2.y = sunDirection.y * cosf(moonAzimuth) - sunDirection.x * sinf(moonAzimuth);
  sun2.z = sunDirection.z * cosf(moonAltitude) - sun2.x * sinf(moonAltitude);
  const float limbAngle = atan2f(sun2.z, sun2.y);
  const float sunRadius = 2.0f * worldSize * atanf(1.0f * DEG2RADf);

  const float gameSize   = 0.5f  * worldSize;
  const float groundSize = 10.0f * worldSize;
  fvec3 gameArea[4];
  fvec3 groundPlane[4];
  for (int i = 0; i < 4; i++) {
    gameArea[i]    = gameSize   * squareShape[i];
    groundPlane[i] = groundSize * squareShape[i];
  }

  float xmin, xmax;
  float ymin, ymax;
  float xdist, ydist;
  float xtexmin, xtexmax;
  float ytexmin, ytexmax;
  float xtexdist, ytexdist;
  fvec2 vec;

#define GROUND_DIVS	(4)	//FIXME -- seems to be enough

  xmax = groundPlane[0].x;
  ymax = groundPlane[0].y;
  xmin = groundPlane[2].x;
  ymin = groundPlane[2].y;
  xdist = (xmax - xmin) / (float)GROUND_DIVS;
  ydist = (ymax - ymin) / (float)GROUND_DIVS;

  lastRenderer->getGroundUV(groundPlane[0].xy(), vec);
  xtexmax = vec[0];
  ytexmax = vec[1];
  lastRenderer->getGroundUV(groundPlane[2].xy(), vec);
  xtexmin = vec[0];
  ytexmin = vec[1];
  xtexdist = (xtexmax - xtexmin) / (float)GROUND_DIVS;
  ytexdist = (ytexmax - ytexmin) / (float)GROUND_DIVS;

  fvec3 cloudsOuter[4];
  fvec3 cloudsInner[4];
  const float uvScale = 0.25f;
  const float cloudHeightMult = BZDB.eval("_cloudHeightMult");
  for (int i = 0; i < 4; i++) {
    cloudsOuter[i] = groundPlane[i];
    cloudsOuter[i].z += (cloudHeightMult * BZDBCache::tankHeight);
    cloudsInner[i].xy() = uvScale * cloudsOuter[i].xy();
    cloudsInner[i].z = cloudsOuter[i].z;
  }

  // make cloud display list.  RIVA 128 doesn't interpolate alpha,
  // so on that system use full alpha everywhere.
  float minAlpha = 0.0f;

  if (displayList == sunXFormList) {
    glPushMatrix();
    glRotatef((float)(atan2f(sunDirection[1], (sunDirection[0])) * 180.0 / M_PI),
	      0.0f, 0.0f, 1.0f);
    glRotatef((float)(asinf(sunDirection[2]) * 180.0 / M_PI), 0.0f, -1.0f, 0.0f);

    glBegin(GL_TRIANGLE_FAN); {
      glVertex3f(2.0f * worldSize, 0.0f, 0.0f);
      for (int i = 0; i < 20; i++) {
	const float angle = (float)(2.0 * M_PI * double(i) / 19.0);
	glVertex3f(2.0f * worldSize, sunRadius * sinf(angle),
		   sunRadius * cosf(angle));
      }
    } glEnd();

    glPopMatrix();
  }
  else if (displayList == moonList) {
    int moonSegements = BZDB.evalInt("moonSegments");

    glPushMatrix();
    glRotatef((float)(atan2f(moonDirection[1], moonDirection[0]) * RAD2DEG), 0.0f, 0.0f, 1.0f);
    glRotatef((float)(asinf(moonDirection[2]) * RAD2DEG), 0.0f, -1.0f, 0.0f);
    glRotatef((float)(limbAngle * RAD2DEG), 1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP); {
      // glTexCoord2f(0,-1);
      glVertex3f(2.0f * worldSize, 0.0f, -moonRadius);
      for (int i = 0; i < moonSegements-1; i++) {
	const float angle = (float)(0.5 * M_PI * double(i-(moonSegements/2)-1) / (moonSegements/2.0));
	float sinAngle = sinf(angle);
	float cosAngle = cosf(angle);
	// glTexCoord2f(coverage*cosAngle,sinAngle);
	glVertex3f(2.0f * worldSize, coverage * moonRadius * cosAngle,moonRadius * sinAngle);

	// glTexCoord2f(cosAngle,sinAngle);
	glVertex3f(2.0f * worldSize, moonRadius * cosAngle,moonRadius * sinAngle);
      }
      // glTexCoord2f(0,1);
      glVertex3f(2.0f * worldSize, 0.0f, moonRadius);
    } glEnd();
    glPopMatrix();
  }
  else if (displayList == starXFormList) {
    // make pretransformed display list for stars
    glPushMatrix();
    glMultMatrixf(lastRenderer->getCelestialTransform());
    glScalef(worldSize, worldSize, worldSize);

    glBegin(GL_POINTS); {
      for (int i = 0; i < (int)NumStars; i++) {
	glColor3fv(stars[i]);
	glVertex3fv(stars[i] + 3);
      }
    } glEnd();

    glPopMatrix();
  }
  else if (displayList == mediumGroundList) {
    for (int i = 0; i < GROUND_DIVS; i++) {
      float yoff, ytexoff;

      yoff = ymin + ydist * (float)i;
      ytexoff = ytexmin + ytexdist * (float)i;

      glBegin(GL_TRIANGLE_STRIP); {

	glTexCoord2f(xtexmin, ytexoff + ytexdist);
	glVertex2f(xmin, yoff + ydist);
	glTexCoord2f(xtexmin, ytexoff);
	glVertex2f(xmin, yoff);

	for (int j = 0; j < GROUND_DIVS; j++) {
	  float xoff, xtexoff;

	  xoff = xmin + xdist * (float)(j + 1);
	  xtexoff = xtexmin + xtexdist * (float)(j + 1);

	  glTexCoord2f(xtexoff, ytexoff + ytexdist);
	  glVertex2f(xoff, yoff + ydist);
	  glTexCoord2f(xtexoff, ytexoff);
	  glVertex2f(xoff, yoff);
	}
      } glEnd();
    }
  }
  else if (displayList == lowGroundList) {
    glBegin(GL_TRIANGLE_STRIP); {
      glVertex2fv(groundPlane[0]);
      glVertex2fv(groundPlane[1]);
      glVertex2fv(groundPlane[3]);
      glVertex2fv(groundPlane[2]);
    } glEnd();
  }
  else if (displayList == cloudsList) {
    glNormal3f(0.0f, 0.0f, 1.0f);
    // inner clouds -- full opacity
    glBegin(GL_QUADS); {
      const float scale = uvScale * cloudRepeats;
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glTexCoord2fv(scale * squareShape[3].xy()); glVertex3fv(cloudsInner[3]);
      glTexCoord2fv(scale * squareShape[2].xy()); glVertex3fv(cloudsInner[2]);
      glTexCoord2fv(scale * squareShape[1].xy()); glVertex3fv(cloudsInner[1]);
      glTexCoord2fv(scale * squareShape[0].xy()); glVertex3fv(cloudsInner[0]);
    }
    glEnd();

    // outer clouds -- fade to zero opacity at outer edge
    glBegin(GL_TRIANGLE_STRIP); {
      const fvec4 opaque(1.0f, 1.0f, 1.0f, 1.0f);
      const fvec4 faded(1.0f, 1.0f, 1.0f, minAlpha);
      const float cr   = cloudRepeats;
      const float crUV = cloudRepeats * uvScale;
      const fvec3* ss = squareShape;
      const fvec3* co = cloudsOuter;
      const fvec3* ci = cloudsInner;
      glColor4fv(faded);  glTexCoord2fv(cr   * ss[1].xy()); glVertex3fv(co[1]);
      glColor4fv(opaque); glTexCoord2fv(crUV * ss[1].xy()); glVertex3fv(ci[1]);
      glColor4fv(faded);  glTexCoord2fv(cr   * ss[2].xy()); glVertex3fv(co[2]);
      glColor4fv(opaque); glTexCoord2fv(crUV * ss[2].xy()); glVertex3fv(ci[2]);
      glColor4fv(faded);  glTexCoord2fv(cr   * ss[3].xy()); glVertex3fv(co[3]);
      glColor4fv(opaque); glTexCoord2fv(crUV * ss[3].xy()); glVertex3fv(ci[3]);
      glColor4fv(faded);  glTexCoord2fv(cr   * ss[0].xy()); glVertex3fv(co[0]);
      glColor4fv(opaque); glTexCoord2fv(crUV * ss[0].xy()); glVertex3fv(ci[0]);
      glColor4fv(faded);  glTexCoord2fv(cr   * ss[1].xy()); glVertex3fv(co[1]);
      glColor4fv(opaque); glTexCoord2fv(crUV * ss[1].xy()); glVertex3fv(ci[1]);
    }
    glEnd();
  }
  else {
    // check for mountans
    for (unsigned int i = 0; i < (unsigned int)mountanLists.size(); i++) {
      if (displayList == mountanLists[i]) {
	buildMountain(i);
      }
    }
  }
}


void BackgroundRenderer::makeCelestialLists(const SceneRenderer& renderer)
{
  setSkyColors();

  lastRenderer = (SceneRenderer*)&renderer;

  // get a few other things concerning the sky
  doShadows = Daylight::areShadowsCast(sunDirection);
  doStars   = Daylight::areStarsVisible(sunDirection);
  doSunset  = Daylight::getSunsetTop(sunDirection, sunsetTop);

  // make pretransformed display list for sun
  DisplayListSystem &ds = DisplayListSystem::Instance();

  ds.freeList(sunXFormList);
  sunXFormList = ds.newList(this);

  ds.freeList(moonList);
  moonList = ds.newList(this);

  ds.freeList(starXFormList);
  starXFormList = ds.newList(this);

  return;
}


void BackgroundRenderer::addCloudDrift(float uDrift, float vDrift)
{
  cloudDriftU += 0.01f * uDrift / cloudRepeats;
  cloudDriftV += 0.01f * vDrift / cloudRepeats;
  if (cloudDriftU > 1.0f) cloudDriftU -= 1.0f;
  else if (cloudDriftU < 0.0f) cloudDriftU += 1.0f;
  if (cloudDriftV > 1.0f) cloudDriftV -= 1.0f;
  else if (cloudDriftV < 0.0f) cloudDriftV += 1.0f;
}


void BackgroundRenderer::renderSky(SceneRenderer& renderer, bool fullWindow,
				   bool mirror)
{
  if (!BZDBCache::drawSky) {
    return;
  }
  if (renderer.useQuality() > _LOW_QUALITY) {
    drawSky(renderer, mirror);
  } else {
    // low detail -- draw as damn fast as ya can, ie cheat.  use glClear()
    // to draw solid color sky and ground.
    MainWindow& window = renderer.getWindow();
    const int x = window.getOriginX();
    const int y = window.getOriginY();
    const int width = window.getWidth();
    const int height = window.getHeight();
    const int viewHeight = window.getViewHeight();
    const SceneRenderer::ViewType viewType = renderer.getViewType();

    // draw sky
    glDisable(GL_DITHER);
    glPushAttrib(GL_SCISSOR_BIT);
    glScissor(x, y + height - (viewHeight >> 1), width, (viewHeight >> 1));
    glClearColor(skyZenithColor[0], skyZenithColor[1], skyZenithColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw ground -- first get the color (assume it's all green)
    float _groundColor = 0.1f + 0.15f * renderer.getSunColor()[1];
    if (fullWindow && viewType == SceneRenderer::ThreeChannel)
      glScissor(x, y, width, height >> 1);
    else if (fullWindow && viewType == SceneRenderer::Stacked)
      glScissor(x, y, width, height >> 1);
#ifndef USE_GL_STEREO
    else if (fullWindow && viewType == SceneRenderer::Stereo)
      glScissor(x, y, width, height >> 1);
#endif
    else
      glScissor(x, y + height - viewHeight, width, (viewHeight + 1) >> 1);
    if (invert)
      glClearColor(_groundColor, 0.0f, _groundColor, 0.0f);
    else
      glClearColor(0.0f, _groundColor, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // back to normal
    glPopAttrib();
    if (BZDB.isTrue("dither")) {
      glEnable(GL_DITHER);
    }
  }
}


void BackgroundRenderer::renderGround(SceneRenderer& renderer, bool fullWindow)
{
  if (renderer.useQuality() > _LOW_QUALITY) {
    drawGround();
    return;
  }

  // low detail -- draw as damn fast as ya can, ie cheat.  use glClear()
  // to draw solid color sky and ground.
  MainWindow& window = renderer.getWindow();
  const int x = window.getOriginX();
  const int y = window.getOriginY();
  const int width = window.getWidth();
  const int height = window.getHeight();
  const int viewHeight = window.getViewHeight();
  const SceneRenderer::ViewType viewType = renderer.getViewType();

  // draw sky
  glDisable(GL_DITHER);
  glPushAttrib(GL_SCISSOR_BIT);
  glScissor(x, y + height - (viewHeight >> 1), width, (viewHeight >> 1));
  glClearColor(skyZenithColor[0], skyZenithColor[1], skyZenithColor[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // draw ground -- first get the color (assume it's all green)
  float _groundColor = 0.1f + 0.15f * renderer.getSunColor()[1];
  if (fullWindow && viewType == SceneRenderer::ThreeChannel) {
    glScissor(x, y, width, height >> 1);
  }
  else if (fullWindow && viewType == SceneRenderer::Stacked) {
    glScissor(x, y, width, height >> 1);
  }
#ifndef USE_GL_STEREO
  else if (fullWindow && viewType == SceneRenderer::Stereo) {
    glScissor(x, y, width, height >> 1);
  }
#endif
  else {
    glScissor(x, y + height - viewHeight, width, (viewHeight + 1) >> 1);
  }

  if (invert) {
    glClearColor(_groundColor, 0.0f, _groundColor, 0.0f);
  } else {
    glClearColor(0.0f, _groundColor, 0.0f, 0.0f);
  }

  glClear(GL_COLOR_BUFFER_BIT);

  // back to normal
  glPopAttrib();
  if (BZDB.isTrue("dither")) {
    glEnable(GL_DITHER);
  }
}


void BackgroundRenderer::renderGroundEffects(SceneRenderer& renderer,
					     bool drawingMirror)
{
  // zbuffer should be disabled.  either everything is coplanar with
  // the ground or is drawn back to front and is occluded by everything
  // drawn after it.  also use projection with very far clipping plane.

  // only draw the grid lines if texturing is disabled
  if (!BZDBCache::texture || (renderer.useQuality() <= _LOW_QUALITY)) {
    drawGroundGrid(renderer);
  }

  if (!blank) {
    if (doShadows && shadowsVisible && !drawingMirror) {
      drawGroundShadows(renderer);
    }

    // draw light receivers on ground (little meshes under light sources so
    // the ground gets illuminated).  this is necessary because lighting is
    // performed only at a vertex, and the ground's vertices are a few
    // kilometers away.
    if (BZDBCache::blend && BZDBCache::lighting &&
	!drawingMirror && BZDBCache::drawGroundLights) {
      if (BZDBCache::tesselation && (renderer.useQuality() >= _HIGH_QUALITY)) {
	//	  (BZDB.get(StateDatabase::BZDB_FOGMODE) == "none")) {
	// not really tesselation, but it is tied to the "Best" lighting,
	// avoid on foggy maps, because the blending function accumulates
	// too much brightness.
	drawAdvancedGroundReceivers(renderer);
      } else {
	drawGroundReceivers(renderer);
      }
    }
  }

  if (renderer.useQuality() > _MEDIUM_QUALITY) {
    // light the mountains (so that they get dark when the sun goes down).
    // don't do zbuffer test since they occlude all drawn before them and
    // are occluded by all drawn after.
    if (mountainsVisible && BZDBCache::drawMountains) {
      drawMountains();
    }

    // draw clouds
    if (cloudsVisible && BZDBCache::drawClouds) {
      cloudsGState.setState();
      glMatrixMode(GL_TEXTURE);
      glPushMatrix();
      glTranslatef(cloudDriftU, cloudDriftV, 0.0f);
      DisplayListSystem::Instance().callList(cloudsList);
      glLoadIdentity();	// maybe works around bug in some systems
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
    }
  }
}


void BackgroundRenderer::renderEnvironment(SceneRenderer& renderer, bool update)
{
  if (blank) {
    return;
  }

  if (update) {
    weather.update();
  }
  weather.draw(renderer);

  if (update) {
    EFFECTS.update();
  }
  EFFECTS.draw(renderer);
}


void BackgroundRenderer::resizeSky() {
  // sky pyramid must fit inside far clipping plane
  // (adjusted for the deepProjection matrix)
  const float skySize = 3.0f * BZDBCache::worldSize;
  for (int i = 0; i < 4; i++) {
    skyPyramid[i] = skySize * squareShape[i];
  }
  skyPyramid[4][0] = 0.0f;
  skyPyramid[4][1] = 0.0f;
  skyPyramid[4][2] = skySize;
}


void BackgroundRenderer::setupSkybox()
{
  haveSkybox = false;

  int i;
  const char *(skyboxNames[6]) = {
    "LeftSkyboxMaterial",
    "FrontSkyboxMaterial",
    "RightSkyboxMaterial",
    "BackSkyboxMaterial",
    "TopSkyboxMaterial",
    "BottomSkyboxMaterial"
  };
  TextureManager& tm = TextureManager::instance();
  const BzMaterial* bzmats[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

  // try to load the textures
  for (i = 0; i < 6; i++) {
    bzmats[i] = MATERIALMGR.findMaterial(skyboxNames[i]);
    if ((bzmats[i] == NULL) || (bzmats[i]->getTextureCount() <= 0)) {
      break;
    }
    skyboxTexID[i] = tm.getTextureID(bzmats[i]->getTextureLocal(0));
    if (skyboxTexID[i] < 0) {
      break;
    }
  }

  // unload textures if they were not all successful
  if (i != 6) {
    while (i >= 0) {
      if ((bzmats[i] != NULL) && (bzmats[i]->getTextureCount() > 0)) {
	// NOTE: this could delete textures the might be used elsewhere
	tm.removeTexture(bzmats[i]->getTextureLocal(0));
      }
      i--;
    }
    return;
  }

  // reference map specified materials
  for (i = 0; i < 6; i++) {
    ((BzMaterial*)bzmats[i])->setReference();
  }

  // setup the wrap mode
  skyboxWrapMode = GL_CLAMP_TO_EDGE;

  // setup the corner colors
  const int cornerFaces[8][3] = {
    {5, 0, 1}, {5, 1, 2}, {5, 2, 3}, {5, 3, 0},
    {4, 0, 1}, {4, 1, 2}, {4, 2, 3}, {4, 3, 0}
  };
  for (i = 0; i < 8; i++) {
    for (int c = 0; c < 4; c++) {
      skyboxColor[i][c] = 0.0f;
      for (int f = 0; f < 3; f++) {
	skyboxColor[i][c] += bzmats[cornerFaces[i][f]]->getDiffuse()[c];
      }
      skyboxColor[i][c] /= 3.0f;
    }
  }

  haveSkybox = true;

  return;
}


void BackgroundRenderer::drawSkybox()
{
  // sky box must fit inside far clipping plane
  // (adjusted for the deepProjection matrix)
  const float d = 3.0f * BZDBCache::worldSize;
  const float verts[8][3] = {
    {-d, -d, -d}, {+d, -d, -d}, {+d, +d, -d}, {-d, +d, -d},
    {-d, -d, +d}, {+d, -d, +d}, {+d, +d, +d}, {-d, +d, +d}
  };
  const float txcds[4][2] = {
    {1.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 1.0f}, {1.0f, 1.0f}
  };

  TextureManager& tm = TextureManager::instance();

  OpenGLGState::resetState();

  const fvec4* color = skyboxColor;

  glEnable(GL_TEXTURE_2D);
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);

  if (!BZDBCache::drawGround) {
    tm.bind(skyboxTexID[5]); // bottom
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, skyboxWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, skyboxWrapMode);
    glBegin(GL_QUADS); {
      glTexCoord2fv(txcds[0]); glColor3fv(color[2]); glVertex3fv(verts[2]);
      glTexCoord2fv(txcds[1]); glColor3fv(color[3]); glVertex3fv(verts[3]);
      glTexCoord2fv(txcds[2]); glColor3fv(color[0]); glVertex3fv(verts[0]);
      glTexCoord2fv(txcds[3]); glColor3fv(color[1]); glVertex3fv(verts[1]);
    } glEnd();
  }

  tm.bind(skyboxTexID[4]); // top
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, skyboxWrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, skyboxWrapMode);
  glBegin(GL_QUADS); {
    glTexCoord2fv(txcds[0]); glColor3fv(color[5]); glVertex3fv(verts[5]);
    glTexCoord2fv(txcds[1]); glColor3fv(color[4]); glVertex3fv(verts[4]);
    glTexCoord2fv(txcds[2]); glColor3fv(color[7]); glVertex3fv(verts[7]);
    glTexCoord2fv(txcds[3]); glColor3fv(color[6]); glVertex3fv(verts[6]);
  } glEnd();

  tm.bind(skyboxTexID[0]); // left
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, skyboxWrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, skyboxWrapMode);
  glBegin(GL_QUADS); {
    glTexCoord2fv(txcds[0]); glColor3fv(color[0]); glVertex3fv(verts[0]);
    glTexCoord2fv(txcds[1]); glColor3fv(color[3]); glVertex3fv(verts[3]);
    glTexCoord2fv(txcds[2]); glColor3fv(color[7]); glVertex3fv(verts[7]);
    glTexCoord2fv(txcds[3]); glColor3fv(color[4]); glVertex3fv(verts[4]);
  } glEnd();

  tm.bind(skyboxTexID[1]); // front
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, skyboxWrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, skyboxWrapMode);
  glBegin(GL_QUADS); {
    glTexCoord2fv(txcds[0]); glColor3fv(color[1]); glVertex3fv(verts[1]);
    glTexCoord2fv(txcds[1]); glColor3fv(color[0]); glVertex3fv(verts[0]);
    glTexCoord2fv(txcds[2]); glColor3fv(color[4]); glVertex3fv(verts[4]);
    glTexCoord2fv(txcds[3]); glColor3fv(color[5]); glVertex3fv(verts[5]);
  } glEnd();

  tm.bind(skyboxTexID[2]); // right
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, skyboxWrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, skyboxWrapMode);
  glBegin(GL_QUADS); {
    glTexCoord2fv(txcds[0]); glColor3fv(color[2]); glVertex3fv(verts[2]);
    glTexCoord2fv(txcds[1]); glColor3fv(color[1]); glVertex3fv(verts[1]);
    glTexCoord2fv(txcds[2]); glColor3fv(color[5]); glVertex3fv(verts[5]);
    glTexCoord2fv(txcds[3]); glColor3fv(color[6]); glVertex3fv(verts[6]);
  } glEnd();

  tm.bind(skyboxTexID[3]); // back
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, skyboxWrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, skyboxWrapMode);
  glBegin(GL_QUADS); {
    glTexCoord2fv(txcds[0]); glColor3fv(color[3]); glVertex3fv(verts[3]);
    glTexCoord2fv(txcds[1]); glColor3fv(color[2]); glVertex3fv(verts[2]);
    glTexCoord2fv(txcds[2]); glColor3fv(color[6]); glVertex3fv(verts[6]);
    glTexCoord2fv(txcds[3]); glColor3fv(color[7]); glVertex3fv(verts[7]);
  } glEnd();

  glShadeModel(GL_FLAT);
  glEnable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
}


void BackgroundRenderer::drawSky(SceneRenderer& renderer, bool mirror)
{
  glPushMatrix();

  const bool doSkybox = haveSkybox && (renderer.useQuality() >= _HIGH_QUALITY);

  if (!doSkybox) {
    // rotate sky so that horizon-point-toward-sun-color is actually
    // toward the sun
    glRotatef((float)((atan2f(sunDirection[1], sunDirection[0]) * 180.0 + 135.0) / M_PI),
	      0.0f, 0.0f, 1.0f);

    // draw sky
    skyGState.setState();
    if (!doSunset) {
      // just a pyramid
      glBegin(GL_TRIANGLE_FAN); {
	glColor3fv(skyZenithColor);      glVertex3fv(skyPyramid[4]);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[0]);
	glColor3fv(skySunDirColor);      glVertex3fv(skyPyramid[3]);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[2]);
	glColor3fv(skyAntiSunDirColor);  glVertex3fv(skyPyramid[1]);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[0]);
      } glEnd();
    } else {
      // overall shape is a pyramid, but the solar sides are two
      // triangles each.  the top triangle is all zenith color,
      // the bottom goes from zenith to sun-dir color.
      glBegin(GL_TRIANGLE_FAN); {
	glColor3fv(skyZenithColor);      glVertex3fv(skyPyramid[4]);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[2]);
	glColor3fv(skyAntiSunDirColor);  glVertex3fv(skyPyramid[1]);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[0]);
      } glEnd();

      float sunsetTopPoint[3];
      sunsetTopPoint[0] = skyPyramid[3][0] * (1.0f - sunsetTop);
      sunsetTopPoint[1] = skyPyramid[3][1] * (1.0f - sunsetTop);
      sunsetTopPoint[2] = skyPyramid[4][2] * sunsetTop;
      glBegin(GL_TRIANGLES); {
	glColor3fv(skyZenithColor);      glVertex3fv(skyPyramid[4]);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[0]);
	glColor3fv(skyZenithColor);      glVertex3fv(sunsetTopPoint);
	                                 glVertex3fv(skyPyramid[4]);
	                                 glVertex3fv(sunsetTopPoint);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[2]);
	glColor3fv(skyZenithColor);      glVertex3fv(sunsetTopPoint);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[0]);
	glColor3fv(skySunDirColor);      glVertex3fv(skyPyramid[3]);
	glColor3fv(skyCrossSunDirColor); glVertex3fv(skyPyramid[2]);
	glColor3fv(skyZenithColor);      glVertex3fv(sunsetTopPoint);
	glColor3fv(skySunDirColor);      glVertex3fv(skyPyramid[3]);
      } glEnd();
    }
  }


  glLoadIdentity();
  renderer.getViewFrustum().executeOrientation();

  const bool drawCelestial = BZDBCache::drawCelestial;
  const bool useClipPlane = (mirror && (doSkybox || drawCelestial));

  if (useClipPlane) {
    glEnable(GL_CLIP_PLANE0);
    const GLdouble plane[4] = {0.0, 0.0, +1.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0, plane);
  }

  if (doSkybox) {
    drawSkybox();
  }

  DisplayListSystem &ds = DisplayListSystem::Instance();

  if (drawCelestial) {
    if (sunDirection[2] > -0.009f) {
      sunGState.setState();
      glColor3fv(renderer.getSunScaledColor());

      ds.callList(sunXFormList);
    }

    if (doStars) {
      starGState[starGStateIndex].setState();
      ds.callList(starXFormList);
    }

    if (moonDirection[2] > -0.009f) {
      glColor3f(1.0f, 1.0f, 1.0f);
      ds.callList(moonList);
    }
  }


  if (useClipPlane) {
    glDisable(GL_CLIP_PLANE0);
  }


  glPopMatrix();
}


void BackgroundRenderer::drawGround()
{
  if (!BZDBCache::drawGround) {
    return;
  }
  // draw ground
  glNormal3f(0.0f, 0.0f, 1.0f);
  if (invert) {
    glColor4fv(groundColorInv[styleIndex]);
    invGroundGState[styleIndex].setState();
  } else {
    fvec4 color;
    if (BZDB.isSet("groundOverrideColor") &&
        parseColorString(BZDB.get("groundOverrideColor"), color)) {
      glColor4fv(color);
    } else {
      glColor4fv(groundColor[styleIndex]);
    }
    groundGState[styleIndex].setState();
  }

  if (RENDERER.useQuality() >= _HIGH_QUALITY) {
    drawGroundCentered();
  }
  else if (RENDERER.useQuality() == _LOW_QUALITY) {
    DisplayListSystem::Instance().callList(lowGroundList);
  }
  else {
    DisplayListSystem::Instance().callList(mediumGroundList);
  }
}


void BackgroundRenderer::drawGroundCentered()
{
  const float groundSize = 10.0f * BZDBCache::worldSize;
  const float centerSize = 128.0f;

  const ViewFrustum& frustum = RENDERER.getViewFrustum();
  fvec2 center = frustum.getEye().xy();
  const float minDist = -groundSize + centerSize;
  const float maxDist = +groundSize - centerSize;
  if (center.x < minDist) { center.x = minDist; }
  if (center.x > maxDist) { center.x = maxDist; }
  if (center.y < minDist) { center.y = minDist; }
  if (center.y > maxDist) { center.y = maxDist; }

  const fvec2 vertices[8] = {
    fvec2(-groundSize, -groundSize),
    fvec2(+groundSize, -groundSize),
    fvec2(+groundSize, +groundSize),
    fvec2(-groundSize, +groundSize),
    fvec2(center.x - centerSize, center.y - centerSize),
    fvec2(center.x + centerSize, center.y - centerSize),
    fvec2(center.x + centerSize, center.y + centerSize),
    fvec2(center.x - centerSize, center.y + centerSize),
  };

  const float repeat = BZDB.eval("groundHighResTexRepeat");
  const int indices[5][4] = {
    { 4, 5, 6, 7 },
    { 0, 1, 5, 4 },
    { 1, 2, 6, 5 },
    { 2, 3, 7, 6 },
    { 3, 0, 4, 7 },
  };

  glNormal3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_QUADS);
  {
    for (int q = 0; q < 5; q++) {
      for (int c = 0; c < 4; c++) {
        const int index = indices[q][c];
        glTexCoord2fv(vertices[index] * repeat);
        glVertex2fv(vertices[index]);
      }
    }
  }
  glEnd();

  return;
}


void BackgroundRenderer::drawGroundGrid(SceneRenderer& renderer)
{
  const fvec3& pos = renderer.getViewFrustum().getEye();
  const float xhalf = gridSpacing * (gridCount + floorf(pos[2] / 4.0f));
  const float yhalf = gridSpacing * (gridCount + floorf(pos[2] / 4.0f));
  const float x0 = floorf(pos[0] / gridSpacing) * gridSpacing;
  const float y0 = floorf(pos[1] / gridSpacing) * gridSpacing;
  float i;

  gridGState.setState();

  // x lines
  if (doShadows) glColor3f(0.0f, 0.75f, 0.5f);
  else glColor3f(0.0f, 0.4f, 0.3f);
  glBegin(GL_LINES); {
    for (i = -xhalf; i <= xhalf; i += gridSpacing) {
      glVertex2f(x0 + i, y0 - yhalf);
      glVertex2f(x0 + i, y0 + yhalf);
    }
  } glEnd();

  // z lines
  if (doShadows) glColor3f(0.5f, 0.75f, 0.0f);
  else glColor3f(0.3f, 0.4f, 0.0f);
  glBegin(GL_LINES); {
    for (i = -yhalf; i <= yhalf; i += gridSpacing) {
      glVertex2f(x0 - xhalf, y0 + i);
      glVertex2f(x0 + xhalf, y0 + i);
    }
  } glEnd();
}


void BackgroundRenderer::multShadowMatrix() const
{
  // draw sun shadows -- always stippled so overlapping shadows don't
  // accumulate darkness.  make and multiply by shadow projection matrix.
  static float shadowProjection[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    8888, 9999, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
  };
  shadowProjection[8] = -sunDirection[0] / sunDirection[2];
  shadowProjection[9] = -sunDirection[1] / sunDirection[2];
  glMultMatrixf(shadowProjection);
}


void BackgroundRenderer::drawGroundShadows(SceneRenderer& renderer)
{
  // draw sun shadows -- always stippled so overlapping shadows don't
  // accumulate darkness.  make and multiply by shadow projection matrix.
  multShadowMatrix();

  // disable color updates
  SceneNode::setColorOverride(true);

  // disable the unused arrays
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  if (!BZDBCache::stencilShadows) {
    // use stippling to avoid overlapping shadows
    sunShadowsGState.setState();
    glColor3f(0.0f, 0.0f, 0.0f);
  }
  else { // use the stencil to avoid overlapping shadows
    // setup blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, BZDBCache::shadowAlpha);
    if (GLEW_ARB_imaging) {
      // better for alpha-thresh input fragments
      glBlendColor(0.0f, 0.0f, 0.0f, BZDBCache::shadowAlpha);
      glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    }

    // setup the stencil
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_NOTEQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_STENCIL_TEST);

    // setup culling
    glDisable(GL_CULL_FACE);
  }

  // render those nodes
  renderer.getShadowList().render();

  // revert to OpenGLGState defaults
  if (BZDBCache::stencilShadows) {
    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
  }

  // enable color updates
  SceneNode::setColorOverride(false);

  OpenGLGState::resetState();

  // re-enable the arrays
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glLoadIdentity();
  renderer.getViewFrustum().executeView();
}


void BackgroundRenderer::drawGroundReceivers(SceneRenderer& renderer)
{
  static const int receiverRings = 4;
  static const int receiverSlices = 8;
  static const float receiverRingSize = 1.2f;	// meters
  static float angle[receiverSlices + 1][2];

  static bool init = false;
  if (!init) {
    init = true;
    const float receiverSliceAngle = (float)(2.0 * M_PI / double(receiverSlices));
    for (int i = 0; i <= receiverSlices; i++) {
      angle[i][0] = cosf((float)i * receiverSliceAngle);
      angle[i][1] = sinf((float)i * receiverSliceAngle);
    }
  }

  const int count = renderer.getNumAllLights();
  if (count == 0) {
    return;
  }

  // bright sun dims intensity of ground receivers
  const float B = 1.0f - (0.6f * renderer.getSunBrightness());

  receiverGState.setState();

  glPushMatrix();
  int i, j;
  for (int k = 0; k < count; k++) {
    const OpenGLLight& light = renderer.getLight(k);
    if (light.getOnlyReal()) {
      continue;
    }

    const fvec4& pos = light.getPosition();
    const fvec4& lightColor = light.getColor();
    const fvec3& atten = light.getAttenuation();

    // point under light
    float d = pos[2];
    float I = B / (atten[0] + d * (atten[1] + d * atten[2]));

    // maximum value
    const float maxVal =
      (lightColor.r > lightColor.g) ?
       ((lightColor.r > lightColor.b) ? lightColor.r : lightColor.b) :
       ((lightColor.g > lightColor.b) ? lightColor.g : lightColor.b);

    // if I is too attenuated, don't bother drawing anything
    if ((I * maxVal) < 0.02f) {
      continue;
    }

    // move to the light's position
    glTranslatef(pos.x, pos.y, 0.0f);

    // set the main lighting color
    fvec4 color(lightColor.rgb(), I);

    // draw ground receiver, computing lighting at each vertex ourselves
    glBegin(GL_TRIANGLE_FAN); {
      glColor4fv(color);
      glVertex2f(0.0f, 0.0f);

      // inner ring
      d = hypotf(receiverRingSize, pos[2]);
      I = B / (atten[0] + d * (atten[1] + d * atten[2]));
      I *= pos[2] / d;
      color.a = I;
      glColor4fv(color);
      for (j = 0; j <= receiverSlices; j++) {
	glVertex2f(receiverRingSize * angle[j][0],
		   receiverRingSize * angle[j][1]);
      }
    } glEnd();
    triangleCount += receiverSlices;

    for (i = 1; i < receiverRings; i++) {
      const float innerSize = receiverRingSize * float(i * i);
      const float outerSize = receiverRingSize * float((i + 1) * (i + 1));

      // compute inner and outer lit colors
      d = hypotf(innerSize, pos[2]);
      I = B / (atten[0] + d * (atten[1] + d * atten[2]));
      I *= pos[2] / d;
      float innerAlpha = I;

      if (i + 1 == receiverRings) {
	I = 0.0f;
      } else {
	d = hypotf(outerSize, pos[2]);
	I = B / (atten[0] + d * (atten[1] + d * atten[2]));
	I *= pos[2] / d;
      }
      float outerAlpha = I;

      glBegin(GL_QUAD_STRIP); {
	for (j = 0; j <= receiverSlices; j++) {
	  color.a = innerAlpha;
	  glColor4fv(color);
	  glVertex2f(angle[j][0] * innerSize, angle[j][1] * innerSize);
	  color.a = outerAlpha;
	  glColor4fv(color);
	  glVertex2f(angle[j][0] * outerSize, angle[j][1] * outerSize);
	}
      } glEnd();
    }
    triangleCount += (receiverSlices * receiverRings * 2);

    glTranslatef(-pos[0], -pos[1], 0.0f);
  }
  glPopMatrix();
}


void BackgroundRenderer::drawAdvancedGroundReceivers(SceneRenderer& renderer)
{
  const float minLuminance = 0.02f;
  static const int receiverSlices = 32;
  static const float receiverRingSize = 0.5f;	// meters
  static float angle[receiverSlices + 1][2];

  static bool init = false;
  if (!init) {
    init = true;
    const float receiverSliceAngle = (float)(2.0 * M_PI / double(receiverSlices));
    for (int i = 0; i <= receiverSlices; i++) {
      angle[i][0] = cosf((float)i * receiverSliceAngle);
      angle[i][1] = sinf((float)i * receiverSliceAngle);
    }
  }

  const int count = renderer.getNumAllLights();
  if (count == 0) {
    return;
  }

  // setup the ground tint
  const fvec4* gndColor = &groundColor[styleIndex];
  fvec4 overrideColor;
  if (BZDB.isSet("groundOverrideColor") &&
      parseColorString(BZDB.get("groundOverrideColor"), overrideColor)) {
    gndColor = &overrideColor;
  }

  const bool useTexture = BZDBCache::texture && (groundTextureID >= 0);
  OpenGLGState advGState;
  OpenGLGStateBuilder builder;
  builder.setShading(GL_SMOOTH);
  builder.setBlending((GLenum)GL_ONE, (GLenum)GL_ONE);
  if (useTexture) {
    builder.setTexture(groundTextureID);
    builder.setTextureMatrix(groundTextureMatrix);
  }
  advGState = builder.getState();
  advGState.setState();

  // lazy way to get texcoords
  if (useTexture) {
    const float repeat = BZDB.eval("groundHighResTexRepeat");
    const float sPlane[4] = { repeat, 0.0f, 0.0f, 0.0f };
    const float tPlane[4] = { 0.0f, repeat, 0.0f, 0.0f };
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, sPlane);
    glTexGenfv(GL_T, GL_EYE_PLANE, tPlane);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
  }

  glPushMatrix();
  int i, j;
  for (int k = 0; k < count; k++) {
    const OpenGLLight& light = renderer.getLight(k);
    if (light.getOnlyReal()) {
      continue;
    }

    // get the light parameters
    const fvec4& pos = light.getPosition();
    const fvec4& lightColor = light.getColor();
    const fvec3& atten = light.getAttenuation();

    // point under light
    float d = pos[2];
    float I = 1.0f / (atten[0] + d * (atten[1] + d * atten[2]));

    // set the main lighting color
    fvec3 baseColor = gndColor->rgb() * lightColor.rgb();
    if (invert) { // beats me, should just color logic op the static nodes
      baseColor = 1.0f - baseColor;
    }

    // maximum value
    const float maxVal =
       (baseColor.r > baseColor.g) ?
      ((baseColor.r > baseColor.b) ? baseColor.r : baseColor.b) :
      ((baseColor.g > baseColor.b) ? baseColor.g : baseColor.b);

    // if I is too attenuated, don't bother drawing anything
    if ((I * maxVal) < minLuminance) {
      continue;
    }

    // move to the light's position
    glTranslatef(pos[0], pos[1], 0.0f);

    float innerSize;
    fvec3 innerColor;
    float outerSize;
    fvec3 outerColor;

    // draw ground receiver, computing lighting at each vertex ourselves
    glBegin(GL_TRIANGLE_FAN); {
      // center point

      innerColor = I * baseColor;
      glColor3fv(innerColor);
      glVertex2f(0.0f, 0.0f);

      // inner ring
      d = hypotf(receiverRingSize, pos[2]);
      I = 1.0f / (atten[0] + d * (atten[1] + d * atten[2]));
      I *= pos[2] / d; // diffuse angle factor
      outerColor = I * baseColor;
      glColor3fv(outerColor);
      outerSize = receiverRingSize;
      for (j = 0; j <= receiverSlices; j++) {
	glVertex2f(outerSize * angle[j][0],
		   outerSize * angle[j][1]);
      }
    } glEnd();
    triangleCount += receiverSlices;

    bool moreRings = true;
    for (i = 2; moreRings; i++) {
      // inner ring
      innerSize = outerSize;
      memcpy(innerColor, outerColor, sizeof(float[3]));

      // outer ring
      outerSize = receiverRingSize * float(i * i);
      d = hypotf(outerSize, pos[2]);
      I = 1.0f / (atten[0] + d * (atten[1] + d * atten[2]));
      I *= pos[2] / d; // diffuse angle factor
      if ((I * maxVal) < minLuminance) {
	I = 0.0f;
	moreRings = false; // bail after this ring
      }
      outerColor = I * baseColor;

      glBegin(GL_QUAD_STRIP); {
	for (j = 0; j <= receiverSlices; j++) {
	  glColor3fv(innerColor);
	  glVertex2f(angle[j][0] * innerSize, angle[j][1] * innerSize);
	  glColor3fv(outerColor);
	  glVertex2f(angle[j][0] * outerSize, angle[j][1] * outerSize);
	}
      } glEnd();
    }
    triangleCount += (receiverSlices * 2 * (i - 2));

    glTranslatef(-pos[0], -pos[1], 0.0f);
  }
  glPopMatrix();

  if (useTexture) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
}


void BackgroundRenderer::drawMountains(void)
{
  glColor3f(1.0f, 1.0f, 1.0f);
  for (int i = 0; i < numMountainTextures; i++) {
    mountainsGState[i].setState();
    DisplayListSystem::Instance().callList(mountanLists[i]);
  }
}


const fvec3* BackgroundRenderer::getSunDirection() const
{
  if (Daylight::areShadowsCast(sunDirection)) {
    return &sunDirection;
  } else {
    return NULL;
  }
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
