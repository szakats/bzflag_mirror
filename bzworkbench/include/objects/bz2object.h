#ifndef BZ2OBJECT_H_
#define BZ2OBJECT_H_

#include "../DataEntry.h"
#include "../Transform.h"
#include "../render/Point3D.h"
#include "../model/BZWParser.h"

#include <vector>
#include <string>

using namespace std;

/**
 * This is the base class of a BZW renderable object.  All of these support name, position, rotation, size, matref, phydrv
 * and sets of shear, shift, scale, and spin key/value pairs.  box and pyramid are derived classes of this, 
 * but simply do not support transformations
 */

class bz2object : public DataEntry {
	
	public:
	
		// default constructor
		bz2object(const char* name, const char* keys);
		
		// constructor with data
		bz2object(const char* name, const char* keys, const char* data);
		
		// getter
		string get(void);
		
		// setter
		virtual int update(string& data);
		
		// toString
		virtual string toString(void);
		
		// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
		string BZWLines(void);
		
		// data getters (makes MasterConfigurationDialog code easier)
		Point3D* getPosition() { return &this->position; }
		Point3D* getSize() { return &this->size; }
		string* getName() { return &this->name; }
		float* getRotation() { return &this->rotation; }
		string* getPhyDrv() { return &this->physicsDriver; }
		vector<Transform>* getTransformations() { return &this->transformations; }
		vector<string>* getMaterials() { return &this->materials; }
		bool isSelected() { return this->selected; }
		
		// data setters (makes MasterConfigurationDialog code easier)
		void setPosition( Point3D* position ) { this->position = *position; }
		void setSize( Point3D* s ) { this->size = *s; }
		void setName( const char* name ) { this->name = name; }
		void setRotation( float* rotation ) { this->rotation = *rotation; }
		void setPhyDrv( const char* phydrv ) { this->physicsDriver = phydrv; }
		void setTransforms( vector<Transform>* transformations ) { this->transformations = *transformations; }
		void setMaterials( vector<string>* materials ) { this->materials = *materials; }
		void setSelected( bool value ) { this->selected = value; }
		
	protected:
		Point3D position;
		Point3D size;
		float rotation;
		string name, physicsDriver;
		vector<string> materials;
		vector<Transform> transformations;
		// set true if selected in the 3D scene
		bool selected;
		
};


#endif /*BZ2OBJECT_H_*/
