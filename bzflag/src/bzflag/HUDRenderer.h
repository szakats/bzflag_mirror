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

#ifndef	__HUDRENDERER_H__
#define	__HUDRENDERER_H__

#include "common.h"

// system headers
#include <vector>
#include <string>

// common headers
#include "TimeKeeper.h"
#include "HUDuiTypeIn.h"
#include "Flag.h"
#include "SceneRenderer.h"
#include "vectors.h"

// local headers
#include "FlashClock.h"
#include "MainWindow.h"
#include "BzfDisplay.h"
#include "Player.h"
#include "ScoreboardRenderer.h"
#include "LocalPlayer.h"
#include "OpenGLUtils.h"

class LocalFontFace;


const int MaxAlerts = 3;


class HUDMarker {
public:
  float heading;
  fvec4 color;
};
typedef std::vector<HUDMarker> MarkerList;


class EnhancedHUDMarker
{
public:
  EnhancedHUDMarker()
  : pos(0.0f, 0.0f, 0.0f)
  , color(0.0f, 0.0f, 0.0f, 1.0f)
  {}
  EnhancedHUDMarker(const fvec3& p, const fvec4& c)
  : pos(p)
  , color(c)
  {}
  fvec3 pos;
  fvec4 color;
  std::string name;
  bool friendly;
};
typedef std::vector<EnhancedHUDMarker> EnhancedMarkerList;



/**
 * HUDRenderer:
 *	Encapsulates information about rendering the heads-up display.
 */
class HUDRenderer : public GLDisplayListCreator {
public:
  HUDRenderer(const BzfDisplay*, const SceneRenderer&);
  ~HUDRenderer();

  virtual void buildGeometry(GLDisplayList displayList);

  int getNoMotionSize() const;
  int getMaxMotionSize() const;

  void setColor(float r, float g, float b);
  void setPlaying(bool playing);
  void setRoaming(bool roaming);
  void setPlayerHasHighScore(bool = true);
  void setTeamHasHighScore(bool = true);
  void setHeading(float angle);
  void setAltitude(float altitude);
  void setAltitudeTape(bool = true);
  void setCracks(bool = true);
  void setFPS(float fps);
  void setDrawTime(float drawTimeInseconds);
  void setFrameTriangleCount(int tpf);
  void setFrameRadarTriangleCount(int rtpf);
  void setAlert(int num, const char* msg, float duration,
                bool warning = false);
  void setFlagHelp(FlagType* desc, float duration);
  void addMarker(float heading, const float* color);
  void setRestartKeyLabel(const std::string&);
  void setTimeLeft(uint32_t timeLeftInSeconds);

  void AddEnhancedMarker(const fvec3& pos, const fvec4& color,
                         bool friendly = false, float zShift = 0.0f);
  void AddEnhancedNamedMarker(const fvec3& pos, const fvec4& color, std::string name,
                              bool friendly = false, float zShift = 0.0f);
  void AddLockOnMarker(const fvec3& pos, std::string name,
                       bool friendly = false, float zShift = 0.0f);

  void saveMatrixes(const float* mm, const float* pm);
  void setDim(bool);

  bool getComposing() const;
  std::string getComposeString() const;
  void setComposeString(const std::string& message) const;
  void setComposeString(const std::string& message, bool allowEdit) const;

  void setComposing(const std::string& prompt);
  void setComposing(const std::string& prompt, bool allowEdit);

  void setLimboMessage(const std::string& message) const;

  void render(void);
  ScoreboardRenderer *getScoreboard();

protected:
  void hudColor3f(float, float, float);
  void hudColor4f(float, float, float, float);
  void hudColor3fv(const float*);
  void hudColor3Afv(const float*, const float);
  void hudColor4fv(const float*);
  void hudSColor3fv(const float*);
  void renderAlerts(void);
  void renderStatus(void);
  void renderOptions(SceneRenderer&);
  void renderCompose(SceneRenderer&);
  void renderBox(SceneRenderer&);
  void renderTankLabels(SceneRenderer&);
  void renderTimes(void);
  void renderShots(const Player*);

