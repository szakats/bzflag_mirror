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
#if defined(_MSC_VER)
// turn off bogus `this used in base member initialization list'
#  pragma warning(disable: 4786)
#endif

#include "global.h"


const struct GlobalDBItem globalDBItems[] = {
  { "_agilityAdVel",		"2.25",				false, StateDatabase::Locked},
  { "_agilityTimeWindow",	"1.0",				false, StateDatabase::Locked},
  { "_agilityVelDelta",		"0.3",				false, StateDatabase::Locked},
  { "_ambientLight",		"none",				false, StateDatabase::Locked},
  { "_angleTolerance",		"0.05",				false, StateDatabase::Locked},
  { "_angularAd",		"1.5",				false, StateDatabase::Locked},
  { "_avenueSize",		"2.0*_boxBase",			false, StateDatabase::Locked},
  { "_baseSize",		"60.0",				false, StateDatabase::Locked},
  { "_boxBase",			"30.0",				false, StateDatabase::Locked},
  { "_boxHeight",		"6.0*_muzzleHeight",		false, StateDatabase::Locked},
  { "_burrowAngularAd",		"0.55",				false, StateDatabase::Locked},
  { "_burrowDepth",		"-1.32",			false, StateDatabase::Locked},
  { "_burrowSpeedAd",		"0.80",				false, StateDatabase::Locked},
  { "_coldetDepth",		"6",				false, StateDatabase::Locked},
  { "_coldetElements",		"4",				false, StateDatabase::Locked},
  { "_cullDepth",		"6",				false, StateDatabase::Locked},
  { "_cullDist",		"fog",				false, StateDatabase::Locked},
  { "_cullElements",		"16",				false, StateDatabase::Locked},
  { "_cullOccluders",		"0",				false, StateDatabase::Locked},
  { "_disableBots",		"0",				false, StateDatabase::Locked},
  { "_drawCelestial",		"1",				false, StateDatabase::Locked},
  { "_drawClouds",		"1",				false, StateDatabase::Locked},
  { "_drawGround",		"1",				false, StateDatabase::Locked},
  { "_drawGroundLights",	"1",				false, StateDatabase::Locked},
  { "_drawMountains",		"1",				false, StateDatabase::Locked},
  { "_drawSky",			"1",				false, StateDatabase::Locked},
  { "_endShotDetection",	"5",				false, StateDatabase::Locked},
  { "_explodeTime",		"5.0",				false, StateDatabase::Locked},
  { "_flagAltitude",		"11.0",				false, StateDatabase::Locked},
  { "_flagEffectTime",		"0.64",				false, StateDatabase::Locked},
  { "_flagHeight",		"10.0",				false, StateDatabase::Locked},
  { "_flagPoleWidth",		"0.025",			false, StateDatabase::Locked},
  { "_flagPoleSize",		"0.8",				false, StateDatabase::Locked},
  { "_flagRadius",		"2.5",				false, StateDatabase::Locked},
  { "_friction",		"0",				false, StateDatabase::Locked},
  { "_fogMode",			"none",				false, StateDatabase::Locked},
  { "_fogDensity",		"0.001",			false, StateDatabase::Locked},
  { "_fogStart",		"0.5*_worldSize",		false, StateDatabase::Locked},
  { "_fogEnd",			"_worldSize",			false, StateDatabase::Locked},
  { "_fogColor",		"0.25 0.25 0.25",		false, StateDatabase::Locked},
  { "_fogNoSky",		"0",				false, StateDatabase::Locked},
  { "_gmActivationTime",	"0.5",				false, StateDatabase::Locked},
  { "_gmAdLife",		"0.95",				false, StateDatabase::Locked},
  { "_gmTurnAngle",		"0.628319",			false, StateDatabase::Locked},
  { "_gravity",			"-9.8",				false, StateDatabase::Locked},
  { "_handicapScoreDiff",	"50.0",				false, StateDatabase::Locked},
  { "_handicapVelAd",		"2.0",				false, StateDatabase::Locked},
  { "_handicapAngAd",		"1.5",				false, StateDatabase::Locked},
  { "_handicapShotAd",		"1.75",				false, StateDatabase::Locked},
  { "_identifyRange",		"50.0",				false, StateDatabase::Locked},
  { "_inertiaLinear",		"50.0",				false, StateDatabase::Locked},
  { "_inertiaAngular",		"38.0",				false, StateDatabase::Locked},
  { "_jumpVelocity",		"19.0",				false, StateDatabase::Locked},
  { "_laserAdLife",		"0.1",				false, StateDatabase::Locked},
  { "_laserAdRate",		"0.5",				false, StateDatabase::Locked},
  { "_laserAdVel",		"13.0",				false, StateDatabase::Locked},
  { "_latitude",		"37.5",				false, StateDatabase::Locked},
  { "_lockOnAngle",		"0.15",				false, StateDatabase::Locked},
  { "_longitude",		"122",				false, StateDatabase::Locked},
  { "_lRAdRate",		"0.5",				false, StateDatabase::Locked},
  { "_maxBumpHeight",		"0.33",				false, StateDatabase::Locked},
  { "_maxFlagGrabs",		"4.0",				false, StateDatabase::Locked},
  { "_maxLOD",			"32767.0",			false, StateDatabase::Locked},
  { "_mirror",			"none",				false, StateDatabase::Locked},
  { "_momentumAngAcc",		"1.0",				false, StateDatabase::Locked},
  { "_momentumLinAcc",		"1.0",				false, StateDatabase::Locked},
  { "_momentumFriction",	"0",				false, StateDatabase::Locked},
  { "_mGunAdLife",		"1.0 / _mGunAdRate",		false, StateDatabase::Locked},
  { "_mGunAdRate",		"10.0",				false, StateDatabase::Locked},
  { "_mGunAdVel",		"1.5",				false, StateDatabase::Locked},
  { "_muzzleFront",		"_tankRadius + 0.1",		false, StateDatabase::Locked},
  { "_muzzleHeight",		"1.57",				false, StateDatabase::Locked},
  { "_noClimb",			"1",				false, StateDatabase::Locked},
  { "_noShadows",		"0",				false, StateDatabase::Locked},
  { "_noSmallPackets",		"0",				false, StateDatabase::Locked},
  { "_notRespondingTime",	"5.0",				false, StateDatabase::Locked},
  { "_obeseFactor",		"2.5",				false, StateDatabase::Locked},
  { "_pauseDropTime",		"15.0",				false, StateDatabase::Locked},
  { "_positionTolerance",	"0.09",				false, StateDatabase::Locked},
  { "_pyrBase",			"4.0*_tankHeight",		false, StateDatabase::Locked},
  { "_pyrHeight",		"5.0*_tankHeight",		false, StateDatabase::Locked},
  { "_rainBaseColor",		"none",				false, StateDatabase::Locked},
  { "_radarLimit",		"_worldSize",			false, StateDatabase::Locked},
  { "_rainRoofs",		"1",				false, StateDatabase::Locked},
  { "_rainDensity",		"none",				false, StateDatabase::Locked},
  { "_rainEndZ",		"none",				false, StateDatabase::Locked},
  { "_rainMaxPuddleTime",	"none",				false, StateDatabase::Locked},
  { "_rainPuddleSpeed",		"none",				false, StateDatabase::Locked},
  { "_rainPuddleColor",		"none",				false, StateDatabase::Locked},
  { "_rainPuddleTexture",	"none",				false, StateDatabase::Locked},
  { "_rainSpread",		"none",				false, StateDatabase::Locked},
  { "_rainSpeed",		"none",				false, StateDatabase::Locked},
  { "_rainSpeedMod",		"none",				false, StateDatabase::Locked},
  { "_rainSpins",		"none",				false, StateDatabase::Locked},
  { "_rainStartZ",		"none",				false, StateDatabase::Locked},
  { "_rainTexture",		"none",				false, StateDatabase::Locked},
  { "_rainTopColor",		"none",				false, StateDatabase::Locked},
  { "_rainType",		"none",				false, StateDatabase::Locked},
  { "_rejoinTime",		"_explodeTime",			false, StateDatabase::Locked},
  { "_reloadTime",		"_shotRange / _shotSpeed",	false, StateDatabase::Locked},
  { "_rFireAdLife",		"1.0 / _rFireAdRate",		false, StateDatabase::Locked},
  { "_rFireAdRate",		"2.0",				false, StateDatabase::Locked},
  { "_rFireAdVel",		"1.5",				false, StateDatabase::Locked},
  { "_shieldFlight",		"2.7",				false, StateDatabase::Locked},
  { "_shockAdLife",		"0.2",				false, StateDatabase::Locked},
  { "_shockInRadius",		"_tankLength",			false, StateDatabase::Locked},
  { "_shockOutRadius",		"60.0",				false, StateDatabase::Locked},
  { "_shotTailLength",		"4.0",				false, StateDatabase::Locked},
  { "_shotRadius",		"0.5",				false, StateDatabase::Locked},
  { "_shotRange",		"350.0",			false, StateDatabase::Locked},
  { "_shotSpeed",		"100.0",			false, StateDatabase::Locked},
  { "_shotsKeepVerticalVelocity",	"0",			false, StateDatabase::Locked},
  { "_skyColor",		"white",			false, StateDatabase::Locked},
  { "_spawnMaxCompTime",	"0.01",				false, StateDatabase::Locked},
  { "_spawnSafeRadMod",		"20",				false, StateDatabase::Locked},
  { "_spawnSafeSRMod",		"3",				false, StateDatabase::Locked},
  { "_spawnSafeSWMod",		"1.5",				false, StateDatabase::Locked},
  { "_srRadiusMult",		"2.0",				false, StateDatabase::Locked},
  { "_squishFactor",		"1.0",				false, StateDatabase::Locked},
  { "_squishTime",		"1.0",				false, StateDatabase::Locked},
  { "_syncTime",		"-1.0",				false, StateDatabase::Locked},
  { "_syncLocation",		"0",				false, StateDatabase::Locked},
  { "_tankExplosionSize",	"3.5 * _tankLength",		false, StateDatabase::Locked},
  { "_tankAngVel",		"0.785398",			false, StateDatabase::Locked},
  { "_tankHeight",		"2.05",				false, StateDatabase::Locked},
  { "_tankLength",		"6.0",				false, StateDatabase::Locked},
  { "_tankRadius",		"0.72 * _tankLength",		false, StateDatabase::Locked},
  { "_tankSpeed",		"25.0",				false, StateDatabase::Locked},
  { "_tankWidth",		"2.8",				false, StateDatabase::Locked},
  { "_targetingAngle",		"0.3",				false, StateDatabase::Locked},
  { "_teleportBreadth",		"4.48",				false, StateDatabase::Locked},
  { "_teleportHeight",		"10.08",			false, StateDatabase::Locked},
  { "_teleportTime",		"1.0",				false, StateDatabase::Locked},
  { "_teleportWidth",		"1.12",				false, StateDatabase::Locked},
  { "_thiefAdLife",		"0.05",				false, StateDatabase::Locked},
  { "_thiefAdRate",		"12.0",				false, StateDatabase::Locked},
  { "_thiefAdShotVel",		"8.0",				false, StateDatabase::Locked},
  { "_thiefTinyFactor",		"0.5",				false, StateDatabase::Locked},
  { "_thiefVelAd",		"1.67",				false, StateDatabase::Locked},
  { "_thiefDropTime",		"_reloadTime * 0.5",		false, StateDatabase::Locked},
  { "_tinyFactor",		"0.4",				false, StateDatabase::Locked},
  { "_trackFade",		"3.0",				false, StateDatabase::Locked},
  { "_updateThrottleRate",	"30.0",				false, StateDatabase::Locked},
  { "_useLineRain",		"none",				false, StateDatabase::Locked},
  { "_useRainPuddles",		"none",				false, StateDatabase::Locked},
  { "_useRainBillboards",	"none",				false, StateDatabase::Locked},
  { "_velocityAd",		"1.5",				false, StateDatabase::Locked},
  { "_wallHeight",		"3.0*_tankHeight",		false, StateDatabase::Locked},
  { "_weapons",			"1",				false, StateDatabase::Locked},
  { "_wideAngleAng",		"1.745329",			false, StateDatabase::Locked},
  { "_wingsGravity",		"_gravity",			false, StateDatabase::Locked},
  { "_wingsJumpCount",		"1",				false, StateDatabase::Locked},
  { "_wingsJumpVelocity",	"_jumpVelocity",		false, StateDatabase::Locked},
  { "_wingsSlideTime",		"0.0",				false, StateDatabase::Locked},
  { "_worldSize",		"800.0",			false, StateDatabase::Locked}

};
const unsigned int	numGlobalDBItems = countof(globalDBItems);

// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
