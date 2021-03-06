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

#ifndef	BZF_CONTROL_PANEL_H
#define	BZF_CONTROL_PANEL_H

#if defined(_MSC_VER)
  #pragma warning(disable: 4786)
#endif

// common - 1st
#include "common.h"

// system headers
#include <string>
#include <vector>
#include <deque>

//common headers
#include "bzfgl.h"
#include "vectors.h"

// local headers
#include "MainWindow.h"

class RadarRenderer;
class SceneRenderer;
class LocalFontFace;


struct ControlPanelMessage {
  ControlPanelMessage(const std::string&);
  void breakLines(float maxLength, int fontFace, float fontSize);

  std::string string;
  std::vector<std::string> lines;
  int numlines;
};


class ControlPanel {
  public:
    enum MessageModes {
      MessageAllTabs = -2,
      MessageCurrent = -1,
      MessageAll     = 0,
      MessageChat    = 1,
      MessageServer  = 2,
      MessageMisc    = 3,
      MessageDebug   = 4,
      MessageModeCount
    };

  public:
    ControlPanel(MainWindow&, SceneRenderer&);
    ~ControlPanel();

    void setControlColor(const fvec4* color = NULL);
    void render(SceneRenderer&);
    void resize();
    void invalidate();

    void setNumberOfFrameBuffers(int);

    void addMessage(const std::string&, MessageModes mode = MessageMisc);
    void setMessagesOffset(int offset, int whence, bool paged);
    void setMessagesMode(MessageModes messageMode);
    void setStatus(const char*);
    void setRadarRenderer(RadarRenderer*);
    MessageModes getMessagesMode() const { return messageMode; }

    void setDimming(float dimming);

    void saveMessages(const std::string& filename,
				     bool stripAnsi) const;

    int getModeMessageCount(MessageModes mode);
    const std::deque<ControlPanelMessage>* getModeMessages(MessageModes mode);

  private:
    // no copying!
    ControlPanel(const ControlPanel&);
    ControlPanel& operator=(const ControlPanel&);

    static void resizeCallback(void*);
    static void exposeCallback(void*);
    static void bzdbCallback(const std::string& name, void* data);
    static void loggingCallback(int level, const std::string& msg, void* data);

    bool isTabVisible(MessageModes mode) const;

  private:
    bool tabsOnRight;
    std::vector<const char *> *tabs;
    std::vector<float> tabTextWidth;
    long totalTabWidth;

    MainWindow&		window;
    bool		resized;
    int			numBuffers;
    int			exposed;
    int			changedMessage;
    RadarRenderer*	radarRenderer;
    SceneRenderer*	renderer;

    LocalFontFace*	fontFace;
    float		fontSize;

    float		dimming;
    float		du, dv;
    int			radarAreaPixels[4];
    int			messageAreaPixels[4];
    std::deque<ControlPanelMessage>	messages[MessageModeCount];
    MessageModes	messageMode;
    fvec4		teamColor;
    int			maxLines;
    float		margin;
    float		lineHeight;
    bool		unRead[MessageModeCount];
    int			messageCounts[MessageModeCount];

    static int		messagesOffset;
    static const int	maxScrollPages;
};


inline void ControlPanel::setDimming(float newDimming)
{
  const float newDim = 1.0f - newDimming;
  dimming = (newDim > 1.0f) ? 1.0f : (newDim < 0.0f) ? 0.0f : newDim;
}


#endif // BZF_CONTROL_PANEL_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
