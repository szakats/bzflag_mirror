#include <string>
#include <stdio.h>
#include "tga.h"

bool checkSize(int x) {
  if(x == 2 || x == 4 || x == 8 || x == 16 || x == 32 || x == 64 || x == 128 || x == 256 || x == 512)
    return true;
  return false;
}

unsigned char *getRGBA(FILE *s, int size, GLenum &texFormat) {
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

unsigned char *getRGB(FILE *s, int size, GLenum &texFormat) {
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

unsigned char *getGrey(FILE *s, int size, GLenum &texFormat) {
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

unsigned char *getData(FILE *s, int size, int bits, GLenum &texFormat) {
  switch(bits) {
    case 32:
      return getRGBA(s, size, texFormat);
    case 24:
      return getRGB(s, size, texFormat);
    case 8:
      return getGrey(s, size, texFormat);
  }
}
