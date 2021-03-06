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

/* BZFlag common header */
#include "common.h"

/* interface header */
#include "ControlPanel.h"

/* system headers */
#include <iostream>
#include <assert.h>
#include <time.h>

/* common implementation headers */
#include "bzregex.h"
#include "BZDBCache.h"
#include "FontManager.h"
#include "TextUtils.h"
#include "ErrorHandler.h"
#include "global.h"

/* local implementation headers */
#include "FontSizer.h"
#include "SceneRenderer.h"
#include "RadarRenderer.h"
#include "bzflag.h"
#include "LocalFontFace.h"
#include "bzUnicode.h"


//============================================================================//
//
// ControlPanelMessage
//

ControlPanelMessage::ControlPanelMessage(const std::string& _string)
: string(_string)
, numlines(0)
{
}


void ControlPanelMessage::breakLines(float maxLength, int fontFace, float fontSize)
{
  FontManager &fm = FontManager::instance();

  // get message and its length
  const char* msg = string.c_str();
  int lineLen     = (int)string.length();

  // if there are tabs in the message, find the last one
  int lastTab = (int)string.rfind('\t');

  lines.clear();
  numlines=0;

  // in order for the new font engine to draw successive lines in the right
  // color, it needs to be fed the right ansi codes at the beginning of each
  // line.
  std::string cumulativeANSICodes = "";

  // break lines
  while (lineLen > 0) {
    int lastWhitespace = 0;
    int n;

    // how many characters will fit?
    // the unprinted ANSI codes don't count
    if ((fm.getStringWidth(fontFace, fontSize, msg) <= maxLength) && (lastTab <= 0)) {
      n = lineLen;
    } else {
      n = 0;
      while ((n < lineLen) &&
	     (fm.getStringWidth(fontFace, fontSize,
				std::string(msg, ((++UTF8StringItr(msg+n)).getBufferFromHere()-msg)))
	      < maxLength)) {
	if (msg[n] == ESC_CHAR) {
	  // clear the cumulative codes when we hit a reset
	  // the reset itself will start the new cumulative string.
	  if ((strncmp(msg + n, ANSI_STR_RESET, strlen(ANSI_STR_RESET)) == 0)
	      || (strncmp(msg + n, ANSI_STR_RESET_FINAL, strlen(ANSI_STR_RESET_FINAL)) == 0))
	    cumulativeANSICodes = "";
	  // add this code to our cumulative string
	  cumulativeANSICodes += msg[n];
	  n++;
	  if ((n < lineLen) && (msg[n] == '[')) {
	    cumulativeANSICodes += msg[n];
	    n++;
	    while ((n < lineLen) &&
		   ((msg[n] == ';') ||
		    ((msg[n] >= '0') && (msg[n] <= '9')))) {
	      cumulativeANSICodes += msg[n];
	      n++;
	    }
	    // ditch the terminating character too
	    if (n < lineLen) {
	      cumulativeANSICodes += msg[n];
	      n++;
	    }
	  }
	} else {
	  n = static_cast<int>((++UTF8StringItr(msg+n)).getBufferFromHere()-msg);
	}
	if (TextUtils::isWhitespace(msg[n])) {
	  lastWhitespace = n;
	  // Tabs break out into their own message.  These get dealt with
	  // in ControlPanel::render, which will increment x instead of y.
	  if (msg[n] == '\t')
	    break;
	}
      }
    }

    if (lastWhitespace > 0)
      n = lastWhitespace;

    // message
    lines.push_back(cumulativeANSICodes + std::string(msg,n));

    if (msg[n] != '\t')
      numlines++;

    // account for portion broken
    msg += n;
    lineLen -= n;
    lastTab -= n;

  }
}


const std::deque<ControlPanelMessage>*
  ControlPanel::getModeMessages(MessageModes mode)
{
  if ((mode < 0) || (mode >= MessageModeCount)) {
    return NULL;
  }
  return &(messages[mode]);
}


int ControlPanel::getModeMessageCount(MessageModes mode)
{
  if ((mode < 0) || (mode >= MessageModeCount)) {
    return -1;
  }
  return messageCounts[mode];
}


