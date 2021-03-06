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

/* interface header */
#include "CustomGroup.h"

/* system headers */
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string.h>

/* common headers */
#include "global.h" // for CtfTeams
#include "ObstacleMgr.h"
#include "ParseColor.h"
#include "PhysicsDriver.h"
#include "TextUtils.h"
#include "BzMaterial.h"


CustomGroup::CustomGroup(const std::string& groupdef)
{
  group = new GroupInstance(groupdef);
  if (groupdef.size() <= 0) {
    std::cout << "WARNING: group instance has no group definition" << std::endl;
  }
  return;
}


CustomGroup::~CustomGroup()
{
  delete group;
  return;
}


bool CustomGroup::read(const char *cmd, std::istream& input) {

  if (strcasecmp(cmd, "team") == 0) {
    int team;
    if (!(input >> team) || (team < 0) || (team >= CtfTeams)) {
      std::cout << "bad team specification" << std::endl;
      return false;
    } else {
      group->setTeam(team);
    }
  }
  else if (strcasecmp(cmd, "tint") == 0) {
    fvec4 tint;
    if (!parseColorStream(input, tint)) {
      std::cout << "bad " << cmd << " specification" << std::endl;
      return false;
    } else {
      group->setTint(tint);
    }
  }
  else if (strcasecmp(cmd, "phydrv") == 0) {
    std::string drvname;
    if (!(input >> drvname)) {
      std::cout << "missing Physics Driver parameter" << std::endl;
      return false;
    }
    int phydrv = PHYDRVMGR.findDriver(drvname);
    if ((phydrv == -1) && (drvname != "-1")) {
      std::cout << "couldn't find PhysicsDriver: " << drvname << std::endl;
    } else {
      group->setPhysicsDriver(phydrv);
    }
  }
  else if (strcasecmp(cmd, "matref") == 0) {
    std::string materialName;
    if (!(input >> materialName)) {
      std::cout << "missing matref parameter" << std::endl;
    }
    const BzMaterial* matref = MATERIALMGR.findMaterial(materialName);
    if ((matref == NULL) && (materialName != "-1")) {
      std::cout << "couldn't find reference material: " << materialName
		<< std::endl;
    } else {
      group->setMaterial(matref);
    }
  }
  else if (strcasecmp(cmd, "matswap") == 0) {
    std::string srcName;
    std::string dstName;
    if (!(input >> srcName) || !(input >> dstName)) {
      std::cout << "missing matswap parameter" << std::endl;
    }
    const BzMaterial* srcMat = MATERIALMGR.findMaterial(srcName);
    const BzMaterial* dstMat = MATERIALMGR.findMaterial(dstName);
    if (srcMat == NULL) {
      std::cout << "couldn't find matswap source: " << srcName
		<< std::endl;
    } else if (dstMat == NULL) {
      std::cout << "couldn't find matswap destination: " << dstName
		<< std::endl;
    } else {
      group->addMaterialSwap(srcMat, dstMat);
    }
  }
  else if (strcasecmp(cmd, "textswap") == 0) {
    std::string line;
    std::getline(input, line);
    input.putback('\n');
    std::vector<std::string> tokens = TextUtils::tokenize(line, " \t", 2);
    if (tokens.size() != 2) {
      std::cout << "invalid textswap parameters" << std::endl;
      return false;
    }
    group->addTextSwap(tokens[0], tokens[1]);
  }
  else if (!WorldFileObstacle::read(cmd, input)) {
    return false;
  }

  return true;
}


void CustomGroup::writeToGroupDef(GroupDefinition *groupDef) const
{
  // include the old style parameters
  MeshTransform xform;
  if ((size.x != 1.0f) || (size.y != 1.0f) || (size.z != 1.0f)) {
    xform.addScale(size);
  }
  if (rotation != 0.0f) {
    const fvec3 zAxis(0.0f, 0.0f, 1.0f);
    xform.addSpin((float)(rotation * (180.0 / M_PI)), zAxis);
  }
  if ((pos.x != 0.0f) || (pos.x != 0.0f) || (pos.z != 0.0f)) {
    xform.addShift(pos);
  }
  xform.append(transform);

  group->setName(name);

  group->setTransform(xform);

  if (driveThrough) { group->setDriveThrough(); }
  if (shootThrough) { group->setShootThrough(); }
  if (ricochet)     { group->setCanRicochet();  }

  // make the group instance
  if (group->getGroupDef().size() > 0) {
    groupDef->addGroupInstance(group);
  } else {
    delete group;
  }
  group = NULL;

  return;
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
