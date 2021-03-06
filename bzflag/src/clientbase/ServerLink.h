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
 * Encapsulates communication between local player and the server.
 */

#ifndef	BZF_SERVER_LINK_H
#define	BZF_SERVER_LINK_H

#include "common.h"

#include <string>
#include "global.h"
#include "Address.h"
#include "Protocol.h"
#include "ShotPath.h"
#include "Flag.h"
#include "BufferedNetworkMessage.h"
#include "vectors.h"

class ServerLink {
public:
  enum State {
    Okay = 0,
    SocketError = 1,
    Rejected = 2,
    BadVersion = 3,
    Hungup = 4,		// only used by Winsock
    CrippledVersion = 5,
    Refused = 6
  };

  enum Abilities {
    Nothing = 0,
    CanDoUDP = 1,
    SendScripts = 2,
    SendTextures = 4,
    HasMessageLink = 8
  };

  ServerLink(const Address& serverAddress,
	     int port = ServerPort);
  ~ServerLink();

  State		getState() const;
  const std::string&	getRejectionMessage() { return rejectionMessage; }
  int			getSocket() const;	// file descriptor actually
  const PlayerId&	getId() const;
  const char*		getVersion() const;

  void		send(uint16_t code, uint16_t len, const void* msg);
  // if millisecondsToBlock < 0 then block forever
  int		read(uint16_t& code, uint16_t& len, void* msg, int millisecondsToBlock = 0);
  int		read(BufferedNetworkMessage *msg, int millisecondsToBlock = 0);

  void		sendCaps(PlayerId id, bool downloads, bool sounds );
  void		sendEnter(PlayerId, PlayerType, NetworkUpdates, TeamColor,
                          const char* name, const char* token, const char* referrer);
  bool		readEnter(std::string& reason,
			  uint16_t& code, uint16_t& rejcode);

  void		sendCaptureFlag(TeamColor);
  void		sendDropFlag(const fvec3& position);
  void		sendKilled(const PlayerId victim,
			   const PlayerId shooter,
			   int reason, int shotId,
			   const FlagType* flag, int phydrv);
  // FIXME -- This is very ugly, but required to build bzadmin with gcc 2.9.5.
  //	  It should be changed to something cleaner.
#ifndef BUILDING_BZADMIN
  void		sendPlayerUpdate(Player*);
  void		sendBeginShot(const FiringInfo&);
#endif
  void		sendEndShot(const PlayerId&, int shotId, int reason);

  void sendHit(const PlayerId &source, const PlayerId &shooter, int shotId);
  void sendVarRequest();

  void		sendAlive(const PlayerId playerId);
  void		sendTeleport(int from, int to);
  void		sendTransferFlag(const PlayerId&, const PlayerId&);
  void		sendNewRabbit();
  void		sendPaused(bool paused);
  void		sendNewPlayer( int botID);
  void		sendCollide(const PlayerId playerId,
			    const PlayerId otherId, const fvec3& pos);

  void		sendExit();
  void		sendAutoPilot(bool autopilot);
  void		sendMessage(const PlayerId& to,
			    char message[MessageLen]);
  void		sendLagPing(char pingRequest[]);
  void		sendUDPlinkRequest();

  void		sendWhatTimeIsIt ( unsigned char tag );

  void		sendCustomData ( const std::string &key, const std::string &value );
  void		sendCustomData ( const char* key, const std::string &value ){if (key) sendCustomData(std::string(key),value);}

  static ServerLink*	getServer(); // const
  static void		setServer(ServerLink*);

  void enableOutboundUDP();
  void confirmIncomingUDP();

  void flush();

private:
  State		state;
  int		fd;

  struct sockaddr	usendaddr;
  int			urecvfd;
  struct sockaddr	urecvaddr; // the clients udp listen address
  bool			ulinkup;

  PlayerId		id;
  char			version[9];
  static ServerLink*	server;
  int			server_abilities;

  std::string	rejectionMessage;

  int		udpLength;
  char	       *udpBufferPtr;
  char		ubuf[MaxPacketLen];

  bool oldNeedForSpeed;
  unsigned int previousFill;
  char txbuf[MaxPacketLen];

};

//
// ServerLink
//

inline ServerLink::State ServerLink::getState() const
{
  return state;
}

inline int		ServerLink::getSocket() const
{
  return fd;
}

inline const PlayerId&	ServerLink::getId() const
{
  return id;
}

inline const char*	ServerLink::getVersion() const
{
  return version;
}

#endif // BZF_SERVER_LINK_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
