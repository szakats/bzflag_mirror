#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <GL/gl.h>

#define t_boxwall 0
#define t_caution 1
#define t_ground  2
#define t_pyrwall 3
#define t_roof    4
#define t_wall    5
extern GLuint textures[6];

void initTextures();

#endif
