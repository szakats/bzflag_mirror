#include "../include/render/Camera.h"

// constructor
Camera::Camera() : UVN() {
	this->position = Point3D(0.0f, 0.0f, 0.0f);	
}

// constructor with position point
Camera::Camera(Point3D& p) : UVN() {
	this->position = p;	
}

// constructor with position point elements
Camera::Camera(float x, float y, float z) : UVN() {
	this->position = Point3D(x, y, z);	
}

// translate the camera with a vector
void Camera::translate(Vector3D& v) {
	this->position.x += v.x;
	this->position.y += v.y;
	this->position.z += v.z;
}

// translate the camera with vector elements
void Camera::translate(float x, float y, float z) {
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;	
}

// make the camera look at a target
void Camera::look_at(Point3D& p) {
	Vector3D look = Vector3D(p.x - position.x, p.y - position.y, p.z - position.z);
	this->look_along(look);
}

// make the camera look at a target
void Camera::look_at(float x, float y, float z) {
	Vector3D look = Vector3D(x - position.x, y - position.y, z - position.z);
	this->look_along(look);
}

// apply the persepctive camera transformation to the OpenGL matrix stack
void Camera::applyPerspectiveCameraTransform(void) {
	gluLookAt(position.x, position.y, position.z,
			  position.x + work_lookVector.x,
			  position.y + work_lookVector.y,
			  position.z + work_lookVector.z,
			  work_upVector.x, work_upVector.y, work_upVector.z);
}

// apply the orthographic camera transformation to the OpenGL matrix stack
void Camera::applyOrthographicCameraTransform(void) {
	gluLookAt(0, 0, 1,
			  0, 0, -1,
			  0, 1, 0);	
}
