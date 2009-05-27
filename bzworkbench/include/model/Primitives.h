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

	// build a primitive given its name
	static osg::Node* BuildByName( const char* name );

	// build a box
	static osg::Node* BuildBox( const osg::Vec3* size );

	// rebuild UVs for a box, should be called whenever the box is scaled
	static void RebuildBoxUV( osg::Group* box, const osg::Vec3* size );

	// build a pyramid
	static osg::Node* BuildPyramid( const osg::Vec3* size );

	// rebuild UVs for a pyr, should be called whenever the box is scaled
	static void RebuildPyramidUV( osg::Geode* pyr, const osg::Vec3* size );

	// build a teleporter
	static osg::Node* BuildTeleporter( const osg::Vec3* size, 
									 const float borderSize );

	// rebuild the UVs for a teleporter, call whenever the size is changed
	static void RebuildTeleporterUV( osg::Group* tele, const osg::Vec3* size, 
									 const float borderSize );

	// rebuild the mesh for a teleporter, call whenever the border size is changed
	static void RebuildTeleporterMesh( osg::Group* tele, const osg::Vec3* size, 
									   const float borderSize );

	// build a blue base
	static osg::Node* BuildBlueBase( const osg::Vec3* size );

	// build a green base
	static osg::Node* BuildGreenBase( const osg::Vec3* size );

	// build a red base
	static osg::Node* BuildRedBase( const osg::Vec3* size );

	// build a purple base
	static osg::Node* BuildPurpleBase( const osg::Vec3* size );

	// regenerate base UVs
	static void RebuildBaseUV( osg::Group* base, const osg::Vec3* size );

	// build a sphere
	static osg::Node* BuildSphere( const osg::Vec3* size, const osg::Vec2* texSize, bool hemisphere, int divisions );

	static void RebuildSphere( osg::Group* sphere, const osg::Vec3* size, const osg::Vec2* texSize, bool hemisphere, int divisions );

private:
	static osg::Group* buildUntexturedBox( const osg::Vec3* size );

	static osg::Group* buildBase( const std::string& sideTextureFile, const std::string& roofTextureFile, const osg::Vec3* size );

	static osg::Texture2D* loadTexture( const std::string& file );

	static void setNodeTexture( osg::Node* node, osg::Texture2D* texture );
};

#endif /* PRIMITIVES_H_ */
