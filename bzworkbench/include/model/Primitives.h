/*
 * Primitives.h
 *
 *  Created on: May 22, 2009
 *      Author: william
 */

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <osg/Node>
#include <osg/Geode>

#include <string>

using namespace std;

/**
 * Contains methods for building various basic meshes found in bzflag.
 */
class Primitives {

public:

	// build a primitive given its name
	static osg::Node* BuildByName( const char* name );

	// build a box
	static osg::Node* BuildBox(osg::Vec3* size);

	// rebuild UVs for a box, should be called whenever the box is scaled
	static void RebuildBoxUV(osg::Group* box, osg::Vec3* size);

	// build a pyramid
	static osg::Geode* BuildPyramid();

	// build a teleporter
	static osg::Geode* BuildTeleporter();

	// build a blue base
	static osg::Geode* BuildBlueBase();

	// build a green base
	static osg::Geode* BuildGreenBase();

	// build a red base
	static osg::Geode* BuildRedBase();

	// build a purple base
	static osg::Geode* BuildPurpleBase();
};

#endif /* PRIMITIVES_H_ */
