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

#ifndef	__SCOREBOARDRENDERER_H__
#define	__SCOREBOARDRENDERER_H__

#include "common.h"

/* system interface headers */
#include <string>

/* common interface headers */
#include "bzfgl.h"
#include "vectors.h"

/* local interface headers */
#include "Player.h"

class LocalFontFace;

/**
 * ScoreboardRenderer:
 *	Encapsulates information about rendering the scoreboard display.
 */
class ScoreboardRenderer {
public:
  ScoreboardRenderer();
  ~ScoreboardRenderer();

  void setDim(bool);
  void setWindowSize(float x, float y, float width, float height);
  void render(bool forceDisplay);

  static Player* getLeader(std::string* label = NULL);
  static const int HUNT_NONE = 0;
  static const int HUNT_SELECTING = 1;
  static const int HUNT_ENABLED = 2;

  void setHuntState(int _state);
  int  getHuntState() const;
  void setHuntNextEvent();       // invoked when 'down' button pressed
  void setHuntPrevEvent();       // invoked when 'up' button pressed
  void setHuntSelectEvent();     // invoked when 'fire' button pressed
  void huntKeyEvent(bool isAdd); // invoked when '7' or 'U' is pressed
  void clearHuntedTanks();
  void huntReset();              // invoked when joining a server

  static void setAlwaysTeamScore(bool onoff);
  static bool getAlwaysTeamScore();

  static void setSort(int _sortby);
  static int  getSort();
  static const char** getSortLabels();
  enum SortingTypes {
    SortScore = 0,
    SortNormalized,
    SortCallsign,
    SortTKs,
    SortTkRatio,
    SortTeam,
    SortMyRatio,
    SortHuntLevel,
    SortTypeCount
  };

  void setTeamScoreY(float val) { teamScoreYVal = val; }
  void setRoaming(bool val) { roaming = val; }

protected:
  void hudColor3fv(const fvec4&);
  void renderScoreboard();
  void renderTeamScores();
  void renderCtfFlags();
  void drawPlayerScore(const Player*,
                       float x1, float x2, float x3, float xs,
                       float y, bool huntInd);
  static const char* sortLabels[SortTypeCount + 1];
  static int  sortMode;
  static bool alwaysShowTeamScore;

  void stringAppendNormalized(std::string& s, float n);

private:
  void setMinorFontSize();
  void setLabelsFontSize(float width, float height);
  void exitSelectState();

  static int teamScoreCompare(const void* a, const void* b);
  static int sortCompareCp(const void* a, const void* b);
  static int sortCompareI2(const void* a, const void* b);
  static Player** newSortedList(int sortType, bool obsLast, int *numPlayers = NULL);

private:
  float winX;
  float winY;
  float winWidth;
  float winHeight;

  float teamScoreYVal;
  bool  roaming;

  fvec4 messageColor;
  LocalFontFace*  minorFontFace;
  float           minorFontSize;
  LocalFontFace*  labelsFontFace;
  float           labelsFontSize;

  bool  dim;
  float scoreLabelWidth;
  float killsLabelWidth;
  float teamScoreLabelWidth;
  float teamCountLabelWidth;
  float huntArrowWidth;
  float huntPlusesWidth;
  float huntedArrowWidth;
  float tkWarnRatio;

  bool huntIndicator;
  int  huntPosition;
  bool huntSelectEvent;
  int  huntPositionEvent;
  int  huntState;
  bool huntAddMode; // valid only if state == SELECTING
  int  numHunted;

  static std::string scoreSpacingLabel;
  static std::string scoreLabel;
  static std::string killSpacingLabel;
  static std::string killLabel;
  static std::string teamScoreSpacingLabel;
  static std::string playerLabel;
  static std::string teamCountSpacingLabel;

};


#endif /* __SCOREBOARDRENDERER_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