  void renderPlaying(SceneRenderer&);
  void renderNotPlaying(SceneRenderer&);
  void renderRoaming(SceneRenderer&);

  void drawLockonMarker(float* color, float alpha, float* object,
                        const float* viewPos, std::string name, bool friendly);
  void drawWaypointMarker(float* color, float alpha, float* object,
                          const float* viewPos, std::string name, bool friendly);
  void drawMarkersInView(int centerX, int centerY, const LocalPlayer* myTank);

  /** basic render update used by renderPlaying(), renderNotPlaying(), and renderRoaming()
   */
  void renderUpdate(SceneRenderer&);

  std::vector<std::string> makeHelpString(const char* help) const;

private:
  void setBigFontSize(int width, int height);
  void setAlertFontSize(int width, int height);
  void setMajorFontSize(int width, int height);
  void setMinorFontSize(int width, int height);
  void setHeadingFontSize(int width, int height);
  void setComposeFontSize(int width, int height);
  void setLabelsFontSize(int width, int height);

  void resize(bool firstTime);

  static void  resizeCallback(void*);
  static float tankScoreCompare(const void* a, const void* b);
  static int   teamScoreCompare(const void* a, const void* b);

  GLDisplayList	friendlyMarkerList;
private:
  const BzfDisplay*	display;
  ScoreboardRenderer*	scoreboard;
  MainWindow&		window;
  bool			firstRender;
  int			noMotionSize;
  int			maxMotionSize;
  float			headingOffset;
  fvec4			hudColor;
  fvec4			messageColor;
  fvec4			warningColor;

  LocalFontFace*	bigFontFace;
  float			bigFontSize;
  LocalFontFace*	alertFontFace;
  float			alertFontSize;
  LocalFontFace*	majorFontFace;
  float			majorFontSize;
  LocalFontFace*	minorFontFace;
  float			minorFontSize;
  LocalFontFace*	headingFontFace;
  float			headingFontSize;
  LocalFontFace*	composeFontFace;
  float			composeFontSize;
  LocalFontFace*	labelsFontFace;
  float			labelsFontSize;
  float			majorFontHeight;
  float			alertFontHeight;

  bool		playing;
  bool		roaming;
  bool		dim;
  int		numPlayers;
  uint32_t	timeLeft;
  TimeKeeper	timeSet;
  bool		playerHasHighScore;
  bool		teamHasHighScore;
  float		heading;
  float		altitude;
  bool		altitudeTape;
  float		fps;
  float		drawTime;
  int		headingMarkSpacing;
  float		headingLabelWidth[36];
  float		altitudeMarkSpacing;
  float		altitudeLabelMaxWidth;
  float		restartLabelWidth;
  float		resumeLabelWidth;
  float		autoPilotWidth;
  float		gameOverLabelWidth;
  float		huntArrowWidth;
  float		huntedArrowWidth;
  float		tkWarnRatio;
  std::string	restartLabel;

  FlashClock	globalClock;
  FlashClock	scoreClock;

  FlashClock	alertClock[MaxAlerts];
  std::string	alertLabel[MaxAlerts];
  float		alertLabelWidth[MaxAlerts];
  const float*	alertColor[MaxAlerts];

  float		flagHelpY;
  FlashClock	flagHelpClock;
  std::vector<std::string>	flagHelpText;

  bool		showOptions;
  bool		showCompose;
  bool		showCracks;

  HUDuiTypeIn*	composeTypeIn;

  bool		dater;
  time_t	lastTimeChange;
  int		triangleCount;
  int		radarTriangleCount;

  double	modelMatrix[16];
  double	projMatrix[16];
  int		viewport[4];

  MarkerList		markers;
  EnhancedMarkerList	enhancedMarkers;
  EnhancedMarkerList	lockOnMarkers;

  static const float	altitudeOffset;
  static const fvec4	black;
  static std::string	headingLabel[36];
  static std::string	restartLabelFormat;
  static std::string	resumeLabel;
  static std::string	gameOverLabel;
  static std::string	autoPilotLabel;
  static std::string    customLimboMessage;
};


#endif /* __HUDRENDERER_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
