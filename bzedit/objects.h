#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "camera.h"
#include <iostream.h>
#include <string>

class World;

class Object {
  public:
    void set_px(float px);
    void set_py(float py);
    void set_pz(float pz);
    void set_sx(float sx);
    void set_sy(float sy);
    void set_sz(float sz);
    void set_angle(float angle);

    float get_px();
    float get_py();
    float get_pz();
    float get_sx();
    float get_sy();
    float get_sz();
    float get_angle();
  protected:
    float px, py, pz;
    float sx, sy, sz;
    float angle;
};

class Ground {
  public:
    void render(Camera &c);
};

class Walls {
  public:
    void render(Camera &c);
};

class Box : public Object {
  public:
    Box();
    Box(const Box &r);
    Box operator = (const Box &r);
    void render(Camera &c, bool transparent);
    friend ostream & operator << (ostream &dest, Box &src);
};

class Pyramid : public Object {
  public:
    Pyramid();
    Pyramid(const Pyramid &r);
    Pyramid operator = (const Pyramid &r);
    void render(Camera &c, bool transparent);
    friend ostream & operator << (ostream &dest, Pyramid &src);
};

class Teleporter : public Object {
  public:
    Teleporter();
    Teleporter(const Teleporter &r);
    Teleporter operator = (const Teleporter &r);
    void render(Camera &c, bool transparent);
    void set_border(float border);
    float get_border();
    friend ostream & operator << (ostream &dest, Teleporter &src);
  protected:
    float border;
};

class Link {
  public:
    Link();
    Link(const Link &r);
    Link operator = (const Link &r);
    void render(Camera &c, World *w);
    void set_from(string from);
    void set_from_side(int from_side);
    void set_to(string to);
    void set_to_side(int from_side);
    string get_from();
    int get_from_side();
    string get_to();
    int get_to_side();
    friend ostream & operator << (ostream &dest, Link &src);
  protected:
    string from;
    int from_side;
    string to;
    int to_side;
};

#endif
