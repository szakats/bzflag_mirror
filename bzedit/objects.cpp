#include "objects.h"
#include "world.h"
#include "textures.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

void Object::set_px(float px) {
  this->px = px;
}

void Object::set_py(float py) {
  this->py = py;
}

void Object::set_pz(float pz) {
  this->pz = pz;
}

void Object::set_sx(float sx) {
  this->sx = sx;
}

void Object::set_sy(float sy) {
  this->sy = sy;
}

void Object::set_sz(float sz) {
  this->sz = sz;
}

void Object::set_angle(float angle) {
  this->angle = angle;
}

float Object::get_px() {
  return px;
}

float Object::get_py() {
  return py;
}

float Object::get_pz() {
  return pz;
}

float Object::get_sx() {
  return sx;
}

float Object::get_sy() {
  return sy;
}

float Object::get_sz() {
  return sz;
}

float Object::get_angle() {
  return angle;
}

void Ground::render(Camera &c) {
  glLoadIdentity();
  c.setup();
  glBindTexture(GL_TEXTURE_2D, textures[t_ground]);
  glBegin(GL_QUADS); {
    glTexCoord2f(0.0, 0.0); glVertex3f(400, -400, 0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-400, -400, 0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-400, 400, 0);
    glTexCoord2f(0.0, 1.0); glVertex3f(400, 400, 0);
  } glEnd();
}

void Walls::render(Camera &c) {
  glLoadIdentity();
  c.setup();
  glBindTexture(GL_TEXTURE_2D, textures[t_wall]);
  glBegin(GL_QUADS); {
    // left
    glTexCoord2f(0.0,  0.0);  glVertex3f(-400, -400, 0);
    glTexCoord2f(20.0, 0.0);  glVertex3f(-400,  400, 0);
    glTexCoord2f(20.0, 1.0); glVertex3f(-400,  400, 8);
    glTexCoord2f(0.0,  1.0); glVertex3f(-400, -400, 8);
    // right
    glTexCoord2f(0.0,  0.0);  glVertex3f(400,  400, 0);
    glTexCoord2f(20.0, 0.0);  glVertex3f(400, -400, 0);
    glTexCoord2f(20.0, 1.0); glVertex3f(400, -400, 8);
    glTexCoord2f(0.0,  1.0); glVertex3f(400,  400, 8);
    // front
    glTexCoord2f(0.0,  0.0);  glVertex3f(400,  -400, 0);
    glTexCoord2f(20.0, 0.0);  glVertex3f(-400, -400, 0);
    glTexCoord2f(20.0, 1.0); glVertex3f(-400, -400, 8);
    glTexCoord2f(0.0,  1.0); glVertex3f(400,  -400, 8);
    // back
    glTexCoord2f(0.0,  0.0);  glVertex3f(-400, 400, 0);
    glTexCoord2f(20.0, 0.0);  glVertex3f(400,  400, 0);
    glTexCoord2f(20.0, 1.0); glVertex3f(400,  400, 8);
    glTexCoord2f(0.0,  1.0); glVertex3f(-400, 400, 8);
  } glEnd();
}

Box::Box() {
  px = py = pz = 0;
  sx = sy = 30;
  sz = 9.42;
  angle = 0;
}

Box::Box(const Box &r) {
  this->px = r.px;
  this->py = r.py;
  this->pz = r.pz;
  this->sx = r.sx;
  this->sy = r.sy;
  this->sz = r.sz;
  this->angle = r.angle;
}

Box Box::operator = (const Box &r) {
  this->px = r.px;
  this->py = r.py;
  this->pz = r.pz;
  this->sx = r.sx;
  this->sy = r.sy;
  this->sz = r.sz;
  this->angle = r.angle;
  return *this;
}

