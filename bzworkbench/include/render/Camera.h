#ifndef CAMERA_H_
#define CAMERA_H_

#include "transform/UVN.h"
#include "Point3D.h"

#include <GL/gl.h>
#include <GL/glu.h>

class Camera : public UVN {

public:

	// constructors
	Camera();
	Camera(Point3D& position);
	Camera(float x, float y, float z);
	
	// methods
	void translate(float x, float y, float z);
	void translate(Vector3D& v);
	
	void look_at(float x, float y, float z);
	void look_at(Point3D& p);
	
	// applies the perspective camera transformation in the 3D rendering pipeline
	void applyPerspectiveCameraTransform(void);
	
	// applies an orthographic camera transformation
	void applyOrthographicCameraTransform(void);
	
private:

	// data
	Point3D position;
};

#endif /*CAMERA_H_*/
