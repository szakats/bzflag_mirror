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

#include "common.h"

// implementation header
#include "MeshDrawMgr.h"

// common headers
#include "bzfgl.h"
#include "OpenGLGState.h"
#include "MeshDrawInfo.h"
#include "bzfio.h" // for DEBUGx()


unsigned int MeshDrawMgr::unloadList = INVALID_GL_LIST_ID;


void MeshDrawMgr::init()
{
}


void MeshDrawMgr::kill()
{
}


MeshDrawMgr::MeshDrawMgr(const MeshDrawInfo* _drawInfo)
{
  drawInfo = _drawInfo;
  if ((drawInfo == NULL) || !drawInfo->isValid()) {
    printf("MeshDrawMgr: invalid drawInfo\n");
    fflush(stdout);
    return;
  } else {
    logDebugMessage(4, "MeshDrawMgr: initializing\n");
    fflush(stdout);
  }

  drawLods = drawInfo->getDrawLods();
  vertices  = (const float*)drawInfo->getVertices();
  normals   = (const float*)drawInfo->getNormals();
  texcoords = (const float*)drawInfo->getTexcoords();

  lodCount = drawInfo->getLodCount();
  lodLists = new LodList[lodCount];

  for (int i = 0; i < lodCount; i++) {
    LodList& lodList = lodLists[i];
    lodList.count = drawLods[i].count;
    lodList.setLists = new unsigned int[lodList.count];
    for (int j = 0; j < lodList.count; j++) {
      lodList.setLists[j] = INVALID_GL_LIST_ID;
    }
  }

  makeLists();
  OpenGLGState::registerContextInitializer(freeContext, initContext, this);
  return;
}


MeshDrawMgr::~MeshDrawMgr()
{
  logDebugMessage(4, "MeshDrawMgr: killing\n");

  OpenGLGState::unregisterContextInitializer(freeContext, initContext, this);
  freeLists();

  for (int i = 0; i < lodCount; i++) {
    delete[] lodLists[i].setLists;
  }
  delete[] lodLists;

  return;
}


inline void MeshDrawMgr::rawExecuteCommands(int lod, int set)
{
  const DrawLod& drawLod = drawLods[lod];
  const DrawSet& drawSet = drawLod.sets[set];
  const int cmdCount = drawSet.count;
  for (int i = 0; i < cmdCount; i++) {
    const DrawCmd& cmd = drawSet.cmds[i];
    glDrawElements(cmd.drawMode, cmd.count, cmd.indexType, cmd.indices);
  }
  return;
}


void MeshDrawMgr::executeSet(int lod, int set,
                             bool useNormals, bool useTexcoords)
{
  // FIXME
  const AnimationInfo* animInfo = drawInfo->getAnimationInfo();
  if (animInfo != NULL) {
    glPushMatrix();
    glRotatef(animInfo->angle, 0.0f, 0.0f, 1.0f);
  }

  const unsigned int list = lodLists[lod].setLists[set];
  if (list != INVALID_GL_LIST_ID) {
    glCallList(list);
  }
  else {
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    if (useNormals) {
      glNormalPointer(GL_FLOAT, 0, normals);
    } else {
      glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (useTexcoords) {
      glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    } else {
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    rawExecuteCommands(lod, set);

    if (!useNormals) {
      glEnableClientState(GL_NORMAL_ARRAY);
    }
    if (!useTexcoords) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }

  if (animInfo != NULL) {
    glPopMatrix();
  }

  return;
}


void MeshDrawMgr::executeSetGeometry(int lod, int set)
{
  // FIXME
  const AnimationInfo* animInfo = drawInfo->getAnimationInfo();
  if (animInfo != NULL) {
    glPushMatrix();
    glRotatef(animInfo->angle, 0.0f, 0.0f, 1.0f);
  }

  const unsigned int list = lodLists[lod].setLists[set];
  if (list != INVALID_GL_LIST_ID) {
    glCallList(list);
  }
  else {
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    rawExecuteCommands(lod, set);
  }

  if (animInfo != NULL) {
    glPopMatrix();
  }

  return;
}


inline void MeshDrawMgr::rawDisableArrays()
{
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  return;
}


void MeshDrawMgr::disableArrays()
{
  if (unloadList == INVALID_GL_LIST_ID) {
    rawDisableArrays();
  } else {
    glCallList(unloadList);
  }
  return;
}


void MeshDrawMgr::makeLists()
{
  GLenum error;
  int errCount = 0;
  // reset the error state
  while (true) {
    error = glGetError();
    if (error == GL_NO_ERROR) {
      break;
    }
    errCount++; // avoid a possible spin-lock?
    if (errCount > 666) {
      logDebugMessage(1, "MeshDrawMgr::makeLists() glError: %i\n", error);
      return; // don't make the lists, something is borked
    }
  };

  if (unloadList == INVALID_GL_LIST_ID) {
    unloadList = glGenLists(1);
    glNewList(unloadList, GL_COMPILE);
    {
      disableArrays();
    }
    glEndList();
    error = glGetError();
    if (error != GL_NO_ERROR) {
      logDebugMessage(1, "MeshDrawMgr::makeLists() unloadList: %i\n", error);
      unloadList = INVALID_GL_LIST_ID;
    }
  }

  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normals);
  glEnableClientState(GL_NORMAL_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);


  for (int lod = 0; lod < lodCount; lod++) {
    const DrawLod& drawLod = drawLods[lod];
    for (int set = 0; set < drawLod.count; set++) {
      const DrawSet& drawSet = drawLod.sets[set];
      if (drawSet.wantList) {
	lodLists[lod].setLists[set] = glGenLists(1);

	glNewList(lodLists[lod].setLists[set], GL_COMPILE);
	{
	  rawExecuteCommands(lod, set);
	}
	glEndList();

	error = glGetError();
	if (error != GL_NO_ERROR) {
	  logDebugMessage(1, "MeshDrawMgr::makeLists() %i/%i glError: %i\n",
		 lod, set, error);
	  lodLists[lod].setLists[set] = INVALID_GL_LIST_ID;
	} else {
	  logDebugMessage(3, "MeshDrawMgr::makeLists() %i/%i created\n", lod, set);
	}
      }
    }
  }

  return;
}


void MeshDrawMgr::freeLists()
{
  if (unloadList != INVALID_GL_LIST_ID) {
    glDeleteLists(unloadList, 1);
    unloadList = INVALID_GL_LIST_ID;
  }

  for (int lod = 0; lod < lodCount; lod++) {
    for (int set = 0; set < lodLists[lod].count; set++) {
      if (lodLists[lod].setLists[set] != INVALID_GL_LIST_ID) {
	glDeleteLists(lodLists[lod].setLists[set], 1);
	lodLists[lod].setLists[set] = INVALID_GL_LIST_ID;
      }
    }
  }

  return;
}


void MeshDrawMgr::initContext(void* data)
{
  ((MeshDrawMgr*)data)->makeLists();
  return;
}


void MeshDrawMgr::freeContext(void* data)
{
  ((MeshDrawMgr*)data)->freeLists();
  return;
}


//============================================================================//

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