void Box::render(Camera &c, bool transparent) {
  glLoadIdentity();
  c.setup();
  glTranslatef(px, py, pz);
  glRotatef(angle, 0, 0, 1);
  glBindTexture(GL_TEXTURE_2D, textures[t_boxwall]);
  if(transparent) {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glColor4f(1.0, 1.0, 1.0, 0.20);
  }
  glBegin(GL_QUADS); {
    // left
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, -sy, 0);
    glTexCoord2f(sy / 4, 0.0); glVertex3f(-sx, sy, 0);
    glTexCoord2f(sy / 4, sz / 4); glVertex3f(-sx, sy, sz);
    glTexCoord2f(0.0, sz / 4); glVertex3f(-sx, -sy, sz);
    // right
    glTexCoord2f(0.0, 0.0); glVertex3f(sx, sy, 0);
    glTexCoord2f(sy / 4, 0.0); glVertex3f(sx, -sy, 0);
    glTexCoord2f(sy / 4, sz / 4); glVertex3f(sx, -sy, sz);
    glTexCoord2f(0.0, sz / 4); glVertex3f(sx, sy, sz);
    // front
    glTexCoord2f(0.0, 0.0); glVertex3f(sx, -sy, 0);
    glTexCoord2f(sy / 4, 0.0); glVertex3f(-sx, -sy, 0);
    glTexCoord2f(sy / 4, sz / 4); glVertex3f(-sx, -sy, sz);
    glTexCoord2f(0.0, sz / 4); glVertex3f(sx, -sy, sz);
    // back
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, sy, 0);
    glTexCoord2f(sy / 4, 0.0); glVertex3f(sx, sy, 0);
    glTexCoord2f(sy / 4, sz / 4); glVertex3f(sx, sy, sz);
    glTexCoord2f(0.0, sz / 4); glVertex3f(-sx, sy, sz);
  } glEnd();
  glBindTexture(GL_TEXTURE_2D, textures[t_roof]);
  glBegin(GL_QUADS); {
    // bottom
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, -sy, 0);
    glTexCoord2f(sy / 16, 0.0); glVertex3f(-sx, sy, 0);
    glTexCoord2f(sy / 16, sx / 16); glVertex3f(sx, sy, 0);
    glTexCoord2f(0.0, sx / 16); glVertex3f(sx, -sy, 0);
    // top
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, -sy, sz);
    glTexCoord2f(sy / 16, 0.0); glVertex3f(-sx, sy, sz);
    glTexCoord2f(sy / 16, sx / 16); glVertex3f(sx, sy, sz);
    glTexCoord2f(0.0, sx / 16); glVertex3f(sx, -sy, sz);
  } glEnd();
  if(transparent) {
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0, 1.0, 1.0, 1.0);
  }
}

ostream & operator << (ostream &dest, Box &src) {
  dest << "box" << endl;
  dest << "position " << src.px << ' ' << src.py << ' ' << src.pz << endl;
  dest << "rotation " << (src.angle / 180.0 * M_PI) << endl;
  dest << "size " << src.sx << ' ' << src.sy << ' ' << src.sz << endl;
  dest << "end" << endl;
  dest << endl;
  return dest;
}

Pyramid::Pyramid() {
  px = py = pz = 0;
  sx = sy = 8.2;
  sz = 10.25;
  angle = 0;
}

Pyramid::Pyramid(const Pyramid &r) {
  this->px = r.px;
  this->py = r.py;
  this->pz = r.pz;
  this->sx = r.sx;
  this->sy = r.sy;
  this->sz = r.sz;
  this->angle = r.angle;
}

Pyramid Pyramid::operator = (const Pyramid &r) {
  this->px = r.px;
  this->py = r.py;
  this->pz = r.pz;
  this->sx = r.sx;
  this->sy = r.sy;
  this->sz = r.sz;
  this->angle = r.angle;
  return *this;
}

void Pyramid::render(Camera &c, bool transparent) {
  glLoadIdentity();
  c.setup();
  glTranslatef(px, py, pz);
  glRotatef(angle, 0, 0, 1);
  glBindTexture(GL_TEXTURE_2D, textures[t_pyrwall]);
  if(transparent) {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glColor4f(1.0, 1.0, 1.0, 0.20);
  }
  glBegin(GL_TRIANGLES); {
    // bottom
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, -sy, 0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-sx,  sy, 0);
    glTexCoord2f(1.0, 1.0); glVertex3f(sx,   sy, 0);
    glTexCoord2f(1.0, 1.0); glVertex3f(sx,   sy, 0);
    glTexCoord2f(0.0, 1.0); glVertex3f(sx,  -sy, 0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-sx, -sy, 0);
    // left
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, sy,  0);
    glTexCoord2f(0.5, 1.0); glVertex3f(0,   0,   sz);
    glTexCoord2f(1.0, 0.0); glVertex3f(-sx, -sy, 0);
    // right
    glTexCoord2f(0.0, 0.0); glVertex3f(sx, sy,  0);
    glTexCoord2f(0.5, 1.0); glVertex3f(0,  0,   sz);
    glTexCoord2f(1.0, 0.0); glVertex3f(sx, -sy, 0);
    // front
    glTexCoord2f(0.0, 0.0); glVertex3f(sx,  -sy, 0);
    glTexCoord2f(0.5, 1.0); glVertex3f(0,   0,   sz);
    glTexCoord2f(1.0, 0.0); glVertex3f(-sx, -sy, 0);
    // back
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, sy, 0);
    glTexCoord2f(0.5, 1.0); glVertex3f(0,   0,  sz);
    glTexCoord2f(1.0, 0.0); glVertex3f(sx,  sy, 0);
  } glEnd();
  if(transparent) {
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0, 1.0, 1.0, 1.0);
  }
}

