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
#include "global.h"

#include "bzfsMessages.h"
#include "GameKeeper.h"
#include "bzfs.h"

void sendRemovePlayerMessage ( int playerID )
{
	void *buf, *bufStart = getDirectMessageBuffer();
	buf = nboPackUByte(bufStart, playerID);
	broadcastMessage(MsgRemovePlayer, (char*)buf-(char*)bufStart, bufStart);

	for (int i = 0; i < curMaxPlayers; i++)
	{
		GameKeeper::Player *p = GameKeeper::Player::getPlayerByIndex(i);
		if ((p == NULL) || !p->playerHandler || playerID == p->getIndex())
			continue;
		p->playerHandler->playerRemoved(playerID);
	}
}

void sendFlagUpdateMessage ( int playerID )
{
	GameKeeper::Player *playerData = GameKeeper::Player::getPlayerByIndex(playerID);
	if (!playerData)
		return;

	int result;
	int cnt = 0;

	std::vector<bz_FlagUpdateRecord*> flagRecordList;
	if (playerData->playerHandler)
	{
		for (int flagIndex = 0; flagIndex < numFlags; flagIndex++)
		{
			FlagInfo &flag = *FlagInfo::get(flagIndex);
			if (flag.exist())
			{
				bz_FlagUpdateRecord	*flagRecord = new bz_FlagUpdateRecord;

				bool hide = (flag.flag.type->flagTeam == ::NoTeam) && !isIdentifyFlagIn && (flag.player == -1);
				
				flagRecord->index = flagIndex;

				if (hide)
				{
					flagRecord->type[0] = 'P';
					flagRecord->type[1] = 'Z';
				}
				else
					strncpy(flagRecord->type,flag.flag.type->flagAbbv,2);

				flagRecord->status = flag.flag.status;
				flagRecord->endurance = flag.flag.endurance;
				flagRecord->owner = flag.flag.owner;
				flagRecord->position[0] = flag.flag.position[0];flagRecord->position[1] = flag.flag.position[1];flagRecord->position[2] = flag.flag.position[2];
				flagRecord->launchPosition[0] = flag.flag.launchPosition[0];flagRecord->launchPosition[1] = flag.flag.launchPosition[1];flagRecord->launchPosition[2] = flag.flag.launchPosition[2];
				flagRecord->landingPosition[0] = flag.flag.landingPosition[0];flagRecord->landingPosition[1] = flag.flag.landingPosition[1];flagRecord->landingPosition[2] = flag.flag.landingPosition[2];
				flagRecord->flightTime = flag.flag.flightTime;
				flagRecord->flightEnd = flag.flag.flightEnd;
				flagRecord->initialVelocity = flag.flag.initialVelocity;

				flagRecordList.push_back(flagRecord);
			}
		}

		bz_FlagUpdateRecord**	flagHandle = (bz_FlagUpdateRecord**)malloc(sizeof(bz_FlagUpdateRecord*)*flagRecordList.size());
		for(unsigned int i = 0; i < flagRecordList.size(); i++)
			flagHandle[i] = flagRecordList[i];

		playerData->playerHandler->flagUpdate ( (int)flagRecordList.size(), flagHandle );

		delete(flagHandle);
		for(unsigned int i = 0; i < flagRecordList.size(); i++)
			delete(flagRecordList[i]);
	}
	else
	{
		void *buf, *bufStart = getDirectMessageBuffer();

		buf = nboPackUShort(bufStart,0); //placeholder
		int length = sizeof(uint16_t);
		for (int flagIndex = 0; flagIndex < numFlags; flagIndex++)
		{
			FlagInfo &flag = *FlagInfo::get(flagIndex);
			if (flag.exist())
			{
				if ((length + sizeof(uint16_t) + FlagPLen) > MaxPacketLen - 2*sizeof(uint16_t))
				{
					nboPackUShort(bufStart, cnt);
					result = directMessage(playerData->netHandler, MsgFlagUpdate,(char*)buf - (char*)bufStart, bufStart);

					if (result == -1)
						return;

					cnt    = 0;
					length = sizeof(uint16_t);
					buf    = nboPackUShort(bufStart,0); //placeholder
				}

				bool hide = (flag.flag.type->flagTeam == ::NoTeam) && !isIdentifyFlagIn && (flag.player == -1);
				buf = flag.pack(buf, hide);
				length += sizeof(uint16_t)+FlagPLen;
				cnt++;
			}
		}

		if (cnt > 0)
		{
			nboPackUShort(bufStart, cnt);
			result = directMessage(playerData->netHandler, MsgFlagUpdate,
				(char*)buf - (char*)bufStart, bufStart);
		}
	}
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
