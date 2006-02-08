/* bzflag
* Copyright (c) 1993 - 2006 Tim Riker
*
* This package is free software;  you can redistribute it and/or
* modify it under the terms of the license found in the file
* named COPYING that should have accompanied this file.
*
* THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

// bzflag global header
#include "bzfsClientMessages.h"
#include "BZDBCache.h"
#include "bzfsMessages.h"
#include "bzfsPlayerStateVerify.h"

void handleClientEnter(void **buf, GameKeeper::Player *playerData)
{
	if (!playerData)
		return;

	uint16_t rejectCode;
	char     rejectMsg[MessageLen];

	if (!playerData->player.unpackEnter(*buf, rejectCode, rejectMsg))
	{
		rejectPlayer(playerData->getIndex(), rejectCode, rejectMsg);
		return;
	}

	playerData->accessInfo.setName(playerData->player.getCallSign());
	std::string timeStamp = TimeKeeper::timestamp();
	std::string playerIP = "local.player";
	if ( playerData->netHandler )
		playerIP = playerData->netHandler->getTargetIP();

	DEBUG1("Player %s [%d] has joined from %s at %s with token \"%s\"\n",
		playerData->player.getCallSign(),
		playerData->getIndex(), playerIP.c_str(), timeStamp.c_str(),
		playerData->player.getToken());

	if (!clOptions->publicizeServer)
		playerData->_LSAState = GameKeeper::Player::notRequired;
	else if (strlen(playerData->player.getCallSign()))
		playerData->_LSAState = GameKeeper::Player::required;

	dontWait = true;
}

void handleClientExit ( GameKeeper::Player *playerData )
{
	// data: <none>
	removePlayer(playerData->getIndex(), "left", false);
}

void handleSetVar ( NetHandler *netHandler )
{
	if (!netHandler)
		return;

	void *bufStart = getDirectMessageBuffer();
	PackVars pv(bufStart, netHandler);
	BZDB.iterate(PackVars::packIt, &pv);
}

void handleFlagNegotiation( NetHandler *handler, void **buf, int len )
{
	if (!handler)
		return;

	void *bufStart;
	FlagTypeMap::iterator it;
	FlagSet::iterator m_it;
	FlagOptionMap hasFlag;
	FlagSet missingFlags;
	unsigned short numClientFlags = len/2;

	/* Unpack incoming message containing the list of flags our client supports */
	for (int i = 0; i < numClientFlags; i++)
	{
		FlagType *fDesc;
		*buf = FlagType::unpack(*buf, fDesc);
		if (fDesc != Flags::Null)
			hasFlag[fDesc] = true;
	}

	/* Compare them to the flags this game might need, generating a list of missing flags */
	for (it = FlagType::getFlagMap().begin(); it != FlagType::getFlagMap().end(); ++it)
	{
		if (!hasFlag[it->second])
		{
			if (clOptions->flagCount[it->second] > 0)
				missingFlags.insert(it->second);
			if ((clOptions->numExtraFlags > 0) && !clOptions->flagDisallowed[it->second])
				missingFlags.insert(it->second);
		}
	}

	/* Pack a message with the list of missing flags */
	void *buf2 = bufStart = getDirectMessageBuffer();
	for (m_it = missingFlags.begin(); m_it != missingFlags.end(); ++m_it)
	{
		if ((*m_it) != Flags::Null)
			buf2 = (*m_it)->pack(buf2);
	}
	directMessage(handler, MsgNegotiateFlags, (char*)buf2-(char*)bufStart, bufStart);
}

void handleWorldChunk( NetHandler *handler, void *buf )
{
	uint32_t ptr;	// data: count (bytes read so far)
	buf = nboUnpackUInt(buf, ptr);

	sendWorldChunk(handler, ptr);
}

void handleGameJoinRequest( GameKeeper::Player *playerData )
{
	// player is on the waiting list
	char buffer[MessageLen];
	float waitTime = rejoinList.waitTime(playerData->getIndex());
	if (waitTime > 0.0f)
	{
		snprintf (buffer, MessageLen, "You are unable to begin playing for %.1f seconds.", waitTime);
		sendMessage(ServerPlayer, playerData->getIndex(), buffer);

		// Make them pay dearly for trying to rejoin quickly
		playerAlive(playerData->getIndex());
		playerKilled(playerData->getIndex(), playerData->getIndex(), 0, -1, Flags::Null, -1);
		return;
	}

	// player moved before countdown started
	if (clOptions->timeLimit>0.0f && !countdownActive)
		playerData->player.setPlayedEarly();
	playerAlive(playerData->getIndex());
}


void handlePlayerUpdate ( void **buf, uint16_t &code, GameKeeper::Player *playerData, const void *rawbuf, int len )
{
	if (!playerData)
		return;

	float timestamp;
	PlayerState state;

	*buf = nboUnpackFloat(*buf, timestamp);
	*buf = state.unpack(*buf,code);

	// observer updates are not relayed
	if (playerData->player.isObserver())
	{
		// skip all of the checks
		playerData->setPlayerState(state, timestamp);
		return;
	}

	// silently drop old packet
	if (state.order <= playerData->lastState.order)
		return;

	bool stateValid = validatePlayerState(playerData,state);

	if(!stateValid)
		return;

	playerData->setPlayerState(state, timestamp);

	// Player might already be dead and did not know it yet (e.g. teamkill)
	// do not propogate
	if (!playerData->player.isAlive() && (state.status & short(PlayerState::Alive)))
		return;

	// observer shouldn't send bulk messages anymore, they used to
	// when it was a server-only hack; but the check does not hurt,
	// either
	if (playerData->player.isObserver())
		return;

	searchFlag(*playerData);

	//TODO for server bot, send the update to the handler, since this will skip users
	relayPlayerPacket(playerData->getIndex(), len, rawbuf, code);
		return;
}



// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