//============================================================================//
//
// ControlPanel
//
int ControlPanel::messagesOffset = 0;

ControlPanel::ControlPanel(MainWindow& _mainWindow, SceneRenderer& _renderer)
: tabsOnRight(true)
, tabs(NULL)
, totalTabWidth(0)
, window(_mainWindow)
, resized(false)
, numBuffers(2)
, changedMessage(0)
, radarRenderer(NULL)
, renderer(&_renderer)
, fontFace(NULL)
, dimming(1.0f)
, du(0)
, dv(0)
, messageMode(MessageAll)
, teamColor(0.0f, 0.0f, 0.0f, 1.0f)
{
  setControlColor();

  // make sure we're notified when MainWindow resizes or is exposed
  window.getWindow()->addResizeCallback(resizeCallback, this);
  window.getWindow()->addExposeCallback(exposeCallback, this);
  BZDB.addCallback("debugLevel",   bzdbCallback, this);
  BZDB.addCallback("displayRadar", bzdbCallback, this);
  BZDB.addCallback(StateDatabase::BZDB_RADARLIMIT, bzdbCallback, this);

  // other initialization
  radarAreaPixels[0] = 0;
  radarAreaPixels[1] = 0;
  radarAreaPixels[2] = 0;
  radarAreaPixels[3] = 0;
  messageAreaPixels[0] = 0;
  messageAreaPixels[1] = 0;
  messageAreaPixels[2] = 0;
  messageAreaPixels[3] = 0;
  for (int i = 0; i < MessageModeCount; i++) {
    messages[i].clear();
    messageCounts[i] = 0;
    unRead[i] = false;
  }
  teamColor[0] = teamColor[1] = teamColor[2] = (float)0.0f;

  maxLines = 30;

  /* use pointer initialization to perform precomputation and
   * prestorage. eventually should move this data into constructor.
   */
  if (tabs == NULL) {
    tabs = new std::vector<const char *>;
    tabs->push_back("All");
    tabs->push_back("Chat");
    tabs->push_back("Server");
    tabs->push_back("Misc");
    tabs->push_back("Debug");
  }

  resize(); // need resize to set up font and window dimensions

  // register after we're fully initialized
  registerLoggingProc(loggingCallback, this);
}

ControlPanel::~ControlPanel()
{
  // don't notify me anymore (cos you can't wake the dead!)
  unregisterLoggingProc(loggingCallback, this);
  window.getWindow()->removeResizeCallback(resizeCallback, this);
  window.getWindow()->removeExposeCallback(exposeCallback, this);
  BZDB.removeCallback("debugLevel",   bzdbCallback, this);
  BZDB.removeCallback("displayRadar", bzdbCallback, this);
  BZDB.removeCallback(StateDatabase::BZDB_RADARLIMIT, bzdbCallback, this);

  // release font face
  if (fontFace)
    LocalFontFace::release(fontFace);

  if (echoToConsole && echoAnsi) {
    std::cout << ColorStrings[FinalResetColor] << std::flush;
  }
  if (tabs != NULL) {
    tabs->clear();
    delete tabs;
    tabTextWidth.clear();
    totalTabWidth = 0;
  }
}


void ControlPanel::loggingCallback(int level,
                                   const std::string& rawMsg, void* data)
{
// -- always store the debug messages
//  if (level > debugLevel) {
//    return;
//  }
  std::string msg = rawMsg;
  while (!msg.empty() && (msg[msg.size() - 1] == '\n')) {
    msg.resize(msg.size() - 1);
  }
  std::string color = "";
  if (level >= 0) {
    switch (level) {
      case 1:  { color = ANSI_STR_FG_GREEN;   break; }
      case 2:  { color = ANSI_STR_FG_CYAN;    break; }
      case 3:  { color = ANSI_STR_FG_BLUE;    break; }
      case 4:  { color = ANSI_STR_FG_YELLOW;  break; }
      case 5:  { color = ANSI_STR_FG_ORANGE;  break; }
      case 6:  { color = ANSI_STR_FG_RED;     break; }
      case 7:  { color = ANSI_STR_FG_MAGENTA; break; }
      case 8:  { color = ANSI_STR_FG_WHITE;   break; }
      case 9:  { color = ANSI_STR_FG_BLACK;   break; }
      default: { color = ANSI_STR_UNDERLINE;  break; }
    }
  }
  ((ControlPanel*)data)->addMessage(color + msg, ControlPanel::MessageDebug);
}


