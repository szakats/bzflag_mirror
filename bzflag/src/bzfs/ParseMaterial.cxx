/* bzflag
 * Copyright (c) 1993 - 2004 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* interface header */
#include "BzMaterial.h"
#include "ParseMaterial.h"

/* system headers */
#include <sstream>

/* common implementation headers */
#include "TextureMatrix.h"
#include "DynamicColor.h"


bool parseMaterials(const char* cmd, std::istream& input,
                    BzMaterial* materials, int materialCount, bool& error)
{
  int i;
  error = false;

  if (strcasecmp(cmd, "refmat") == 0) {
    std::string name;
    if (!(input >> name)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    const BzMaterial* matref = MATERIALMGR.findMaterial(name);
    if (matref == NULL) {
      std::cout << "couldn't find reference material: " << name << std::endl;
    } else {
      for (i = 0; i < materialCount; i++) {
        materials[i] = *matref;
      }
    }
  }
  else if (strcasecmp(cmd, "resetmat") == 0) {
    for (i = 0; i < materialCount; i++) {
      materials[i].reset();
    }
  }
  else if (strcasecmp(cmd, "dyncol") == 0) {
    std::string dyncol;
    if (!(input >> dyncol)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    int dynamicColor = DYNCOLORMGR.findColor(dyncol);
    if (dynamicColor == -1) {
      std::cout << "couldn't find dynamicColor: " << dyncol << std::endl;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setDynamicColor(dynamicColor);
    }
  }
  else if (strcasecmp(cmd, "ambient") == 0) {
    float ambient[4];
    if (!(input >> ambient[0] >> ambient[1] >> ambient[2] >> ambient[3])) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setAmbient(ambient);
    }
  }
  else if ((strcasecmp(cmd, "diffuse") == 0) || // currently used by bzflag
           (strcasecmp(cmd, "color") == 0)) {
    float diffuse[4];
    if (!(input >> diffuse[0] >> diffuse[1] >> diffuse[2] >> diffuse[3])) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setDiffuse(diffuse);
    }
  }
  else if (strcasecmp(cmd, "specular") == 0) {
    float specular[4];
    if (!(input >> specular[0] >> specular[1]
                >> specular[2] >> specular[3])) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setSpecular(specular);
    }
  }
  else if (strcasecmp(cmd, "emission") == 0) {
    float emission[4];
    if (!(input >> emission[0] >> emission[1]
                >> emission[2] >> emission[3])) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setEmission(emission);
    }
  }
  else if (strcasecmp(cmd, "shininess") == 0) {
    float shininess;
    if (!(input >> shininess)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setShininess(shininess);
    }
  }
  else if (strcasecmp(cmd, "texture") == 0) {
    std::string name;
    if (!(input >> name)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setTexture(name);
    }
  }
  else if (strcasecmp(cmd, "notextures") == 0) {
    for (i = 0; i < materialCount; i++) {
      materials[i].clearTextures();
    }
  }
  else if (strcasecmp(cmd, "addtexture") == 0) {
    std::string name;
    if (!(input >> name)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].addTexture(name);
    }
  }
  else if (strcasecmp(cmd, "texmat") == 0) {
    std::string texmat;
    if (!(input >> texmat)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    int textureMatrix = TEXMATRIXMGR.findMatrix(texmat);
    if (textureMatrix == -1) {
      std::cout << "couldn't find textureMatrix: " << texmat << std::endl;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setTextureMatrix(textureMatrix);
    }
  }
  else if (strcasecmp(cmd, "notexalpha") == 0) {
    for (i = 0; i < materialCount; i++) {
      materials[i].setUseTextureAlpha(false);
    }
  }
  else if (strcasecmp(cmd, "notexcolor") == 0) {
    for (i = 0; i < materialCount; i++) {
      materials[i].setUseColorOnTexture(false);
    }
  }
  else if (strcasecmp(cmd, "spheremap") == 0) {
    for (i = 0; i < materialCount; i++) {
      materials[i].setUseSphereMap(true);
    }
  }
  else if (strcasecmp(cmd, "shader") == 0) {
    std::string name;
    if (!(input >> name)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].setShader(name);
    }
  }
  else if (strcasecmp(cmd, "addshader") == 0) {
    std::string name;
    if (!(input >> name)) {
      std::cout << "missing " << cmd << " parameters" << std::endl;
      error = true;
    }
    for (i = 0; i < materialCount; i++) {
      materials[i].addShader(name);
    }
  }
  else if (strcasecmp(cmd, "noshaders") == 0) {
    for (i = 0; i < materialCount; i++) {
      materials[i].clearShaders();
    }
  }
  else {
    return false;
  }

  return true;
}


bool parseMaterialsByName(const char* cmd, std::istream& input,
                          BzMaterial* materials, const char** names,
                          int materialCount, bool& error)
{
  error = false;

  for (int n = 0; n < materialCount; n++) {
    if (strcasecmp (cmd, names[n]) == 0) {
      std::string line, matcmd;
      std::getline(input, line);
      std::istringstream parms(line);
      if (!(parms >> matcmd)) {
        error = true;
      } else {
        // put the material command string back into the stream
        for (int i = 0; i < (int)(line.size() - matcmd.size()); i++) {
          input.putback(line[line.size() - i]);
        }
        if (!parseMaterials(matcmd.c_str(), input, &materials[n], 1, error)) {
          error = true;
        }
      }
      return true;
    }
  }

  return false;
}


// Local variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
