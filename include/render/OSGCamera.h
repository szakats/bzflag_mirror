#ifndef OSGCAMERA_H_
#define OSGCAMERA_H_

#include <osg/CameraNode>
#include "Vector3D.h"

/**
 * This is a wrapper class for osg::CameraNode to make it easier to manipulate
 */
class OSGCamera {
	
public:

	// constructors
	OSGCamera( osg::CameraNode* osgcam );
	
	// methods
	// rotate the camera about its local axes (angle is measured in degrees)
	void rotate_x(double angle);
	void rotate_y(double angle);
	void rotate_z(double angle);
	
	// translate the camera
	void translate( osg::Vec3 newPosition );
	void translate( double x, double y, double z );
	
	// look at a new target
	void lookAt( osg::Vec3 newTarget );
	void lookAt( double x, double y, double z );
	
	// get the osg camera
	osg::CameraNode* get() { return this->osgcam; }
	
private:
	
	// angular orientation
	Vector3D angularOrientation;
	
	// reference to the actual camera
	osg::CameraNode* osgcam;
	
	// rotation matrix (cached)
	osg::Matrixd rotMat;
	
	// position matrix (cached)
	osg::Matrixd transMat;
	
};

#endif /*OSGCAMERA_H_*/