void ControlPanel::bzdbCallback(const std::string& /*name*/, void* data)
{
  ((ControlPanel*)data)->resize();
  return;
}


void ControlPanel::setControlColor(const fvec4* color)
{
  if (color != NULL) {
    teamColor = *color;
  } else {
    teamColor = fvec4(0.0f, 0.0f, 0.0f, 1.0f);
  }
}


bool ControlPanel::isTabVisible(MessageModes mode) const
{
  if (debugLevel > 0) {
    return true;
  }
  return (mode != MessageDebug);
}


void ControlPanel::render(SceneRenderer& _renderer)
{
  if (!BZDB.isTrue("displayConsole")) {
    // always draw the console if its fully opaque
    const float opacity = RENDERER.getPanelOpacity();
    if (opacity != 1.0f) {
      return;
    }
  }

  if (!resized) resize();

  // optimization for software rendering folks
  if (!changedMessage && _renderer.getPanelOpacity() == 1.0f) {
    return;
  }

  int i, j;
  const int x = window.getOriginX();
  const int y = window.getOriginY();
  const int w = window.getWidth();
  const int tabStyle = BZDB.evalInt("showtabs");
  const bool showTabs = (tabStyle > 0);
  tabsOnRight = (tabStyle == 2);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (double)w, 0.0, window.getHeight(), -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  OpenGLGState::resetState();

  FontManager &fm = FontManager::instance();
  fm.setOpacity(dimming);

  if (changedMessage > 0) {
    changedMessage--;
  }
  float fx = messageAreaPixels[0] + margin;
  float fy = messageAreaPixels[1] + margin + 1.0f;
  int   ay = (_renderer.getPanelOpacity() == 1.0f || !showTabs) ? 0
    : int(lineHeight + 4);

  glScissor(messageAreaPixels[0] + x - 1,
	    messageAreaPixels[1] + y,
	    messageAreaPixels[2] + 1,
	    messageAreaPixels[3] + ay);
  OpenGLGState::resetState();

  if (_renderer.getPanelOpacity() > 0.0f) {
    // nice blended messages background
    if (BZDBCache::blend && _renderer.getPanelOpacity() < 1.0f)
      glEnable(GL_BLEND);

    // clear the background
    glColor4f(0.0f, 0.0f, 0.0f, _renderer.getPanelOpacity());
    glRecti(messageAreaPixels[0] - 1, // clear an extra pixel column to simplify fuzzy float stuff later
	    messageAreaPixels[1],
	    messageAreaPixels[0] + messageAreaPixels[2],
	    messageAreaPixels[1] + messageAreaPixels[3]);

    // display tabs for chat sections
    if (showTabs) {
      long int drawnTabWidth = 0;
      for (size_t tab = 0; tab < tabs->size(); tab++) {
        if (!isTabVisible((MessageModes)tab)) {
          continue;
        }

	// current mode is given a dark background to match the control panel
	if (messageMode == MessageModes(tab)) {
	  glColor4f(0.0f, 0.0f, 0.0f, _renderer.getPanelOpacity());
	} else {
	  glColor4f(0.10f, 0.10f, 0.10f, _renderer.getPanelOpacity());
	}

	if (tabsOnRight) {
	  // draw the tabs on the right side
	  glRecti(messageAreaPixels[0] + messageAreaPixels[2] - totalTabWidth + drawnTabWidth,
		  messageAreaPixels[1] + messageAreaPixels[3] - int(lineHeight + 4) + ay,
		  messageAreaPixels[0] + messageAreaPixels[2] - totalTabWidth + drawnTabWidth + int(tabTextWidth[tab]), //+ drawnTabWidth + int(tabTextWidth[tab]),
		  messageAreaPixels[1] + messageAreaPixels[3] + ay);
	} else {
	  // draw the tabs on the left side
	  glRecti(messageAreaPixels[0] + drawnTabWidth,
		  messageAreaPixels[1] + messageAreaPixels[3] - int(lineHeight + 4) + ay,
		  messageAreaPixels[0] + drawnTabWidth + int(tabTextWidth[tab]),
		  messageAreaPixels[1] + messageAreaPixels[3] + ay);
	}
	drawnTabWidth += long(tabTextWidth[tab]);
      } // end iteration over tabs
    }

    if (BZDBCache::blend && _renderer.getPanelOpacity() < 1.0f)
      glDisable(GL_BLEND);
  }

  // show scroll indicator if not at end
  if (messagesOffset > 0 && messageMode >= 0) {
    int lines = int(messages[messageMode].size());
    if (lines > 0) {

      const float size = std::max(float(maxLines) / lines, 0.02f);
      const float offset = float(messagesOffset) / lines;
      const int maxTop = messageAreaPixels[1] + messageAreaPixels[3];
      int top = messageAreaPixels[1] + int((offset + size) * (float)messageAreaPixels[3]);
      if (top > maxTop) {
	top = maxTop;
      }
      glColor3f(0.7f, 0.7f, 0.7f);
      glRecti(messageAreaPixels[0],
	      messageAreaPixels[1] + int(offset * (float)messageAreaPixels[3]),
	      messageAreaPixels[0] + 2, top);

    }
  }

  // Draw tab labels
  if (showTabs) {
    long int drawnTabWidth = 0;
    for (size_t tab = 0; tab < tabs->size(); tab++) {
      if (!isTabVisible((MessageModes)tab)) {
        continue;
      }

      // current mode is bright, others are not so bright
      if (messageMode == MessageModes(tab)) {
	glColor4f(1.0f, 1.0f, 1.0f, dimming);
      } else if (unRead[MessageModes(tab)]) {
	glColor4f(0.5f, 0.0f, 0.0f, dimming);
      } else {
	glColor4f(0.5f, 0.5f, 0.5f, dimming);
      }

      if (tabsOnRight) {
	// draw the tabs on the right side (with one letter padding)
	fm.drawString(messageAreaPixels[0] + messageAreaPixels[2] - totalTabWidth + drawnTabWidth + floorf(fontSize),
		      messageAreaPixels[1] + messageAreaPixels[3] - floorf(lineHeight * 0.9f) + ay,
		      0.0f, fontFace->getFMFace(), (float)fontSize, (*tabs)[tab]);
      } else {
	// draw the tabs on the left side (with one letter padding)
	fm.drawString(messageAreaPixels[0] + drawnTabWidth + floorf(fontSize),
		      messageAreaPixels[1] + messageAreaPixels[3] - floorf(lineHeight * 0.9f) + ay,
		      0.0f, fontFace->getFMFace(), (float)fontSize, (*tabs)[tab]);
      }
      drawnTabWidth += long(tabTextWidth[tab]);
    }
  }

  /* draw messages
   *
   * It works by first breaking the string into a vector of strings
   * (done elsewhere), each of which will fit the control panel, and
   * tallying the number of lines, then moving up the proper number of
   * lines and displaying downward -- that is, it kinda backtracks for
   * each line that will wrap.
   *
   * messageAreaPixels[2] = Width of Message Window in Pixels.
   *
   * maxLines = Max messages lines that can be displayed at once per
   * page.  This COULD be a BZDB parameter (but isn't).
   *
   * maxScrollPages = This number * maxLines is the total maximum
   * lines of messages (and scrollback). It is stored as a BZDB
   * parameter.
   */

  glScissor(messageAreaPixels[0] + x,
	    messageAreaPixels[1] + y,
	    messageAreaPixels[2],
	    messageAreaPixels[3] - (showTabs ? int(lineHeight + 4) : 0) + ay);

  if (messageMode >= 0) {
    i = (int)messages[messageMode].size() - 1;
  } else {
    i = -1;
  }
  if (i >= 0 && messagesOffset > 0) {
    i -= messagesOffset;
    if (i < 0)
      i = 0;
  }

  const std::string highlightPattern = BZDB.get("highlightPattern");
  bool useHighlight = (highlightPattern.size() > 0);
  regex_t re;
  if (useHighlight) {
    if (regcomp(&re, highlightPattern.c_str(), REG_EXTENDED | REG_ICASE) != 0) {
      useHighlight = false; // bad regex
    }
  }

  for (j = 0; i >= 0 && j < maxLines; i--) {
    // draw each line of text
    int numLines = messages[messageMode][i].numlines;
    int numStrings = (int)messages[messageMode][i].lines.size();
    int msgy = numLines - 1;
    int msgx = 0;

    // see if this message need to be highlighted (check each line)
    bool highlight = false;
    if (useHighlight) {
      for (int l = 0; l < numStrings; l++)  {
	const std::string &msg = messages[messageMode][i].lines[l];
	std::string raw = stripAnsiCodes(msg.c_str());
	if (regexec(&re, raw.c_str(), 0, NULL, 0) == 0) {
	  highlight = true;
	}
      }
    }

    // default to drawing text in white
    float whiteColor[4] = {1.0f, 1.0f, 1.0f, dimming};
    glColor4fv(whiteColor);

    bool isTab = false;

    for (int l = 0; l < numStrings; l++)  {
      const std::string &msg = messages[messageMode][i].lines[l];

      // Tab chars move horizontally instead of vertically
      // It doesn't matter where in the string the tab char is
      // Usually it will be like <ansi><ansi><ansi>\ttext
      // We use 1 tabstop spaced 1/3 of the way across the controlpanel
      isTab = (msg.find('\t') != std::string::npos);
      if (isTab) {
	msgx += messageAreaPixels[2] / 3;
	msgy++;
      } else {
	msgx = 0;
      }

      assert(msgy >= 0);

      // only draw message if inside message area
      if (j + msgy < maxLines) {
	if (!highlight) {
	  fm.drawString(fx + msgx, fy + msgy * lineHeight, 0, fontFace->getFMFace(), fontSize, msg);
	} else {
	  // highlight this line
	  std::string newMsg = ANSI_STR_PULSATING;
	  newMsg += ANSI_STR_UNDERLINE;
	  newMsg += ANSI_STR_FG_CYAN;
	  newMsg += stripAnsiCodes(msg.c_str());
	  fm.drawString(fx + msgx, fy + msgy * lineHeight, 0, fontFace->getFMFace(), fontSize, newMsg);
	}
      }

      // next line
      msgy--;
    }
    j += numLines;
    fy += int(lineHeight * numLines);
  }

  // free the regex memory
  if (useHighlight) {
    regfree(&re);
  }

  glScissor(messageAreaPixels[0] + x - 2,
	    messageAreaPixels[1] + y - 2,
	    messageAreaPixels[2] + 3,
	    messageAreaPixels[3] + 33);
  OpenGLGState::resetState();

  // draw the lines around the console panel
  long xpos;
  long ypos;

  float outlineOpacity = RENDERER.getPanelOpacity();
  float fudgeFactor = BZDBCache::hudGUIBorderOpacityFactor; // bzdb cache this maybe?
  if (outlineOpacity < 1.0f)
    outlineOpacity = (outlineOpacity*fudgeFactor) + (1.0f - fudgeFactor);

  if (BZDBCache::blend)
    glEnable(GL_BLEND);

  // nice border
  glColor4f(teamColor[0], teamColor[1], teamColor[2],outlineOpacity);
  glBegin(GL_LINE_LOOP); {
    // bottom left
    xpos = x + messageAreaPixels[0] - 1;
    ypos = y + messageAreaPixels[1] - 1;
    glVertex2f((float) xpos, (float) ypos);

    // bottom right
    xpos += messageAreaPixels[2] + 1;
    glVertex2f((float) xpos, (float) ypos);

    // top right
    ypos += messageAreaPixels[3] + 1;
    glVertex2f((float) xpos, (float) ypos);

    // over to panel on left
    if (!tabsOnRight) {
      xpos = x + messageAreaPixels[0] + totalTabWidth;
      glVertex2f((float) xpos, (float) ypos);
    }

    // across the top from right to left
    long int drawnTabWidth = 0;
    for (int tab = (int)tabs->size() - 1; tab >= 0; tab--) {
      if (!isTabVisible((MessageModes)tab)) {
        continue;
      }
      if (messageMode == MessageModes(tab)) {
	ypos += ay;
	glVertex2f((float) xpos, (float) ypos);

	xpos -= long(tabTextWidth[tab]) + 1;
	glVertex2f((float) xpos, (float) ypos);

	ypos -= ay;
	glVertex2f((float) xpos, (float) ypos);
      } else {
	xpos -= long(tabTextWidth[tab]);
	glVertex2f((float) xpos, (float) ypos);
      }
      drawnTabWidth += long(tabTextWidth[tab]);
    }

    // over from panel on right
    //    if (tabsOnRight) {
    xpos = x + messageAreaPixels[0] - 1;
    glVertex2f((float) xpos, (float) ypos);
    //    }

  } glEnd();

  // some engines miss the corners
  glBegin(GL_POINTS); {
    glVertex2f((float) (x + messageAreaPixels[0] - 1),
	       (float) (y + messageAreaPixels[1] - 1));
    glVertex2f((float) (x + messageAreaPixels[0] + messageAreaPixels[2]),
	       (float) (y + messageAreaPixels[1] - 1));
    glVertex2f((float) (x + messageAreaPixels[0] + messageAreaPixels[2]),
	       (float) (y + messageAreaPixels[1] + messageAreaPixels[3]));
    glVertex2f((float) (x + messageAreaPixels[0] - 1),
	       (float) (y + messageAreaPixels[1] + messageAreaPixels[3]));
    long int tabPosition = 0;
    for (size_t tab = 0; tab < tabs->size(); tab++) {
      if (!isTabVisible((MessageModes)tab)) {
        continue;
      }
      if (messageMode == MessageModes(tab)) {
	if (tabsOnRight) {
	  glVertex2f((float) (x + messageAreaPixels[0] + messageAreaPixels[2] - totalTabWidth + tabPosition),
		     (float) (y + messageAreaPixels[1] + messageAreaPixels[3] + ay));
	  glVertex2f((float) (x + messageAreaPixels[0] + messageAreaPixels[2] - totalTabWidth + tabPosition + long(tabTextWidth[tab])),
		     (float) (y + messageAreaPixels[1] + messageAreaPixels[3] + ay));
	} else {
	  glVertex2f((float) (x + messageAreaPixels[0] + tabPosition),
		     (float) (y + messageAreaPixels[1] + messageAreaPixels[3] + ay));
	  glVertex2f((float) (x + messageAreaPixels[0] + tabPosition + long(tabTextWidth[tab])),
		     (float) (y + messageAreaPixels[1] + messageAreaPixels[3] + ay));
	}
      }
      tabPosition += long(tabTextWidth[tab]);
    }
  } glEnd();

  if (BZDBCache::blend)
    glDisable(GL_BLEND);

  glColor4f(teamColor[0], teamColor[1], teamColor[2],1.0f);

  glPopMatrix();

  fm.setOpacity(1.0f);
}


