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

/*
* EffectsRenderer:
 *	Encapsulates rendering of effects ( spawn flashes, sparks,
 *	explosions, etc...)
 */

#ifndef BZF_EFFECTS_RENDERER_H
#define BZF_EFFECTS_RENDERER_H

#include "common.h"

/* system headers */
#include <string>
#include <vector>

/* common interface headers */
#include "OpenGLGState.h"
#include "SceneRenderer.h"
#include "vectors.h"

#include "Singleton.h"


#define EFFECTS (EffectsRenderer::instance())


class BasicEffect
{
public:
  BasicEffect();
  virtual ~BasicEffect() {};

  virtual void setPos(const fvec3& pos);
  virtual void setRot(const fvec3& rot);
  virtual void setVel(const fvec3& vel);
  virtual void setColor(const fvec4& rgba);
  virtual void setStartTime(float time);

  virtual bool update(float time);
  virtual void draw(const SceneRenderer &) = 0;

protected:
  fvec3 position;
  fvec3 rotation;
  fvec3 velocity;
  fvec4 color;
  float startTime;
  float lifetime;
  float lastTime;
  float deltaTime;
  float age;
};

typedef std::vector<BasicEffect*> tvEffectsList;

class StdSpawnEffect : public BasicEffect
{
public:
  StdSpawnEffect();
  virtual ~StdSpawnEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);

protected:
  int	  texture;
  float	radius;
  OpenGLGState ringState;
};

class ConeSpawnEffect : public StdSpawnEffect
{
public:
  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);
};

class RingSpawnEffect : public StdSpawnEffect
{
public:
  RingSpawnEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);

private:
  void drawRing(int n, float coreAlpha);

  float maxZ;
  float ringRange;
  float numRings;
};

class StdShotEffect : public BasicEffect
{
public:
  StdShotEffect();
  virtual ~StdShotEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);

protected:
  int	  texture;
  float	radius;
  OpenGLGState ringState;
};

class FlashShotEffect : public StdShotEffect
{
public:
  FlashShotEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);

private:
  float length;
};

class StdDeathEffect : public BasicEffect
{
public:
  StdDeathEffect();
  virtual ~StdDeathEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);

protected:
  int	  texture;
  float	radius;
  OpenGLGState ringState;
};

class StdLandEffect : public BasicEffect
{
public:
  StdLandEffect();
  virtual ~StdLandEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);

protected:
  int	  texture;
  float	radius;
  OpenGLGState ringState;
};

class StdGMPuffEffect : public BasicEffect
{
public:
  StdGMPuffEffect();
  virtual ~StdGMPuffEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);
protected:
  int	  texture;
  float	radius;
  OpenGLGState ringState;
};

class StdRicoEffect : public BasicEffect
{
public:
  StdRicoEffect();
  virtual ~StdRicoEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);
protected:
  int	  texture;
  float	radius;
  OpenGLGState ringState;
};

class StdShotTeleportEffect : public BasicEffect
{
public:
  StdShotTeleportEffect(float length, const fvec4* clipPlane);
  virtual ~StdShotTeleportEffect();

  virtual bool update(float time);
  virtual void draw(const SceneRenderer& sr);

protected:
  int texture;
  float	radius;
  float length;
  const fvec4* clipPlane;
  OpenGLGState ringState;
};


class EffectsRenderer : public Singleton<EffectsRenderer>
{
public:
  // called once to setup the effects system
  void init();

  // called to update the various effects
  void update();

  // called to draw all the current effects
  void draw(const SceneRenderer& sr);

  // spawn flashes
  void addSpawnEffect(const fvec4& rgba, const fvec3& pos );
  std::vector<std::string> getSpawnEffectTypes();

  // shot flashes
  void addShotEffect(const fvec4& rgba, const fvec3& pos, float rot,
		     const fvec3& vel, int _type = -1);
  std::vector<std::string> getShotEffectTypes();

  // gm puffs
  void addGMPuffEffect(const fvec3& pos, const fvec2& rot, const fvec3* vel = NULL);
  std::vector<std::string> getGMPuffEffectTypes();

  // death effects
  void addDeathEffect(const fvec4& rgba, const fvec3& pos, float rot);
  std::vector<std::string> getDeathEffectTypes();

  // landing effects
  void addLandEffect(const fvec4& rgba, const fvec3& pos, float rot);
  std::vector<std::string> getLandEffectTypes();

  // rico effect
  void addRicoEffect(const fvec3& pos, const fvec3& normal, const fvec3* vel = NULL);
  std::vector<std::string> getRicoEffectTypes();

  // shot teleport effect
  void addShotTeleportEffect(const fvec3& pos, const fvec3& vel,
                             const fvec4* plane = NULL);
  std::vector<std::string> getShotTeleportEffectTypes();

protected:
  friend class Singleton<EffectsRenderer>;

protected:
  EffectsRenderer();
  ~EffectsRenderer();

  tvEffectsList effectsList;
};

#endif // BZF_EFFECTS_RENDERER_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
