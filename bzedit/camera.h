#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/gl.h>

class Camera {
  public:
    double d, rot, ele, fx, fy, fz;
    bool fcamera;

    Camera() {
      fx = 0; fy = 0; fz = 0;
      d = 750;
      rot = 45.0;
      ele = 285.0;
      fcamera = true;
    }
    void setup() {
      glTranslatef(0, 0, -d);
      glRotatef(ele, 1.0, 0.0, 0.0);
      glRotatef(rot, 0.0, 0.0, 1.0);
      if(fcamera)
        glTranslatef(-fx, -fy, -fz);
    }
};

#endif
