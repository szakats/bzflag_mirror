#ifndef __TGA_H__
#define __TGA_H__

#include <stdio.h>

bool checkSize(int x);
unsigned char *getRGBA(FILE *s, int size);
unsigned char *getRGB(FILE *s, int size);
unsigned char *getGrey(FILE *s, int size);
unsigned char *getdata(FILE *s, int size, int bits);
bool loadTGA(char *name, int id);

#endif
