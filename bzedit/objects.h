#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "camera.h"
#include <iostream.h>

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
    friend ostream & operator << (ostream &dest, Teleporter &src);
    void set_border(float border);
    void set_link1(Teleporter *link1);
    void set_link1_side(int link1_side);
    void set_link2(Teleporter *link2);
    void set_link2_side(int link2_side);
    float get_border();
    Teleporter *get_link1();
    int get_link1_side();
    Teleporter *get_link2();
    int get_link2_side();
  protected:
    float border;
    int link1_side;
    Teleporter *link1;
    int link2_side;
    Teleporter *link2;
};

#endif
