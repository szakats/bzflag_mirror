/* bzflag
 * Copyright (c) 1993 - 2006 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* interface header */
#include "Score.h"

// bzflag library headers
#include "Pack.h"

float Score::tkKickRatio = 3.0;
int Score::score = 999;
bool Score::randomRanking = false;

Score::Score(): wins( 0 ), losses( 0 ), tks( 0 ){}

void Score::dump()
{
	std::cout << wins << '-' << losses;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

float Score::ranking()
{
	if( randomRanking )
		return ( float )bzfrand();

	// otherwise do score-based ranking
	int sum = wins + losses;
	if( sum == 0 )
		return 0.5;
	float average = ( float )wins / ( float )sum;
	// IIRC that is how wide is the gaussian
	float penalty = ( 1.0f - 0.5f / sqrt(( float )sum ));
	return average *penalty;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

bool Score::isTK()
{
	// arbitrary 3
	return ( tks >= 3 ) && ( tkKickRatio > 0 ) && (( wins == 0 ) || ( tks *100 / wins > tkKickRatio ));
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void Score::tK()
{
	tks++;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void Score::killedBy()
{
	losses++;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void Score::kill()
{
	wins++;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void *Score::pack( void *buf )
{
	buf = nboPackUShort( buf, wins );
	buf = nboPackUShort( buf, losses );
	buf = nboPackUShort( buf, tks );
	return buf;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

bool Score::reached()
{
	return wins - losses >= score;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void Score::setTeamKillRatio( int _tkKickRatio )
{
	tkKickRatio = ( float )_tkKickRatio;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void Score::setWinLimit( int _score )
{
	score = _score;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void Score::setRandomRanking()
{
	randomRanking = true;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int Score::getHandicap()
{
	return losses - wins;
}

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