void ControlPanel::resize()
{
  float radarSpace, radarSize;
  // get important metrics
  const float w = (float)window.getWidth();
  const float h = (float)window.getHeight();
  const float opacity = RENDERER.getPanelOpacity();
  radarSize = float(window.getHeight() - window.getViewHeight());
  if (opacity == 1.0f) {
    radarSize = float(window.getHeight() - window.getViewHeight());
    radarSpace = 0.0f;
  } else {
    radarSize = h * (14 + RENDERER.getRadarSize()) / 60.0f;
    radarSpace = 3.0f * w / MinY;
  }

  // compute areas in pixels x,y,w,h
  // leave off 1 pixel for the border
  radarAreaPixels[0] = radarAreaPixels[1] = (int)radarSpace + 1;
  radarAreaPixels[2] = radarAreaPixels[3] = (int)(radarSize - (radarSpace * 2.0f)) - 2;

  messageAreaPixels[0] = (int)radarSize + 1; // X coord
  messageAreaPixels[1] = radarAreaPixels[1]; // Y coord
  messageAreaPixels[2] = (int)(w - radarSize - radarSpace) - 2; // Width
  messageAreaPixels[3] = radarAreaPixels[3]; // Height
  if (!BZDB.isTrue("displayRadar") || (BZDBCache::radarLimit <= 0.0f)) {
    messageAreaPixels[0] = (int)radarSpace + 1;
    messageAreaPixels[2] = (int)(w - (radarSpace * 2.0f)) - 2;
  }

  // if radar connected then resize it
  if (radarRenderer)
    radarRenderer->setShape(radarAreaPixels[0], radarAreaPixels[1],
			    radarAreaPixels[2], radarAreaPixels[3]);

  FontManager &fm = FontManager::instance();
  if (!fontFace)
    fontFace = LocalFontFace::create("consoleFont");

  FontSizer fs = FontSizer(w, h);
  fontSize = fs.getFontSize(fontFace, "consoleFontSize");

  // tab widths may have changed
  if (tabs) {
    tabTextWidth.clear();
    totalTabWidth = 0;
    const float charWidth = fm.getStringWidth(fontFace->getFMFace(), fontSize, "-");
    for (size_t tab = 0; tab < tabs->size(); tab++) {
      if (!isTabVisible((MessageModes)tab)) {
        continue;
      }
      // add space for about 2-chars on each side for padding
      tabTextWidth.push_back(fm.getStringWidth(fontFace->getFMFace(), fontSize, (*tabs)[tab]) + (4.0f * charWidth));
      totalTabWidth += long(tabTextWidth[tab]);
    }
  }

  lineHeight = fm.getStringHeight(fontFace->getFMFace(), fontSize);

  maxLines = int(messageAreaPixels[3] / lineHeight);

  margin = lineHeight / 4.0f;

  // rewrap all the lines
  for (int i = 0; i < MessageModeCount; i++) {
    for (int j = 0; j < (int)messages[i].size(); j++) {
      messages[i][j].breakLines(messageAreaPixels[2] - 2 * margin,
                                fontFace->getFMFace(), fontSize);
    }
  }

  // note that we've been resized at least once
  resized = true;
  invalidate();
}


