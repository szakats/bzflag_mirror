/* bzflag
 * Copyright (c) 1993 - 2005 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifdef _MSC_VER
#pragma warning( 4: 4786 )
#endif

// interface headers
#include "TextureManager.h"

// system headers
#include <vector>
#include <string>

// common implementation headers
#include "TextUtils.h"
#include "global.h"
#include "MediaFile.h"
#include "ErrorHandler.h"

#include "OSFile.h"

/*const int NO_VARIANT = (-1); */

// initialize the singleton
template <>
TextureManager* Singleton<TextureManager>::_instance = (TextureManager*)0;

static int noiseProc(ProcTextureInit &init);

ProcTextureInit procLoader[1];

TextureManager::TextureManager()
{
  // fill out the standard proc textures
  procLoader[0].name = "noise";
  procLoader[0].filter = OpenGLTexture::Nearest;
  procLoader[0].proc = noiseProc;

  lastImageID = -1;
  lastBoundID = -1;

  int i, numTextures;
  numTextures = countof(procLoader);

  for (i = 0; i < numTextures; i++) {
    procLoader[i].manager = this;
    procLoader[i].proc(procLoader[i]);
  }
}

TextureManager::~TextureManager()
{
  // we are done remove all textures
  for (TextureNameMap::iterator it = textureNames.begin(); it != textureNames.end(); ++it) {
    ImageInfo &tex = it->second;
    if (tex.texture != NULL) {
      delete tex.texture;
    }
  }
  textureNames.clear();
  textureIDs.clear();
}

int TextureManager::getTextureID( const char* name, bool reportFail )
{
  if (!name) {
    DEBUG2("Could not get texture ID; no provided name\n");
    return -1;
  }
  OSFile	osFilename(name);

  std::string texName = osFilename.getOSName();
  // see if we have the texture
  TextureNameMap::iterator it = textureNames.find(texName);
  if (it != textureNames.end()) {
    return it->second.id;
  } else { // we don't have it so try and load it
    FileTextureInit	file;
    file.filter = OpenGLTexture::LinearMipmapLinear;
    file.name = texName;
    ImageInfo info;
    OpenGLTexture *image = loadTexture(file, reportFail);
    if (!image) {
      DEBUG2("Image not found or unloadable: %s\n", name);
      return -1;
    }
    return addTexture(name, image);
  }
  return -1;
}


bool TextureManager::bind ( int id )
{
  TextureIDMap::iterator it = textureIDs.find(id);
  if (it == textureIDs.end()) {
    DEBUG1("Unable to bind texture (by id): %d\n", id);
    return false;
  }

  if (id != lastBoundID) {
    it->second->texture->execute();
    lastBoundID = id;
  }
  return true;
}


bool TextureManager::bind ( const char* name )
{
  std::string nameStr = name;

  TextureNameMap::iterator it = textureNames.find(nameStr);
  if (it == textureNames.end()) {
    DEBUG1("Unable to bind texture (by name): %s\n", name);
    return false;
  }

  int id = it->second.id;
  if (id != lastBoundID) {
    it->second.texture->execute();
    lastBoundID = id;
  }
  return true;
}


OpenGLTexture::Filter TextureManager::getMaxFilter ( void )
{
  return OpenGLTexture::getMaxFilter();
}

  
std::string TextureManager::getMaxFilterName ( void )
{
  OpenGLTexture::Filter maxFilter = OpenGLTexture::getMaxFilter();
  std::string name = OpenGLTexture::getFilterName(maxFilter);
  return name;
}


void TextureManager::setMaxFilter(std::string filter)
{
  const char** names = OpenGLTexture::getFilterNames();
  for (int i = 0; i < OpenGLTexture::getFilterCount(); i++) {
    if (filter == names[i]) {
      setMaxFilter((OpenGLTexture::Filter) i);
      return;
    }
  }
  DEBUG1 ("setMaxFilter(): bad filter = %s\n", filter.c_str());
}


void TextureManager::setMaxFilter (OpenGLTexture::Filter filter )
{
  OpenGLTexture::setMaxFilter(filter);
  updateTextureFilters();
  return;
}


