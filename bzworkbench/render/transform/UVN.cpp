#include "../../include/render/transform/UVN.h"

// default constructor
UVN::UVN() {
	this->lookVector = Vector3D(1.0f, 0.0f, 0.0f);
	this->rightVector = Vector3D(0.0f, 1.0f, 0.0f);
	this->upVector = Vector3D(0.0f, 0.0f, 1.0f);
	this->angularOrientation = Vector3D(0.0f, 0.0f, 0.0f);	
	
	this->work_lookVector = lookVector;
	this->work_rightVector = rightVector;
	this->work_upVector = upVector;
}

// construct from "look" vector elements
UVN::UVN(float x, float y, float z) {
	UVN();
	
	this->look_along(x, y, z);
}

// construct from another "look" vector
UVN::UVN(Vector3D& look) {
	UVN();
	
	this->look_along(look);
}

// rotate the "look" and "up" vectors around the "right" vector
void UVN::work_rotate_x(float ax) {
	ax *= (3.141592653589 / 180.0);
	
	float cos_ax = cos(ax);
	float sin_ax = sin(ax);
		
	work_lookVector.x = (float)(work_lookVector.x*cos_ax + work_upVector.x*sin_ax);
	work_lookVector.y = (float)(work_lookVector.y*cos_ax + work_upVector.y*sin_ax);
	work_lookVector.z = (float)(work_lookVector.z*cos_ax + work_upVector.z*sin_ax);
	
	work_lookVector.normalize();
	Vector3D::crossProduct(work_upVector, work_lookVector, work_rightVector);
	
	work_upVector = -work_upVector;
}

// rotate the "right" and "look" vectors around the "up" vector
void UVN::work_rotate_y(float ay) {
	ay *= (3.141592653589 / 180.0);
	
	float cos_ay = cos(ay);
	float sin_ay = sin(ay);
		
	work_lookVector.x = (float)(work_lookVector.x*cos_ay - work_rightVector.x*sin_ay);
	work_lookVector.y = (float)(work_lookVector.y*cos_ay - work_rightVector.y*sin_ay);
	work_lookVector.z = (float)(work_lookVector.z*cos_ay - work_rightVector.z*sin_ay);
	
	work_lookVector.normalize();
	
	Vector3D::crossProduct(work_rightVector, work_lookVector, work_upVector);
}

// rotate the "up" and "right" vectors around the "look" vector
void UVN::work_rotate_z(float az) {
	az *= (3.141592653589 / 180.0);
	
	float cos_az = cos(az);
	float sin_az = sin(az);
		
	work_rightVector.x = (float)(work_rightVector.x*cos_az + work_upVector.x*sin_az);
	work_rightVector.y = (float)(work_rightVector.y*cos_az + work_upVector.y*sin_az);
	work_rightVector.z = (float)(work_rightVector.z*cos_az + work_upVector.z*sin_az);
	
	work_rightVector.normalize();
	
	Vector3D::crossProduct(work_upVector, work_lookVector, work_rightVector);
	
	work_upVector = -work_upVector;
}

// rotate "up" and "look" around "right" by the angle passed
void UVN::rotate_x(float a) {
	this->angularOrientation.x += a;
	
	// reset look and up vectors
	work_lookVector = lookVector;
	work_rightVector = rightVector;
	
	// recompute look and up
	this->work_rotate_x(angularOrientation.x);	
}
void UVN::rotate_right(float a) { this->rotate_x(a); }

// rotate "look" and "right" vectors around "up" by the angle passed
void UVN::rotate_y(float a) {
	this->angularOrientation.y += a;
	
	// reset the look and right vectors
	work_lookVector = lookVector;
	work_rightVector = rightVector;
	
	// recompute look and right
	this->work_rotate_y(angularOrientation.y);
}
void UVN::rotate_up(float a) { this->rotate_y(a); }

// rotate "right" and "up" around "look" by the angle passed
void UVN::rotate_z(float a) {
	this->angularOrientation.z += a;
	
	// reset the right and up vectors
	work_rightVector = rightVector;
	work_upVector = upVector;
	
	// recompute right and up
	this->work_rotate_z(angularOrientation.z);	
}
void UVN::rotate_look(float a) { this->rotate_z(a); }

// set all three vectors based on a new look vector
void UVN::look_along(float x, float y, float z) {
	Vector3D look(x, y, z);
	this->look_along(look);	
}
void UVN::look_along(Vector3D& look) {
	look.normalize();
	
	work_lookVector = look;
	work_upVector = Vector3D(0.0f, 0.0f, 1.0f);
	
	Vector3D::crossProduct(work_rightVector, work_lookVector, work_upVector);
	work_rightVector.normalize();
	
	Vector3D::crossProduct(work_upVector, work_rightVector, work_lookVector);
}

