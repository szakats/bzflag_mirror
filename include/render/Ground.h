#ifndef GROUND_H_
#define GROUND_H_

#include <osg/Geode>

#include "Renderable.h"
#include "../model/SceneBuilder.h"

/**
 * This class stores the ground
 */
class Ground : public Renderable {

public:
	
	// default size
	static const float DEFAULT_SIZE = 400.0f;
	
	// initialize
	Ground( float size );
	
	// uninitialize
	~Ground();
	
	// getters
	float getSize();
	
	// setters
	void setSize( float size );
	
private:

	// this is the ground's grid
	osg::ref_ptr< Renderable > grid;
	
	// build the grid
	osg::ref_ptr< Renderable > buildGrid( float size, float unit );
	
	// the size of the ground
	float size;
};

#endif /*GROUND_H_*/
