#ifndef __TGA_H__
#define __TGA_H__

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

bool checkSize(int x);
unsigned char *getRGBA(FILE *s, int size, GLenum &texFormat);
unsigned char *getRGB(FILE *s, int size, GLenum &texFormat);
unsigned char *getGrey(FILE *s, int size, GLenum &texFormat);
unsigned char *getData(FILE *s, int size, int bits, GLenum &texFormat);

#endif