void TextureManager::updateTextureFilters()
{
  // flush all the textures so they get rebuilt on next use
  TextureNameMap::iterator	itr = textureNames.begin();

  while (itr != textureNames.end()) {
    OpenGLTexture* texture = itr->second.texture;
    OpenGLTexture::Filter current = texture->getFilter();
    texture->setFilter(current);
    itr++;
  }

  // rebuild proc textures
  for (int i = 0; i < (int)countof(procLoader); i++) {
    procLoader[i].manager = this;
    procLoader[i].proc(procLoader[i]);
  }
}


float TextureManager::GetAspectRatio ( int id )
{
  TextureIDMap::iterator it = textureIDs.find(id);
  if (it == textureIDs.end())
    return 0.0;

  return (float)it->second->y/(float)it->second->x;
}

const ImageInfo& TextureManager::getInfo ( int id )
{
 static ImageInfo   crapInfo;
  crapInfo.id = -1;
  TextureIDMap::iterator it = textureIDs.find(id);
  if (it == textureIDs.end())
    return crapInfo;

  return *(it->second);
}
const ImageInfo& TextureManager::getInfo ( const char* name )
{
  static ImageInfo crapInfo;
  crapInfo.id = -1;
  std::string nameStr = name;

  TextureNameMap::iterator it = textureNames.find(nameStr);
  if (it == textureNames.end())
    return crapInfo;

  return it->second;
}

int TextureManager::addTexture( const char* name, OpenGLTexture *texture )
{
  if (!name || !texture)
    return -1;

  // if the texture already exists kill it
  // this is why IDs are way better than objects for this stuff
  TextureNameMap::iterator it = textureNames.find(name);
  if (it != textureNames.end()) {
   DEBUG3("Texture %s already exists, overwriting\n", name);
   textureIDs.erase(textureIDs.find(it->second.id));
   delete it->second.texture;
  }
  ImageInfo info;
  info.name = name;
  info.texture = texture;
  info.id = ++lastImageID;
  info.alpha = texture->hasAlpha();
  info.x = texture->getWidth();
  info.y = texture->getHeight();

  textureNames[name] = info;
  textureIDs[info.id] = &textureNames[name];

  DEBUG4("Added texture %s: id %d\n", name, info.id);

  return info.id;
}

OpenGLTexture* TextureManager::loadTexture(FileTextureInit &init, bool reportFail)
{
  int width, height;
  unsigned char* image = MediaFile::readImage(init.name, &width, &height);

  if (!image) {
    if (reportFail) {
      std::vector<std::string> args;
      args.push_back(init.name);
      printError("cannot load texture: {1}", &args);
    }
    return NULL;
  }

  OpenGLTexture *texture =
    new OpenGLTexture(width, height, image, init.filter, true);
    
  delete[] image;

  return texture;
}


int TextureManager::newTexture(const char* name, int x, int y, unsigned char* data,
                               OpenGLTexture::Filter filter, bool repeat, int format)
{
  return addTexture(name, new OpenGLTexture(x, y, data, filter, repeat, format));
}


void TextureManager::setTextureFilter(int texId, OpenGLTexture::Filter filter)
{
  TextureIDMap::iterator it = textureIDs.find(texId);
  if (it == textureIDs.end()) {
    DEBUG1("setTextureFilter() Couldn't find texid: %i\n", texId);
    return;
  }

  ImageInfo& image = *(it->second);
  OpenGLTexture* texture = image.texture;
  texture->setFilter(filter);

  return;
}
  

OpenGLTexture::Filter TextureManager::getTextureFilter(int texId)
{
  TextureIDMap::iterator it = textureIDs.find(texId);
  if (it == textureIDs.end()) {
    DEBUG1("getTextureFilter() Couldn't find texid: %i\n", texId);
    return OpenGLTexture::Max;
  }
  ImageInfo& image = *(it->second);
  OpenGLTexture* texture = image.texture;

  return texture->getFilter();
}
  

/* --- Procs --- */

int noiseProc(ProcTextureInit &init)
{
  int noizeSize = 128;
  const int size = 4 * noizeSize * noizeSize;
  unsigned char* noise = new unsigned char[size];
  for (int i = 0; i < size; i += 4 ) {
    unsigned char n = (unsigned char)floor(256.0 * bzfrand());
    noise[i+0] = n;
    noise[i+1] = n;
    noise[i+2] = n;
    noise[i+3] = n;
  }
  int texture = init.manager->newTexture(init.name.c_str(), noizeSize, noizeSize, noise, init.filter);
  delete[] noise;
  return texture;
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
