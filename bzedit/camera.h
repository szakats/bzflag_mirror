#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/gl.h>

class Camera {
  public:
    Camera();

    void setup();

    float d, rot, ele, fx, fy, fz;
    bool fcamera;
};

#endif