void ControlPanel::resizeCallback(void* self)
{
  ((ControlPanel*)self)->resize();
}


void ControlPanel::setNumberOfFrameBuffers(int n)
{
  numBuffers = n;
}


void ControlPanel::invalidate()
{
  if (numBuffers) {
    changedMessage = numBuffers;
  } else {
    changedMessage++;
  }
}


void ControlPanel::exposeCallback(void* self)
{
  ((ControlPanel*)self)->invalidate();
}


void ControlPanel::setMessagesOffset(int offset, int whence, bool paged)
{
  if (paged) {
    if (abs(offset) <= 1) {
      offset = offset * (maxLines - 1);
    } else {
      offset = offset * maxLines;
    }
  }

  // offset = offset from whence (offset of 0 is the bottom/most recent)
  // whence = 0, 1, or 2 (akin to SEEK_SET, SEEK_CUR, SEEK_END)

  switch (whence) {
  case 0:
    if (offset < (int)messages[messageMode].size())
      messagesOffset = offset;
    else
      messagesOffset = (int)messages[messageMode].size() - 1;
    break;
  case 1:
    if (offset > 0) {
      if (messagesOffset + offset < (int)messages[messageMode].size())
	messagesOffset += offset;
      else
	messagesOffset = (int)messages[messageMode].size() - 1;
    } else if (offset < 0) {
      if (messagesOffset + offset >= 0)
	messagesOffset += offset;
      else
	messagesOffset = 0;
    }
    break;
  case 2:
    if (offset < 0) {
      if ((int)messages[messageMode].size() >= offset)
	messagesOffset += offset;
      else
	messagesOffset = 0;
    }
    break;
  }
  invalidate();
}


