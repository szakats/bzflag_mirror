/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <osg/Node>
#include <osg/Geode>
#include <osg/Texture2D>

#include <string>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

/**
 * Contains methods for building various basic meshes found in bzflag.
 */
class Primitives {

public:

	// build a pyramid
	static osg::Node* buildPyramid( osg::Vec3 size );

	// rebuild UVs for a pyr, should be called whenever the box is scaled
	static void rebuildPyramidUV( osg::Group* pyr, osg::Vec3 size );

	static void rebuildBoxUV(osg::Group* box, osg::Vec3 size);

	static osg::Group* buildUntexturedBox( osg::Vec3 size );
};

#endif /* PRIMITIVES_H_ */
