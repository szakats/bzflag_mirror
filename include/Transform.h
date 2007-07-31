#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <string>
#include <vector>

#include "DataEntry.h"
#include "model/BZWParser.h"

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

using namespace std;

// a metadata entry that is used to describe a transformation such as spin, shear, shift, and scale
class BZTransform : public DataEntry, public osg::MatrixTransform {
	
	public:
	
		BZTransform() : DataEntry("", ""), osg::MatrixTransform() {
			name = string("");
			data = vector<float>();
		}
		
		BZTransform(string& data) : DataEntry("", ""), osg::MatrixTransform() {
			name = string("");
			this->data = vector<float>();
			this->update(data);
		}
		
		BZTransform( string name, float n1, float n2, float n3, float n4 ) : DataEntry("", ""), osg::MatrixTransform() {
			this->name = name;
			this->data = vector<float>();
			data.push_back(n1);
			data.push_back(n2);
			data.push_back(n3);
			data.push_back(n4);
		}
		
		// getter
		string get(void) {
			return this->toString();	
		}
		
		// setter
		int update(string& newData) {
			// expect just one line
			this->name = BZWParser::key(newData.c_str());
			
			// break up the line
			vector<string> elements = BZWParser::getLineElements(newData.c_str());
			
			// erase the current data
			data.clear();
			
			// get the numbers (don't start with elements.begin(), because this is what name is)
			for(vector<string>::iterator i = elements.begin() + 1; i != elements.end(); i++) {
				data.push_back( atof( i->c_str() ) );
			}
			
			this->setHeader(name.c_str());
			
			// recompute this matrix
			if( this->name == "spin" )
				this->makeSpin();
				
			if( this->name == "shift" )
				this->makeShift();
				
			if( this->name == "shear" )
				this->makeShear();
				
			if( this->name == "scale" )
				this->makeScale();
			
			return 1;
		}
		
		// toString
		string toString(void) {
			string ret = string(name);
			for(vector<float>::iterator i = data.begin(); i != data.end(); i++) {
				ret += " " + string(ftoa(*i));
			}
			
			return ret + "\n";
		}
		
		// data getters
		string getName() { return name; }
		vector<float> getData() { return data; }
		
		// data setters
		void setName(string& s) { this->name = s; }
		void setData(vector<float>& data) { this->data = data; }
		
		// make this public
		BZTransform operator =( const BZTransform& obj ) { 
			BZTransform newObj = BZTransform();
			memcpy(&newObj, &obj, sizeof(BZTransform));
			return newObj;
		}
		
	private:
	
		// make this into a shift matrix
		void makeShift() {
			
			osg::Matrixd theMatrix;
			theMatrix.makeTranslate( osg::Vec3( data[0], data[1], data[2] ) );
			
			this->setMatrix( theMatrix );
		}
		
		// make this into a scale matrix
		void makeScale() {
			double matvals[] = {
				data[0],	0.0,		0.0,		0.0,
				0.0,		data[1],	0.0,		0.0,
				0.0,		0.0,		data[2],	0.0,
				0.0,		0.0,		0.0,		1.0
			};
			
			osg::Matrixd theMatrix = osg::Matrixd( matvals );
			this->setMatrix( theMatrix );
		}
		
		// make this into a spin matrix
		void makeSpin() {
			// use a quaternion to compute the "spin" matrix
			osg::Quat quat = osg::Quat( data[0], osg::Vec3( data[1], data[2], data[3] ) );
			
			osg::Matrixd matrix;
			quat.get( matrix );
			
			this->setMatrix( matrix );
		}
		
		// make this into a shear matrix
		void makeShear() {
			double matvals[] = {
				1.0,		0.0,		0.0, 	0.0,
				0.0,		1.0,		0.0, 	0.0,
				0.0,		data[0],	1.0,	0.0,
				0.0,		data[1],	0.0,	1.0
			};
			
			osg::Matrixd theMatrix = osg::Matrixd( matvals );
			this->setMatrix( theMatrix );
		}
		
		
		string name;
		vector<float> data;
		vector<string> transformationFormats;
};

#endif /*TRANSFORM_H_*/
