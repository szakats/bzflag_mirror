#ifndef BZ2OBJECT_H_
#define BZ2OBJECT_H_

#include "../render/Renderable.h"
#include "../render/Point3D.h"
#include "../DataEntry.h"
#include "../Transform.h"

#include <vector>
#include <string>

using namespace std;

/**
 * This is the base class of a BZW renderable object.  All of these support name, position, rotation, size, matref, phydrv
 * and sets of shear, shift, scale, and spin key/value pairs.  box and pyramid are derived classes of this, 
 * but simply do not support transformations
 */

class bz2object : public Renderable, public DataEntry {
	
	public:
	
		// default constructor
		bz2object(const char* name, const char* keys);
		
		// constructor with data
		bz2object(const char* name, const char* keys, const char* data);
		
		// constructor with node
		bz2object( const char* name, const char* keys, osg::Node* node );
		
		// constructor with node and data
		bz2object( const char* name, const char* keys, const char* data, osg::Node* node );
		
		// destructor
		virtual ~bz2object() { }
		
		// getter
		string get(void);
		
		// setter
		virtual int update(string& data);
		
		// specific update message
		virtual int update(UpdateMessage& msg) { return 1; }
		
		// toString
		virtual string toString(void);
		
		// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
		string BZWLines(void);
		
		// data getters (makes MasterConfigurationDialog code easier)
		string getPhyDrv() { return this->physicsDriver; }
		vector< osg::ref_ptr<BZTransform> >& getTransformations() { return this->transformations; }
		vector<string>& getMaterials() { return this->materials; }
		bool isSelected() { return this->selected; }
		
		// use this instead of getPosition()
		virtual osg::Vec3 getPos() { return this->getPosition(); }
		
		// use this instead of getScale()
		virtual osg::Vec3 getSize() { return this->getScale(); }
		
		// use this instead of getAttitude()
		virtual osg::Quat getRot() { return this->getAttitude(); }
		
		// use this instead of setPosition()
		virtual void setPos( const osg::Vec3d& newPos ) { this->setPosition( newPos ); }
		
		// use this instead of setScale()
		virtual void setSize( const osg::Vec3d& newSize ) { this->setScale( newSize ); }
		
		// use this instead of setAttitude()
		virtual void setRot( const osg::Quat& newRot ) { this->setAttitude( newRot ); }
		
		// data setters (makes MasterConfigurationDialog code easier)
		void setPhyDrv( const char* phydrv ) { this->physicsDriver = phydrv; }
		void setTransforms( vector< osg::ref_ptr<BZTransform> >& transformations ) { this->transformations = transformations; }
		void setMaterials( vector<string>& materials ) { this->materials = materials; }
		void setSelected( bool value ) { this->selected = value; }
		
		// use this instead of setScale();
		virtual void setSize( osg::Vec3 newSize ) { this->setScale( newSize ); }
		
		// set/set the thisNode
		osg::Node* getThisNode() { return this->thisNode.get(); }
		void setThisNode( osg::Node* node ) { this->thisNode = node; }
		
		// make this public
		bz2object operator =( const bz2object& obj ) { 
			bz2object newObj(NULL, NULL);
			memcpy(&newObj, &obj, sizeof(bz2object));
			return newObj;
		}
		
	protected:
		string physicsDriver;
		vector<string> materials;
		vector< osg::ref_ptr< BZTransform > > transformations;
		// set true if selected in the 3D scene
		bool selected;
		
		// reference to node data inside the Renderable (for changing the transformation stack)
		osg::ref_ptr< osg::Node > thisNode;
		
	private:
		// force these methods to be private, to guarantee that derived classes will use the given replacements
		osg::Vec3f getPosition() { return Renderable::getPosition(); }
		osg::Vec3f getScale() { return Renderable::getScale(); }
		osg::Quat getAttitude() { return Renderable::getAttitude(); }
		
		void setPosition( const osg::Vec3d& newPosition ) { Renderable::setPosition( newPosition ); }
		void setScale( const osg::Vec3d& newScale ) { Renderable::setScale( newScale ); }
		void setAttitude( const osg::Quat& newAttitude ) { Renderable::setAttitude( newAttitude ); }
};

#endif /*BZ2OBJECT_H_*/