void ControlPanel::setMessagesMode(MessageModes mode)
{
  if ((mode == MessageDebug) && (debugLevel <= 0)) {
    return;
  }
  messageMode = mode;
  if (messageMode == MessageAll) {
    for (int i = 0; i < MessageModeCount; i++) {
      unRead[i] = false;
    }
  }
  else if (messageMode >= MessageChat) {
    unRead[messageMode] = false;
  }
  invalidate();
}


void ControlPanel::addMessage(const std::string& line,
			      MessageModes realmode)
{
  ControlPanelMessage item(line);
  item.breakLines(messageAreaPixels[2] - 2 * margin, fontFace->getFMFace(), fontSize);

  int _maxScrollPages = BZDB.evalInt("scrollPages");
  if (_maxScrollPages <= 0) {
    _maxScrollPages = atoi(BZDB.getDefault("scrollPages").c_str());
    BZDB.setInt("scrollPages", _maxScrollPages);
  }

  // the effective tab
  const int tabmode = (realmode == MessageCurrent) ? messageMode : realmode;

  // add to the appropriate tabs
  for (int tab = MessageAll; tab < MessageModeCount; tab++) {

    if ((tab == tabmode) // add to its own mode
	// add to the All tab, unless using Debug or Current mode
	|| ((tab == MessageAll) && (realmode != MessageDebug)
	                        && (realmode != MessageCurrent))
	// always add to all tabs
	|| (realmode == MessageAllTabs)) {

      // insert the message into the tab
      if ((int)messages[tab].size() < maxLines * _maxScrollPages) {
	// not full yet so just append it
	messages[tab].push_back(item);
      } else {
	// rotate list and replace oldest (in newest position after rotate)
	messages[tab].pop_front();
	messages[tab].push_back(item);
      }
      messageCounts[tab]++;

      // visible changes, force a console refresh
      if (messageMode == tab) {
	invalidate();
      }

      // mark the tab as unread (if viewing tabs)
      const bool showTabs = (BZDB.evalInt("showtabs") > 0);
      if (showTabs && (messageMode != tab) && (messageMode != MessageAll)) {
	unRead[tab] = true;
      }
    }
  }

  if (echoToConsole){
#ifdef _WIN32
    // this is cheap but it will work on windows
    FILE *fp = fopen ("stdout.txt","a+");
    if (fp){
      fprintf(fp,"%s\n", stripAnsiCodes(line.c_str()));
      fclose(fp);
    }
#else
    if (echoAnsi) {
      std::cout << line.c_str() << ColorStrings[ResetColor] << std::endl;
    } else {
      std::cout << stripAnsiCodes(line.c_str()) << std::endl;
    }
    fflush(stdout);
#endif
  }
}


void ControlPanel::saveMessages(const std::string& filename,
				bool stripAnsi) const
{
  FILE* file = fopen(filename.c_str(), "a+");
  if (!file) {
    return;
  }

  const time_t nowTime = time (NULL);
  fprintf(file, "\n----------------------------------------\n");
  fprintf(file, "Messages saved: %s", ctime(&nowTime));
  fprintf(file, "----------------------------------------\n\n");

  // add to the appropriate tabs
  std::deque<ControlPanelMessage>::const_iterator msg = messages[MessageAll].begin()++;
  for (; msg != messages[MessageAll].end(); ++msg) {
    const std::string& line = msg->string;
    if (stripAnsi) {
      fprintf(file, "%s\n", stripAnsiCodes(line.c_str()));
    } else {
      fprintf(file, "%s%s\n", line.c_str(), ColorStrings[ResetColor]);
    }
  }

  fclose(file);

  return;
}


void ControlPanel::setRadarRenderer(RadarRenderer* rr)
{
  radarRenderer = rr;
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
