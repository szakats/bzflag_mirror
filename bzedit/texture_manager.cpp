#include "texture_manager.h"
#include "util.h"
#include "tga.h"
#include "bmp.h"
#include <string.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

TextureManager::TextureManager() {
  lastBindID = -1;
}

TextureManager::~TextureManager() {
  flush();
  TextureList::iterator it = textureList.begin();
  while(it != textureList.end())
    free(*it++);
}

int TextureManager::load(const char *path, bool skin) {
  if(!path)
    return -1;

  TextureInfo *texInfo = (TextureInfo *) malloc(sizeof(TextureInfo));

  char name[255], *slash, extension[32] = {0};
#ifdef WIN32
  slash = strrchr(path, '\\');
#else
  slash = strrchr(path, '/');
#endif

  if(slash)
    slash++;

  if(slash && *slash)
    strcpy(name, slash);
  else
    strcpy(name, path);

  slash = strrchr(name, '.');

  if(slash) {
    strcpy(extension, slash + 1);
    (*slash) = '\0';
  }

  strcpy(texInfo->name, name);
  strcpy(texInfo->file, path);

  unsigned int id = 0;

  if(extension[0] && (stricmp(extension, "BMP") == 0)) {
    id = loadBMP(path, texInfo, skin);
  } else if(extension[0] && (stricmp(extension, "TGA") == 0)) {
    id = loadTGA(path, texInfo, skin);
  } else {
    return -1;
  }

  lastBindID = -1;
  texInfo->glID = id;
  texInfo->manID = textureList.size();

  std::string str = name;
  textureMap[str] = texInfo->manID;
  textureList.push_back(texInfo);

  return texInfo->manID;
}

int TextureManager::getID(const char *name) {
  std::string str = name;
  TextureNameMap::iterator it = textureMap.find(str);

  if(it == textureMap.end())
    return -1;
  else
    return it->second;
}

bool TextureManager::getInfo(int id, TextureInfo *info) {
  if((id < 0) || (id >= textureList.size()) || !info)
    return false;
  info = textureList[id];
  return true;
}

bool TextureManager::bind(int id) {
  if((id < 0) || (id >= textureList.size()))
    return false;
  glBindTexture(GL_TEXTURE_2D, textureList[id]->glID);
  return true;
}

bool TextureManager::flush(int id) {
  if(id == -1) {
    TextureList::iterator it = textureList.begin();

    while(it != textureList.end()) {
      glDeleteTextures(1, &((*it)->glID));
      free(*it);
      it++;
    }
    textureList.clear();
    textureMap.clear();
    return true;
  } else {
    return false;
  }
}

int TextureManager::loadBMP(const char *path, TextureInfo *info, bool skin) {
  long imageH, imageW, offset;

  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;

  unsigned int glID;
  unsigned char *data;
  FILE *fp = fopen(path, "rb");

  if(!fp)
    return -1;

  fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
  fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

  if(infoHeader.biBitCount != 24)
    return -1;

  imageH = infoHeader.biHeight;
  imageW = infoHeader.biWidth;

  data = (unsigned char *) malloc(imageH * imageW * 3);
  offset = fileHeader.bfOffBits;
  fseek(fp, offset, SEEK_SET);

  fread(data, imageH * imageW * 3, 1, fp);
  fclose(fp);

  glGenTextures(1, &glID);
  glBindTexture(GL_TEXTURE_2D, glID);

  int edgeType;

  if(skin)
    edgeType = GL_CLAMP;
  else
    edgeType = GL_REPEAT;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imageW, imageH, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
  
  free(data);
  info->x = imageW;
  info->y = imageH;
  info->alpha = false;

  return glID;
}

int TextureManager::loadTGA(const char *path, TextureInfo *info, bool skin) {
  unsigned char type[4];
  unsigned char tinfo[7];
  unsigned char *imageData = NULL;
  int w, h;
  int bits, size;
  FILE *s;

  if(!(s = fopen(path, "rb")))
    return -1;

  fread(&type, sizeof(char), 3, s);
  fseek(s, 12, SEEK_SET);

  fread(&tinfo, sizeof(char), 6, s);
  if(type[1] != 0 || (type[2] != 2 && type[2] != 3))
    return -1;

  w = tinfo[0] + tinfo[1] * 256;
  h = tinfo[2] + tinfo[3] * 256;

  bits = tinfo[4];
  size = w * h;

  GLenum texFormat;

  if(!checkSize(w) || !checkSize(h))
     return -1;

  if(bits != 32 && bits != 24 && bits != 8)
    return -1;

  imageData = getData(s, size, bits, texFormat);
  if(imageData == NULL)
    return -1;

  fclose(s);

  unsigned int glID;
  glGenTextures(1, &glID);

  glBindTexture(GL_TEXTURE_2D, glID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  int edgeType;

  if(skin)
    edgeType = GL_CLAMP;
  else
    edgeType = GL_REPEAT;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, edgeType);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, edgeType);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glTexImage2D(GL_TEXTURE_2D, 0, texFormat, w, h, 0, texFormat, GL_UNSIGNED_BYTE, imageData);
  gluBuild2DMipmaps(GL_TEXTURE_2D, texFormat, w, h, texFormat, GL_UNSIGNED_BYTE, imageData);

  info->alpha = ((texFormat == GL_RGBA) || (texFormat == GL_ALPHA));
  info->x = w;
  info->y = h;

  free(imageData);
  return glID;
}
