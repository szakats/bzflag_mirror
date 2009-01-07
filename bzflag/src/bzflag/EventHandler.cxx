/* bzflag
* Copyright (c) 1993 - 2008 Tim Riker
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

// implementation header
#include "EventHandler.h"

// system headers
#include <assert.h>
#include <algorithm>

// common headers
#include "bzfio.h"
#include "OpenGLGState.h"
#include "OpenGLPassState.h"
#include "GfxBlock.h"

using std::string;
using std::vector;
using std::set;
using std::map;


EventHandler eventHandler;


/******************************************************************************/
/******************************************************************************/

// it's easier to read with named booleans
static const int REQ_FULL_READ  = (1 << 0);
static const int REQ_INPUT_CTRL = (1 << 1);


void EventHandler::SetupEvent(const string& eName, EventClientList* list,
                              int bits, bool reversed)
{
  list->set_reversed(reversed);
  assert(eventMap.find(eName) == eventMap.end());
  const bool reqFullRead  = (bits & REQ_FULL_READ);
  const bool reqInputCtrl = (bits & REQ_INPUT_CTRL);
  eventMap[eName] = EventInfo(eName, list, reqFullRead, reqInputCtrl, reversed);
}


#define SETUP_EVENT(name, ...) \
  SetupEvent(#name, &list ## name, ## __VA_ARGS__)


/******************************************************************************/
/******************************************************************************/

EventHandler::EventHandler()
{
  mouseOwner = NULL;

  SETUP_EVENT(Update);

  SETUP_EVENT(BZDBChange);

  SETUP_EVENT(RecvChatMsg);
  SETUP_EVENT(RecvLuaData);

  SETUP_EVENT(ServerJoined);
  SETUP_EVENT(ServerParted);

  SETUP_EVENT(PlayerAdded);
  SETUP_EVENT(PlayerRemoved);
  SETUP_EVENT(PlayerSpawned);
  SETUP_EVENT(PlayerKilled);
  SETUP_EVENT(PlayerJumped);
  SETUP_EVENT(PlayerLanded);
  SETUP_EVENT(PlayerTeleported);
  SETUP_EVENT(PlayerTeamChange);
  SETUP_EVENT(PlayerScoreChange);

  SETUP_EVENT(ShotAdded);
  SETUP_EVENT(ShotRemoved);

  SETUP_EVENT(FlagAdded);
  SETUP_EVENT(FlagRemoved);
  SETUP_EVENT(FlagGrabbed);
  SETUP_EVENT(FlagDropped);
  SETUP_EVENT(FlagCaptured);
  SETUP_EVENT(FlagTransferred);

  SETUP_EVENT(ViewResize);
  SETUP_EVENT(GLContextInit);
  SETUP_EVENT(GLContextFree);

  SETUP_EVENT(DrawGenesis,     0, true);
  SETUP_EVENT(DrawWorldStart,  0, true);
  SETUP_EVENT(DrawWorld,       0, true);
  SETUP_EVENT(DrawWorldAlpha,  0, true);
  SETUP_EVENT(DrawWorldShadow, 0, true);
  SETUP_EVENT(DrawScreenStart, 0, true);
  SETUP_EVENT(DrawScreen,      0, true);
  SETUP_EVENT(DrawRadar,       0, true);

  SETUP_EVENT(RenderPlayer);
  SETUP_EVENT(RenderPlayerExplosion);
  SETUP_EVENT(RenderShot);
  SETUP_EVENT(RenderShotExplosion);
  SETUP_EVENT(RenderFlag);

  SETUP_EVENT(KeyPress,     REQ_INPUT_CTRL);
  SETUP_EVENT(KeyRelease,   REQ_INPUT_CTRL);
  SETUP_EVENT(MousePress,   REQ_INPUT_CTRL);
  SETUP_EVENT(MouseMove,    REQ_INPUT_CTRL);
  SETUP_EVENT(MouseRelease, REQ_INPUT_CTRL);
  SETUP_EVENT(MouseWheel,   REQ_INPUT_CTRL);
  SETUP_EVENT(IsAbove,      REQ_INPUT_CTRL);
  SETUP_EVENT(GetTooltip,   REQ_INPUT_CTRL);

  SETUP_EVENT(WordComplete, REQ_INPUT_CTRL);
}


EventHandler::~EventHandler()
{
}


/******************************************************************************/
/******************************************************************************/

void EventHandler::AddClient(EventClient* ec)
{
  if (ec->GetName().empty()) {
    logDebugMessage(0, "EventClients must have valid names");
    return;
  }

  if (!clients.insert(ec)) {
    logDebugMessage(0, "Duplicate EventClient name: %s\n");
    return;
  }

  // add events that the clients wants (and is allowed to use)
  EventMap::const_iterator it;
  for (it = eventMap.begin(); it != eventMap.end(); ++it) {
    const EventInfo& ei = it->second;
    if (ei.IsManaged() && CanUseEvent(ec, ei) && ec->WantsEvent(ei.name)) {
      ei.GetList()->insert(ec);
    }
  }
}


void EventHandler::RemoveClient(EventClient* ec)
{
  if (mouseOwner == ec) {
    mouseOwner = NULL;
  }

  clients.remove(ec);

  EventMap::const_iterator it;
  for (it = eventMap.begin(); it != eventMap.end(); ++it) {
    const EventInfo& ei = it->second;
    if (ei.IsManaged() && (ei.GetList() != NULL)) {
      ei.GetList()->remove(ec);
    }
  }

  GfxBlockMgr::removeClient(ec);

//???  RemoveClientActions(ec, syncedActions);
//???  RemoveClientActions(ec, unsyncedActions);
}


/******************************************************************************/
/******************************************************************************/

void EventHandler::GetEventList(vector<string>& list) const
{
  list.clear();
  EventMap::const_iterator it;
  for (it = eventMap.begin(); it != eventMap.end(); ++it) {
    list.push_back(it->first);
  }
}


const EventHandler::EventInfo*
  EventHandler::GetEventInfo(const std::string& eName) const
{
  EventMap::const_iterator it = eventMap.find(eName);
  return (it != eventMap.end()) ? &(it->second) : NULL;
}


bool EventHandler::IsKnown(const string& eName) const
{
  return GetEventInfo(eName) != NULL;
}


bool EventHandler::IsManaged(const string& eName) const
{
  const EventInfo* ei = GetEventInfo(eName);
  return (ei != NULL) && (ei->GetList() != NULL);
}


bool EventHandler::IsReversed(const std::string& eName) const
{
  const EventInfo* ei = GetEventInfo(eName);
  return (ei != NULL) && ei->IsReversed();
}


bool EventHandler::ReqFullRead(const std::string& eName) const
{
  const EventInfo* ei = GetEventInfo(eName);
  return (ei != NULL) && ei->ReqFullRead();
}


bool EventHandler::ReqInputCtrl(const std::string& eName) const
{
  const EventInfo* ei = GetEventInfo(eName);
  return (ei != NULL) && ei->ReqInputCtrl();
}


/******************************************************************************/
/******************************************************************************/

bool EventHandler::CanUseEvent(EventClient* ec, const EventInfo& ei) const
{
  if ((ei.ReqFullRead()  && !ec->HasFullRead()) ||
      (ei.ReqInputCtrl() && !ec->HasInputCtrl())) {
    return false;
  }
  return true;
}


bool EventHandler::InsertEvent(EventClient* ec, const string& ciName)
{
  EventMap::iterator it = eventMap.find(ciName);
  if (it == eventMap.end()) {
    return false;
  }
  const EventInfo& ei = it->second;
  EventClientList* ecList = ei.GetList();
  if (ecList == NULL) {
    return false;
  }
  if (!CanUseEvent(ec, ei)) {
    return false;
  }
  return ecList->insert(ec);
}


bool EventHandler::RemoveEvent(EventClient* ec, const string& ciName)
{
  EventMap::iterator it = eventMap.find(ciName);
  if ((it == eventMap.end()) || (it->second.GetList() == NULL)) {
    return false;
  }
  return it->second.GetList()->remove(ec);
}


/******************************************************************************/
/******************************************************************************/

void EventHandler::RecvLuaData(int srcPlayerID, int srcScriptID,
                               int dstPlayerID, int dstScriptID,
                               int status, const std::string& data)
{
  EventClientList& list = listRecvLuaData;
  if (list.empty()) { return; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    if ((dstScriptID == 0) || (dstScriptID == ec->GetOrder())) {
      ec->RecvLuaData(srcPlayerID, srcScriptID,
                      dstPlayerID, dstScriptID,
                      status, data);
    }
  }
  list.finish();
}


/******************************************************************************/
/******************************************************************************/

#define DRAW_CALLIN(name)                 \
  void EventHandler:: name ()             \
  {                                       \
    EventClientList& list = list ## name; \
    if (list.empty()) { return; }         \
                                          \
    OpenGLPassState::Enable ## name ();         \
                                          \
    EventClient* ec;                      \
    size_t i = 0;                         \
    list.start(i);                        \
    list.next(i, ec);                     \
    ec-> name ();                         \
                                          \
    while (list.next(i, ec)) {            \
      OpenGLPassState::Reset ## name ();        \
      ec-> name ();                       \
    }                                     \
    list.finish();                        \
                                          \
    OpenGLPassState::Disable ## name ();        \
  }

DRAW_CALLIN(DrawGenesis)
DRAW_CALLIN(DrawWorldStart)
DRAW_CALLIN(DrawWorld)
DRAW_CALLIN(DrawWorldAlpha)
DRAW_CALLIN(DrawWorldShadow)
DRAW_CALLIN(DrawScreenStart)
DRAW_CALLIN(DrawScreen)
DRAW_CALLIN(DrawRadar)


/******************************************************************************/
/******************************************************************************/

bool EventHandler::KeyPress(int key, bool isRepeat)
{
  EventClientList& list = listKeyPress;
  if (list.empty()) { return false; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    if (ec->KeyPress(key, isRepeat)) {
      list.finish();
      return true;
    }
  }
  list.finish();
  return false;
}


bool EventHandler::KeyRelease(int key)
{
  EventClientList& list = listKeyRelease;
  if (list.empty()) { return false; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    if (ec->KeyRelease(key)) {
      list.finish();
      return true;
    }
  }
  list.finish();
  return false;
}


bool EventHandler::MousePress(int x, int y, int button)
{
  EventClientList& list = listMousePress;
  if (list.empty()) { return false; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    if (ec->MousePress(x, y, button)) {
      mouseOwner = ec;
      list.finish();
      return true;
    }
  }
  list.finish();
  return false;
}


bool EventHandler::MouseRelease(int x, int y, int button)
{
  if (mouseOwner == NULL) {
    return false;
  }
  const bool retval = mouseOwner->MouseRelease(x, y, button);
  mouseOwner = NULL;
  return retval;
}


bool EventHandler::MouseMove(int x, int y)
{
  EventClientList& list = listMouseMove;
  if (list.empty()) { return false; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    if (ec->MouseMove(x, y) && (ec == mouseOwner)) {
      list.finish();
      return true;
    }
  }
  list.finish();
  return false;
}


bool EventHandler::MouseWheel(float value)
{
  EventClientList& list = listMouseWheel;
  if (list.empty()) { return false; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    if (ec->MouseWheel(value)) {
      list.finish();
      return true;
    }
  }
  list.finish();
  return false;
}


bool EventHandler::IsAbove(int x, int y)
{
  EventClientList& list = listIsAbove;
  if (list.empty()) { return false; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    if (ec->IsAbove(x, y)) {
      list.finish();
      return true;
    }
  }
  list.finish();
  return false;
}


string EventHandler::GetTooltip(int x, int y)
{
  EventClientList& list = listGetTooltip;
  if (list.empty()) { return ""; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    const string tt = ec->GetTooltip(x, y);
    if (!tt.empty()) {
      list.finish();
      return tt;
    }
  }
  list.finish();
  return "";
}


void EventHandler::WordComplete(const string& line, set<string>& partials)
{
  EventClientList& list = listWordComplete;
  if (list.empty()) { return; }
  size_t i = 0;
  EventClient* ec;
  for (list.start(i); list.next(i, ec); /* no-op */) {
    set<string> safePartials;
    ec->WordComplete(line, safePartials);
    set<string>::const_iterator it;
    for (it = safePartials.begin(); it != safePartials.end(); ++it) {
      const string& partial = *it;
      if (!partial.empty()) {
        partials.insert(partial);
      }
    }
  }
  list.finish();
}


/******************************************************************************/
/******************************************************************************/
