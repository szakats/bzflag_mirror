#include <GL/gl.h>
#include "tga.h"
#include "textures.h"

GLuint textures[6];

void initTextures() {
  glGenTextures(6, textures);
  loadTGA("/usr/local/share/bzedit/data/caution.tga", textures[t_caution]);
  loadTGA("/usr/local/share/bzedit/data/wall.tga", textures[t_wall]);
  loadTGA("/usr/local/share/bzedit/data/pyrwall.tga", textures[t_pyrwall]);
  loadTGA("/usr/local/share/bzedit/data/ground.tga", textures[t_ground]);
  loadTGA("/usr/local/share/bzedit/data/roof.tga", textures[t_roof]);
  loadTGA("/usr/local/share/bzedit/data/boxwall.tga", textures[t_boxwall]);
}
