#ifndef UPDATEMESSAGE_H_
#define UPDATEMESSAGE_H_

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/ref_ptr>

#include <vector>

using namespace std;

/**
 * This is a simple message class that can be sent to objects in their update() method.
 * 
 * NOTE: the intended use of this messaging class is to inform an object that it was changed externally.
 * It's the job of the update() method to interpret the message and carry out any additional changes
 * specific to the object itself.
 */
 
class BZTransform;
 
class UpdateMessage {
	
public:

	// some common message types
	enum UpdateMessageType {
		SET_POSITION,				// i.e. use this to indicate a new position
		SET_POSITION_FACTOR,		// i.e. use this to indicate a translation
		SET_ROTATION,				// i.e. use this to indicate a new rotation
		SET_ROTATION_FACTOR,		// i.e. use this to indicate a change in rotation
		SET_SCALE,					// i.e. use this to indicate a new scale
		SET_SCALE_FACTOR,			// i.e. use this to indicate a change in scale
		SET_TRANSFORMATIONS			// i.e. use this along with a vector of BZTransforms to set the transformation stack
	};

	int type;
	
	/*
	UpdateMessage( int type, void* data) {
		this->type = type;
		this->data = data;
	}
	*/
	UpdateMessage( int type, osg::Vec3* data ) {
		this->type = type;
		this->vec = data;
		this->data = data;
	}
	
	UpdateMessage( int type, osg::Quat* data ) {
		this->type = type;
		this->quat = data;
		this->data = data;
	}
	
	UpdateMessage( int type, vector< osg::ref_ptr<BZTransform> >* data) {
		this->type = type;
		this->data = data;
		this->transforms = data;
	}
	
	void* get() { return data; }
	
	// message-specific getters (ensure type-safety)
	osg::Vec3* getAsPosition() { return vec; }
	osg::Vec3* getAsPositionFactor() { return vec; }
	osg::Vec3* getAsScale() { return vec; }
	osg::Vec3* getAsScaleFactor() { return vec; }
	osg::Quat* getAsAttitude() { return quat; }
	osg::Vec3* getAsRotation() { return vec; }
	osg::Vec3* getAsRotationFactor() { return vec; }
	vector< osg::ref_ptr<BZTransform> >* getAsTransformationStack() { return transforms; }

private:
	
	osg::Vec3* vec;		// position (factor), scale (factor), and rotation (factor) data
	osg::Quat* quat;	// rotation (factor) data
	vector< osg::ref_ptr<BZTransform> >* transforms;	// transformations
	void* data;			// other data
	
};


#endif /*UPDATEMESSAGE_H_*/
