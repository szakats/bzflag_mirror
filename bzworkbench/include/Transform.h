/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

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
			this->name = string("");
			this->data = vector<float>();
			this->useThis = true;
		}
		
		BZTransform(string& _data) : DataEntry("", ""), osg::MatrixTransform() {
			this->name = string("");
			this->data = vector<float>();
			this->useThis = true;
			this->update(_data);
		}
		
		BZTransform( string _name, vector<float> _data ) : DataEntry("", ""), osg::MatrixTransform() {
			this->name = _name;
			this->data = _data;
			this->useThis = true;
			this->refreshMatrix();
		} 
		
		BZTransform( string _name, float n1, float n2, float n3, float n4 ) : DataEntry("", ""), osg::MatrixTransform() {
			this->name = _name;
			this->useThis = true;
			this->data = vector<float>();
			this->data.push_back(n1);
			this->data.push_back(n2);
			this->data.push_back(n3);
			this->data.push_back(n4);
			this->refreshMatrix();
		}
		
		// getter
		string get(void) {
			return toString();	
		}
		
		// setter
		int update(string& newData) {
			// expect just one line
			name = BZWParser::key(newData.c_str());
			
			// break up the line
			vector<string> elements = BZWParser::getLineElements(newData.c_str());
			
			// erase the current data
			data.clear();
			
			// get the numbers (don't start with elements.begin(), because this is what name is)
			for(vector<string>::iterator i = elements.begin() + 1; i != elements.end(); i++) {
				data.push_back( atof( i->c_str() ) );
			}
			
			setHeader(name.c_str());
			
			// recompute this matrix
			refreshMatrix();
			
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
		void setName(string& s) { name = s; refreshMatrix(); }
		void setData(const vector<float>& inputData) { data = inputData; refreshMatrix(); }
		void setData( const osg::Vec3d& inputData ) {
			data.clear();
			data.push_back(inputData.x());
			data.push_back(inputData.y());
			data.push_back(inputData.z());
			refreshMatrix();
		}
		
		void setData( const osg::Vec4d& inputData ) {
			data.clear();
			data.push_back(inputData.x());
			data.push_back(inputData.y());
			data.push_back(inputData.z());
			data.push_back(inputData.w());
			refreshMatrix();
		}
		
		// make this operator public
		BZTransform operator =( const BZTransform& obj ) { 
			BZTransform newObj = BZTransform();
			memcpy(&newObj, &obj, sizeof(BZTransform));
			return newObj;
		}
		
		// should this transform be written to the file, or is it just for BZWB's use?
		bool isApplied() { return this->useThis; }
		
		// set the transformation to be used or not used by BZFlag (BZWB uses all transformations)
		void setApplied( bool value ) { this->useThis = value; }
		
	private:
	
		// make this into a shift matrix
		void makeShift() {
			
			osg::Matrixd theMatrix;
			theMatrix.makeTranslate( osg::Vec3( data[0], data[1], data[2] ) );
			
			setMatrix( theMatrix );
		}
		
		// make this into a scale matrix
		void makeScale() {
			osg::Matrixd theMatrix;
			theMatrix.makeScale( data[0], data[1], data[2] );
			setMatrix( theMatrix );
		}
		
		// make this into a spin matrix
		void makeSpin() {
			
			// use a quaternion to compute the "spin" matrix
			osg::Quat quat = osg::Quat( osg::DegreesToRadians(data[0]), osg::Vec3( data[1], data[2], data[3] ) );
			
			osg::Matrix matrix;
			matrix.makeRotate( quat );
			
			setMatrix( matrix );
		}
		
		// make this into a shear matrix
		void makeShear() {
			double matvals[] = {
				1.0,		0.0,		data[2],	0.0,
				0.0,		1.0,		0.0,		0.0,
				data[0],	data[1],	1.0,		0.0,
				0.0,		0.0,		0.0,		1.0
			};
			
			osg::Matrixd matrix = osg::Matrixd( matvals );
			setMatrix( matrix );
		}
		
		
		// member data
		string name;
		vector<float> data;
		vector<string> transformationFormats;
		
		// set to true if this transformation is to be used
		bool useThis;
		
		void refreshMatrix() {
			if( name == "shift" )
				makeShift();
			else if(name == "spin" )
				makeSpin();
			else if(name == "shear" )
				makeShear();
			else if(name == "scale" )
				makeScale();
		}
};

#endif /*TRANSFORM_H_*/
