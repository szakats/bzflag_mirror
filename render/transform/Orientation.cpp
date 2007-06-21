#include "../../include/render/Orientation.h"

// constructor
Orientation::Orientation() : UVN() {
	this->position = Point3D(0.0f, 0.0f, 0.0f);
	this->transformations = vector<TransOp>();	
}

// constructor with position elements
Orientation::Orientation(float x, float y, float z) : UVN() {
	this->position = Point3D(x, y, z);
	this->transformations = vector<TransOp>();	
}

// constructor with point
Orientation::Orientation(Point3D& p) : UVN() {
	this->position = p;
	this->transformations = vector<TransOp>();	
}

// add a transformation to the transformation stack
void Orientation::pushTransOp(TransOp& top) { this->transformations.push_back(top); }

// remove a transformation from the transformation stack
TransOp Orientation::popTransOp(void) { 
	TransOp ret = TransOp();
	if(this->hasTransOps()) {
		ret = this->transformations[ this->transformations.size() - 1 ];
		this->transformations.pop_back();
	}
	return ret;
}

// peek the top of the stack
TransOp Orientation::peekTransOp(void) {
	TransOp ret = TransOp();
	if(this->hasTransOps()) {
		ret = this->transformations[ this->transformations.size() - 1 ];
	}
	return ret;
}

// peek a TransOp from the stack at an arbitrary location
TransOp Orientation::peekTransOp(unsigned int index) {
	TransOp ret = TransOp();
	if(index < 0 || index >= this->transformations.size())
		return ret;
	
	return this->transformations[ index ];		
}

// see if there are any TransOps in the stack
bool Orientation::hasTransOps(void) { return (this->transformations.size() != 0 ? true : false); }

// insert a transformation into the stack
bool Orientation::insertTransOp(TransOp& op, unsigned int index) {
	if(index < 0 || index >= this->transformations.size())
		return false;
	
	this->transformations.insert(this->transformations.begin() + index, op);
	return true;
}

// remove a transformation from the stack
bool Orientation::removeTransOp(unsigned int index) {
	if(index < 0 || index >= this->transformations.size())
		return false;
	
	this->transformations.erase(this->transformations.begin() + index);
	
	return true;
}

// translate by a vector
void Orientation::translate(Vector3D& v) {
	position.x += v.x;
	position.y += v.y;
	position.z += v.z;
}

// translate by a series of points
void Orientation::translate(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
}
