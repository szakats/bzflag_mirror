#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "bz2object.h"
#include <string>
#include <vector>

using namespace std;

/**
 * The 1.x pyramid object.
 */
 
class pyramid : public bz2object {
public:
	
	// default constructor
	pyramid();
		
	// constructor with data
	pyramid(string& data);
	
	// constructor with binary data
	pyramid( osg::Vec3 position, float rotation, osg::Vec3 scale );
		
	static DataEntry* init() { return new pyramid(); }
	static DataEntry* init(string& data) { 
		if( data.size() <= 1 )
			return new pyramid( osg::Vec3( 0.0, 0.0, 0.0 ), 0.0f, osg::Vec3( 10.0, 10.0, 10.0 ) );
		else
			return new pyramid(data);
	}
	// getter
	string get();
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
};

#endif /*PYRAMID_H_*/
