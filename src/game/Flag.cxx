/* bzflag
 * Copyright (c) 1993 - 2001 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "Flag.h"
#include "Pack.h"
#include "Team.h"

const char*				Flag::flagName[] = {
								"Rogue",				// should never be used
								"Red Team",
								"Green Team",
								"Blue Team",
								"Purple Team",
								"High Speed",
								"Quick Turn",
								"Oscillation Overthruster",
								"Rapid Fire",
								"Machine Gun",
								"Guided Missile",
								"Laser",
								"Ricochet",
								"Super Bullet",
								"Invisible Bullet",
								"Stealth",
								"Tiny",
								"Narrow",
								"Shield",
								"Steamroller",
								"Shock Wave",
								"Phantom Zone",
								"Genocide",
								"Jumping",
								"Identify",
								"Cloaking",
								"Colorblindness",
								"Obesity",
								"Left Turn Only",
								"Right Turn Only",
								"Momentum",
								"Blindness",
								"Jamming",
								"Wide Angle"
						};

const char*				Flag::flagAbbv[] = {
								"",						// rogue team
								"",						// red team
								"",						// green team
								"",						// blue team
								"",						// purple team
								"V",						// high speed
								"A",						// quick turn
								"OO",						// oscillation over...
								"F",						// rapid fire
								"MG",						// machine gun
								"GM",						// guided missile
								"L",						// laser
								"R",						// ricochet
								"SB",						// super bullet
								"IB",						// invisible bullet
								"ST",						// stealth
								"T",						// tiny
								"N",						// narrow
								"SH",						// shield
								"SR",						// steamroller
								"SW",						// shock wave
								"PZ",						// phantom zone
								"G",						// genocide
								"JP",						// jumping
								"ID",						// identify
								"CL",						// cloaking
								"CB",						// colorblindness
								"O",						// obesity
								"<-",						// left turn only
								"->",						// right turn only
								"M",						// momentum
								"B",						// blindness
								"JM",						// jamming
								"WA"						// wide angle
						};
const char*				Flag::flagHelp[] = {
"You have no flag.",
"",
"",
"",
"",
"Velocity (+V):  Tank moves faster.  Outrun bad guys.",
"Angular velocity (+A):  Tank turns faster.  Dodge quicker.",
"Oscillation Overthruster (+OO):  Can drive through buildings.  "
				"Can't backup or shoot while inside.",
"rapid Fire (+F):  Shoots more often.  Shells go faster but not as far.",
"Machine Gun (+MG):  Very fast reload and very short range.",
"Guided Missile (+GM):  Shots track a target.  "
				"Lock on with right button.  "
				"Can lock on or retarget after firing.",
"Laser (+L):  Shoots a laser.  Infinite speed and range but long reload time.",
"Ricochet (+R):  Shots bounce off walls.  Don't shoot yourself!",
"SuperBullet (+SB):  Shoots through buildings.  Can kill Phantom Zone.",
"Invisible Bullet (+IB):  Your shots don't appear on other radars.  "
				"Can still see them out window.",
"STealth (+ST):  Tank is invisible on radar.  "
				"Shots are still visible.  Sneak up behind enemies!",
"Tiny (+T):  Tank is small and can get through small openings.  "
				"Very hard to hit.",
"Narrow (+N):  Tank is super thin.  Very hard to hit from front but is "
				"normal size from side.  Can get through small openings.",
"SHield (+SH):  Getting hit only drops flag.  Flag flys an extra-long time.",
"SteamRoller (+SR):  Destroys tanks you touch but you have to get really close.",
"Shock Wave (+SW):  Firing destroys all tanks nearby.  "
				"Don't kill teammates!  Can kill tanks on/in buildings.",
"Phantom Zone (+PZ):  Teleporting toggles Zoned effect.  "
				"Zoned tank can drive through buildings.  "
				"Zoned tank can't shoot or be shot (except by "
				"superbullet and shock wave).",
"Genocide (+G):  Killing one tank kills that tank's whole team.",
"JumPing (+JP):  Tank can jump.  Use Tab key.  Can't steer in the air.",
"IDentify (+ID):  Identifies type of nearest flag.",
"CLoaking (+CL):  Makes your tank invisible out-the-window.  "
				"Still visible on radar.",
"ColorBlindness (-CB):  Can't tell team colors.  Don't shoot teammates!",
"Obesity (-O):  Tank becomes very large.  Can't fit through teleporters.",
"left turn only (- <-):  Can't turn right.",
"right turn only (- ->):  Can't turn left.",
"Momentum (-M):  Tank has inertia.  Acceleration is limited.",
"Blindness (-B):  Can't see out window.  Radar still works.",
"JaMming (-JM):  Radar doesn't work.  Can still see.",
"Wide Angle (-WA):  Fish-eye lens distorts view."
						};

void*					Flag::pack(void* buf) const
{
	buf = nboPackUShort(buf, uint16_t(id));
	buf = nboPackUShort(buf, uint16_t(status));
	buf = nboPackUShort(buf, uint16_t(type));
	buf = owner.pack(buf);
	buf = nboPackVector(buf, position);
	buf = nboPackVector(buf, launchPosition);
	buf = nboPackVector(buf, landingPosition);
	buf = nboPackFloat(buf, flightTime);
	buf = nboPackFloat(buf, flightEnd);
	buf = nboPackFloat(buf, initialVelocity);
	return buf;
}

void*					Flag::unpack(void* buf)
{
	uint16_t data;
	buf = nboUnpackUShort(buf, data); id = FlagId(data);
	buf = nboUnpackUShort(buf, data); status = FlagStatus(data);
	buf = nboUnpackUShort(buf, data); type = FlagType(data);
	buf = owner.unpack(buf);
	buf = nboUnpackVector(buf, position);
	buf = nboUnpackVector(buf, launchPosition);
	buf = nboUnpackVector(buf, landingPosition);
	buf = nboUnpackFloat(buf, flightTime);
	buf = nboUnpackFloat(buf, flightEnd);
	buf = nboUnpackFloat(buf, initialVelocity);
	return buf;
}

const char*				Flag::getName(FlagId id)
{
	return flagName[int(id)];
}

const char*				Flag::getAbbreviation(FlagId id)
{
	return flagAbbv[int(id)];
}

const char*				Flag::getHelp(FlagId id)
{
	return flagHelp[int(id)];
}

FlagType				Flag::getType(FlagId id)
{
	switch (id) {
		case NoFlag:
		case RedFlag:
		case GreenFlag:
		case BlueFlag:
		case PurpleFlag:
		return FlagNormal;
		case ColorblindnessFlag:
		case ObesityFlag:
		case LeftTurnOnlyFlag:
		case RightTurnOnlyFlag:
		case MomentumFlag:
		case BlindnessFlag:
		case JammingFlag:
		case WideAngleFlag:
		return FlagSticky;
		default:
		return FlagUnstable;
	}
}

const float*				Flag::getColor(FlagId id)
{
	static const float superColor[3] = { 1.0, 1.0, 1.0 };
	switch (id) {
		case NoFlag:
		case RedFlag:
		case GreenFlag:
		case BlueFlag:
		case PurpleFlag:
			return Team::getTankColor(TeamColor(id));
		default:
			return superColor;
	}
}