ostream & operator << (ostream &dest, Pyramid &src) {
  dest << "pyramid" << endl;
  dest << "position " << src.px << ' ' << src.py << ' ' << src.pz << endl;
  dest << "rotation " << (src.angle / 180.0 * M_PI) << endl;
  dest << "size " << src.sx << ' ' << src.sy << ' ' << src.sz << endl;
  dest << "end" << endl;
  dest << endl;
  return dest;
}

Teleporter::Teleporter() {
  px = py = pz = 0;
  sx = 4.48;
  sy = 0;
  sz = 20.16;
  angle = 0;
  border = 1;
}

Teleporter::Teleporter(const Teleporter &r) {
  this->px = r.px;
  this->py = r.py;
  this->pz = r.pz;
  this->sx = r.sx;
  this->sy = r.sy;
  this->sz = r.sz;
  this->angle = r.angle;
  this->border = r.border;
}

Teleporter Teleporter::operator = (const Teleporter &r) {
  this->px = r.px;
  this->py = r.py;
  this->pz = r.pz;
  this->sx = r.sx;
  this->sy = r.sy;
  this->sz = r.sz;
  this->angle = r.angle;
  this->border = r.border;
  return *this;
}

void Teleporter::render(Camera &c, bool transparent) {
  glLoadIdentity();
  c.setup();
  glTranslatef(px, py, pz);
  glRotatef(angle, 0, 0, 1);
  glEnable(GL_COLOR_MATERIAL);
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.25, 0.25, 0.25);
  if(transparent) {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glColor4f(0.25, 0.25, 0.25, 0.20);
  }
  glBegin(GL_QUADS); {
    glVertex3f(sx, 0, 0); glVertex3f(-sx, 0, 0); glVertex3f(-sx, 0, sz); glVertex3f(sx, 0, sz);
  } glEnd();
  glColor3f(1.0, 1.0, 1.0);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_COLOR_MATERIAL);
  if(transparent) {
    glColor4f(1.0, 1.0, 1.0, 0.20);
  }
  glBindTexture(GL_TEXTURE_2D, textures[t_caution]);
  glBegin(GL_QUADS); {
    ///// first box /////
    // left
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx - border, -border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(-sx - border, border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(-sx - border, border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(-sx - border, -border / 2, sz);
    // right
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(-sx, -border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(-sx, -border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(-sx, border / 2, sz);
    // front
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx, -border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(-sx - border, -border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(-sx - border, -border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(-sx, -border / 2, sz);
    // back
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx - border, border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(-sx, border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(-sx, border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(-sx - border, border / 2, sz);
    // bottom
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx - border, -border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(-sx - border, border / 2, 0);
    glTexCoord2f(border, border); glVertex3f(-sx, border / 2, 0);
    glTexCoord2f(0.0, border); glVertex3f(-sx, -border / 2, 0);
    ///// second box /////
    // left
    glTexCoord2f(0.0, 0.0); glVertex3f(sx, -border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(sx, border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(sx, border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(sx, -border / 2, sz);
    // right
    glTexCoord2f(0.0, 0.0); glVertex3f(sx + border, border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(sx + border, -border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(sx + border, -border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(sx + border, border / 2, sz);
    // front
    glTexCoord2f(0.0, 0.0); glVertex3f(sx + border, -border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(sx, -border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(sx, -border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(sx + border, -border / 2, sz);
    // back
    glTexCoord2f(0.0, 0.0); glVertex3f(sx, border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(sx + border, border / 2, 0);
    glTexCoord2f(border, sz); glVertex3f(sx + border, border / 2, sz);
    glTexCoord2f(0.0, sz); glVertex3f(sx, border / 2, sz);
    // bottom
    glTexCoord2f(0.0, 0.0); glVertex3f(sx, -border / 2, 0);
    glTexCoord2f(border, 0.0); glVertex3f(sx, border / 2, 0);
    glTexCoord2f(border, border); glVertex3f(sx, border / 2, sz);
    glTexCoord2f(0.0, border); glVertex3f(sx, -border / 2, sz);
    ///// third box /////
    // left
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx - border, -border / 2, sz);
    glTexCoord2f(border, 0.0); glVertex3f(-sx - border, border / 2, sz);
    glTexCoord2f(border, border); glVertex3f(-sx - border, border / 2, sz + border);
    glTexCoord2f(0.0, border); glVertex3f(-sx - border, -border / 2, sz + border);
    // right
    glTexCoord2f(0.0, 0.0); glVertex3f(sx + border, border / 2, sz);
    glTexCoord2f(border, 0.0); glVertex3f(sx + border, -border / 2, sz);
    glTexCoord2f(border, border); glVertex3f(sx + border, -border / 2, sz + border);
    glTexCoord2f(0.0, border); glVertex3f(sx + border, border / 2, sz + border);
    // front
    glTexCoord2f(0.0, 0.0); glVertex3f(sx + border, -border / 2, sz);
    glTexCoord2f(sx + border * 2, 0.0); glVertex3f(-sx - border, -border / 2, sz);
    glTexCoord2f(sx + border * 2, border); glVertex3f(-sx - border, -border / 2, sz + border);
    glTexCoord2f(0.0, border); glVertex3f(sx + border, -border / 2, sz + border);
    // back
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx - border, border / 2, sz);
    glTexCoord2f(sx + border * 2, 0.0); glVertex3f(sx + border, border / 2, sz);
    glTexCoord2f(sx + border * 2, border); glVertex3f(sx + border, border / 2, sz + border);
    glTexCoord2f(0.0, border); glVertex3f(-sx - border, border / 2, sz + border);
    // top
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx - border, -border / 2, sz + border);
    glTexCoord2f(border, 0.0); glVertex3f(-sx - border, border / 2, sz + border);
    glTexCoord2f(border, sx); glVertex3f(sx + border, border / 2, sz + border);
    glTexCoord2f(0.0, sx); glVertex3f(sx + border, -border / 2, sz + border);
    // bottom
    glTexCoord2f(0.0, 0.0); glVertex3f(-sx - border, -border / 2, sz);
    glTexCoord2f(border, 0.0); glVertex3f(-sx - border, border / 2, sz);
    glTexCoord2f(border, sx); glVertex3f(sx + border, border / 2, sz);
    glTexCoord2f(0.0, sx); glVertex3f(sx + border, -border / 2, sz);
  } glEnd();
  if(transparent) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 1.0);
  }
}

void Teleporter::set_border(float border) {
  this->border = border;
}

float Teleporter::get_border() {
  return border;
}

ostream & operator << (ostream &dest, Teleporter &src) {
  dest << "teleporter" << endl;
  dest << "position " << src.px << ' ' << src.py << ' ' << src.pz << endl;
  dest << "rotation " << (((src.angle / 180.0) - 90) * M_PI) << endl;
  dest << "size " << src.sy << ' ' << src.sx << ' ' << src.sz << endl;
  dest << "border " << src.border << endl;
  dest << "end" << endl;
  dest << endl;
  return dest;
}

Link::Link() {
  from = "";
  to = "";
}

Link::Link(const Link &r) {
  from = r.from;
  to = r.to;
}

Link Link::operator = (const Link &r) {
  from = r.from;
  to = r.to;
  return *this;
}

void Link::render(Camera &c, World *w) {
  Element fromE = w->getElementByName(from);
  Element toE = w->getElementByName(to);
}

void Link::set_from(string from) {
  this->from = from;
}

void Link::set_to(string to) {
  this->to = to;
}

string Link::get_from() {
  return from;
}

string Link::get_to() {
  return to;
}

ostream & operator << (ostream &dest, Link &src) {
  dest << "link" << endl;
  dest << "from " << src.from << endl;
  dest << "to " << src.to << endl;
  dest << "end" << endl;
  return dest;
}
