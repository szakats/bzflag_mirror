#include "camera.h"

Camera::Camera() {
  fx = fy = fz = 0;
  d = 750;
  rot = 45;
  ele = -75;
  fcamera = true;
}

void Camera::setup() {
  glLoadIdentity();
  glTranslatef(0, 0, -d);
  glRotatef(ele, 1.0, 0.0, 0.0);
  glRotatef(rot, 0.0, 0.0, 1.0);
  if(fcamera)
    glTranslatef(-fx, -fy, -fz);
}
