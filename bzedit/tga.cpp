#include <stl.h>
#include <string>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "tga.h"

GLenum texFormat;

bool checkSize(int x) {
  if(x == 2 || x == 4 || x == 8 || x == 16 || x == 32 || x == 64 || x == 128 || x == 256 || x == 512)
    return true;
  return false;
}

unsigned char *getRGBA(FILE *s, int size) {
  unsigned char *rgba;
  unsigned char temp;
  int bread;
  
  rgba = (unsigned char *) malloc(size * 4);
  if(rgba == NULL)
    return NULL;
  bread = fread(rgba, sizeof(unsigned char), size * 4, s);
  if(bread != size * 4) {
    free(rgba);
    return NULL;
  }
  for(int i = 0; i < size * 4; i += 4) {
    temp = rgba[i];
    rgba[i] = rgba[i + 2];
    rgba[i] = temp;
  }
  texFormat = GL_RGBA;
  return rgba;
}

unsigned char *getRGB(FILE *s, int size) {
  unsigned char *rgb;
  unsigned char temp;
  int bread;

  rgb = (unsigned char *) malloc(size * 3);
  if(rgb == NULL)
    return NULL;
  bread = fread(rgb, sizeof(unsigned char), size * 3, s);
  if(bread != size * 3) {
    free(rgb);
    return NULL;
  }
  for(int i = 0; i < size * 3; i += 3) {
    temp = rgb[i];
    rgb[i] = rgb[i + 2];
    rgb[i + 2] = temp;
  }
  texFormat = GL_RGB;
  return rgb;
}

unsigned char *getGrey(FILE *s, int size) {
  unsigned char *grey;
  int bread;

  grey = (unsigned char *) malloc(size);
  if(grey == NULL)
    return NULL;
  bread = fread(grey, sizeof(unsigned char), size, s);
  if(bread != size) {
    free(grey);
    return NULL;
  }
  texFormat = GL_ALPHA;
  return grey;
}

unsigned char *getData(FILE *s, int size, int bits) {
  switch(bits) {
    case 32:
      return getRGBA(s, size);
    case 24:
      return getRGB(s, size);
    case 8:
      return getGrey(s, size);
  }
}

bool loadTGA(char *name, int id) {
  unsigned char type[4];
  unsigned char info[7];
  unsigned char *imageData = NULL;
  int w, h;
  int bits, size;
  FILE *s;

  if(!(s = fopen(name, "rb")))
    return false;
  fread(&type, sizeof(char), 3, s);
  fseek(s, 12, SEEK_SET);
  fread(&info, sizeof(char), 6, s);
  if(type[1] != 0 || (type[2] != 2 && type[2] != 3))
    return false;
  w = info[0] + info[1] * 256;
  h = info[2] + info[3] * 256;
  bits = info[4];
  size = w * h;
  if(!checkSize(w) || !checkSize(h))
    return false;
  if(bits != 32 && bits != 24 && bits != 8)
    return false;
  imageData = getData(s, size, bits);
  if(imageData == NULL)
    return false;
  fclose(s);
  glBindTexture(GL_TEXTURE_2D, id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D, 0, texFormat, w, h, 0, texFormat, GL_UNSIGNED_BYTE, imageData);
  gluBuild2DMipmaps(GL_TEXTURE_2D, texFormat, w, h, texFormat, GL_UNSIGNED_BYTE, imageData);
  free(imageData);
  return true;
}
