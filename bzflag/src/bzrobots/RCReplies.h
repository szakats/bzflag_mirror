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

#ifndef	__RCREPLIES_H__
#define	__RCREPLIES_H__

#include "common.h"

/* common interface headers */
#include <string>

/* local interface headers */
#include "RCReply.h"
#include "RCEvent.h"
#include "Tank.h"

/* bzflag interface headers */
#include "RemotePlayer.h"


/**
 * Remote Control Replies: Encapsulates data and logic associated with
 * replies to the requests made by the frontend
 */
class EventReply : public RCReply
{
  RCEvent *notification;

public:
  EventReply(RCEvent *_notification) : notification(_notification) {}
  EventReply() : notification(NULL) {}
  
  std::string getType() const { return "Event"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;
};


class IdentifyBackend : public RCReply
{
public:
  IdentifyBackend() : version("") {}
  IdentifyBackend(std::string _version) : version(_version) {}

  std::string getType() const { return "IdentifyBackend"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;

private:
  std::string version;
};


class CommandDoneReply : public RCReply
{
public:
  CommandDoneReply(std::string _command) : command(_command) {}
  CommandDoneReply() : command("") {}

  std::string getType() const { return "CommandDone"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;

  std::string command;
};


class GunHeatReply : public RCReply
{
public:
  GunHeatReply() : heat(0.0f) {}
  GunHeatReply(double _heat) : heat(_heat) {}

  std::string getType() const { return "GunHeat"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double heat;
};


class DistanceRemainingReply : public RCReply
{
public:
  DistanceRemainingReply() {}
  DistanceRemainingReply(double _distance) : distance(_distance) {}

  std::string getType() const { return "DistanceRemaining"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;
  
private:
  double distance;
};


class TurnRemainingReply : public RCReply
{
public:
  TurnRemainingReply() {}
  TurnRemainingReply(double _turn) : turn(_turn) {}

  std::string getType() const { return "TurnRemaining"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;
  
private:
  double turn;
};


class TickDurationReply : public RCReply
{
public:
  std::string getType() const { return "TickDuration"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  
private:
  double duration;
};


class TickRemainingReply : public RCReply
{
public:
  std::string getType() const { return "TickRemaining"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;

private:
  double remaining;
};


class BattleFieldSizeReply : public RCReply
{
public:
  BattleFieldSizeReply() {}
  BattleFieldSizeReply(double _size) : size(_size) {}

  std::string getType() const { return "BattleFieldSize"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double size;
};


class XReply : public RCReply
{
public:
  XReply() {}
  XReply(double _x) : x(_x) {}

  std::string getType() const { return "X"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double x;
};


class YReply : public RCReply
{
public:
  YReply() {}
  YReply(double _y) : y(_y) {}

  std::string getType() const { return "Y"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double y;
};


class ZReply : public RCReply
{
public:
  ZReply() {}
  ZReply(double _z) : z(_z) {}

  std::string getType() const { return "Z"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double z;
};


class WidthReply : public RCReply
{
public:
  WidthReply() {}
  WidthReply(double _width) : width(_width) {}

  std::string getType() const { return "Width"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double width;
};


class HeightReply : public RCReply
{
public:
  HeightReply() {}
  HeightReply(double _height) : height(_height) {}

  std::string getType() const { return "Height"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double height;
};


class LengthReply : public RCReply
{
public:
  LengthReply() {}
  LengthReply(double _length) : length(_length) {}

  std::string getType() const { return "Length"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double length;
};


class HeadingReply : public RCReply
{
public:
  HeadingReply() {}
  HeadingReply(double _heading) : heading(_heading) {}

  std::string getType() const { return "Heading"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  double heading;
};


class PlayersBeginReply : public RCReply
{
public:
  PlayersBeginReply() {}

  std::string getType() const { return "PlayersBegin"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;
};


class PlayersReply : public RCReply
{
public:
  PlayersReply() {}
  PlayersReply(RemotePlayer *_tank) : tank(_tank) {}

  std::string getType() const { return "Players"; }

  messageParseStatus parse(char **arguments, int count);
  void getParameters(std::ostream &stream) const;
  bool updateBot(const BZAdvancedRobot *robot) const;

private:
  Tank tank;
};


#endif /* __RCREPLIES_H__ */

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
