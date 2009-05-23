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
#include "model/Primitives.h"

#include <osg/Vec3>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/Group>
#include <osgDB/ReadFile>

// build a primitive given its name
osg::Node* Primitives::BuildByName( const char* name ) {
	string str(name);

	if (str == "box")
		return BuildBox( &osg::Vec3( 1, 1, 1 ) );
	else
		return NULL;
}

// build a box
osg::Node* Primitives::BuildBox(osg::Vec3* size) {
	osg::Group* group = new osg::Group();
	// separate geometry nodes are needed so that each side
	// can have a separate material
	// array is in the order +x -x +y -y +z -z in bzflag coordinates
	osg::Geode* sideNodes[6];
	for (int i = 0; i < 6; i++)
		sideNodes[i] = new osg::Geode();

	// assign geometry nodes to group
	for (int i = 0; i < 6; i++)
		group->addChild(sideNodes[i]);

	// create geometry and assign it to the nodes
	osg::Geometry* sideGeometry[6];
	for (int i = 0; i < 6; i++) {
		sideGeometry[i] = new osg::Geometry();
		sideNodes[i]->addDrawable(sideGeometry[i]);
	}

	// add vertices for all sides
	osg::Vec3Array* pxVerts = new osg::Vec3Array();
	pxVerts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	pxVerts->push_back( osg::Vec3( size->x(), -size->y(), size->z() ) );
	pxVerts->push_back( osg::Vec3( size->x(), size->y(), size->z() ) );
	pxVerts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	sideGeometry[0]->setVertexArray(pxVerts);

	osg::Vec3Array* nxVerts = new osg::Vec3Array();
	nxVerts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	nxVerts->push_back( osg::Vec3( -size->x(), size->y(), size->z() ) );
	nxVerts->push_back( osg::Vec3( -size->x(), -size->y(), size->z() ) );
	nxVerts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	sideGeometry[1]->setVertexArray(nxVerts);

	osg::Vec3Array* pyVerts = new osg::Vec3Array();
	pyVerts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	pyVerts->push_back( osg::Vec3( size->x(), size->y(), size->z() ) );
	pyVerts->push_back( osg::Vec3( -size->x(), size->y(), size->z() ) );
	pyVerts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	sideGeometry[2]->setVertexArray(pyVerts);

	osg::Vec3Array* nyVerts = new osg::Vec3Array();
	nyVerts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	nyVerts->push_back( osg::Vec3( -size->x(), -size->y(), size->z() ) );
	nyVerts->push_back( osg::Vec3( size->x(), -size->y(), size->z() ) );
	nyVerts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	sideGeometry[3]->setVertexArray(nyVerts);

	osg::Vec3Array* pzVerts = new osg::Vec3Array();
	pzVerts->push_back( osg::Vec3( size->x(), size->y(), size->z() ) );
	pzVerts->push_back( osg::Vec3( size->x(), -size->y(), size->z() ) );
	pzVerts->push_back( osg::Vec3( -size->x(), -size->y(), size->z() ) );
	pzVerts->push_back( osg::Vec3( -size->x(), size->y(), size->z() ) );
	sideGeometry[4]->setVertexArray(pzVerts);

	osg::Vec3Array* nzVerts = new osg::Vec3Array();
	nzVerts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	nzVerts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	nzVerts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	nzVerts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	sideGeometry[5]->setVertexArray(nzVerts);

	// generate UV coordinates
	RebuildBoxUV(group, size);

	osg::DrawElementsUInt* side =
		new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	side->push_back( 0 );
	side->push_back( 1 );
	side->push_back( 2 );
	side->push_back( 3 );

	for (int i = 0; i < 6; i++) {
		sideGeometry[i]->setVertexAttribBinding( 0, osg::Geometry::BIND_PER_VERTEX );
		sideGeometry[i]->addPrimitiveSet(side);
	}

	// load side texture
	osg::Texture2D* sideTexture = new osg::Texture2D();
	sideTexture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* sideImg = osgDB::readImageFile("share/box/boxwall.png");
	// only set image if it was loaded properly
	if (sideImg)
		sideTexture->setImage(sideImg);

	// load roof texture
	osg::Texture2D* roofTexture = new osg::Texture2D();
	sideTexture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* roofImg = osgDB::readImageFile("share/box/roof.png");
	// only set image if it was loaded properly
	if (roofImg)
		roofTexture->setImage(roofImg);

	// make a state set for associating the textures with the box
	osg::StateSet* state[6];
	for (int i = 0; i < 6; i++) {
		state[i] = new osg::StateSet();

		if (i < 4)
			state[i]->setTextureAttributeAndModes( 0, sideTexture, osg::StateAttribute::ON );
		else
			state[i]->setTextureAttributeAndModes( 0, roofTexture, osg::StateAttribute::ON );

		sideNodes[i]->setStateSet(state[i]);
	}

	return group;
}

void Primitives::RebuildBoxUV(osg::Group* box, osg::Vec3* size)
{
	// generate UVs
	osg::Vec2Array* sideUVs[6];
	for (int i = 0; i < 6; i++)
		sideUVs[i] = new osg::Vec2Array();

	float xSideScale = size->x()*2/9.42f;
	float ySideScale = size->y()*2/9.42f;
	float zSideScale = size->z()/9.42f;
	float xTopScale = xSideScale*5;
	float yTopScale = ySideScale*5;

	// +x -x
	sideUVs[0]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[0]->push_back( osg::Vec2( 0, ySideScale ) );
	sideUVs[0]->push_back( osg::Vec2( zSideScale, ySideScale ) );
	sideUVs[0]->push_back( osg::Vec2( zSideScale, 0 ) );
	sideUVs[1]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[1]->push_back( osg::Vec2( 0, ySideScale ) );
	sideUVs[1]->push_back( osg::Vec2( zSideScale, ySideScale ) );
	sideUVs[1]->push_back( osg::Vec2( zSideScale, 0 ) );

	// +y -y
	sideUVs[2]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[2]->push_back( osg::Vec2( 0, xSideScale ) );
	sideUVs[2]->push_back( osg::Vec2( zSideScale, xSideScale ) );
	sideUVs[2]->push_back( osg::Vec2( zSideScale, 0 ) );
	sideUVs[3]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[3]->push_back( osg::Vec2( 0, xSideScale ) );
	sideUVs[3]->push_back( osg::Vec2( zSideScale, xSideScale ) );
	sideUVs[3]->push_back( osg::Vec2( zSideScale, 0 ) );

	// +z -z
	sideUVs[4]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[4]->push_back( osg::Vec2( 0, yTopScale ) );
	sideUVs[4]->push_back( osg::Vec2( xTopScale, yTopScale ) );
	sideUVs[4]->push_back( osg::Vec2( xTopScale, 0 ) );
	sideUVs[5]->push_back( osg::Vec2( 0, 0 ) );
	sideUVs[5]->push_back( osg::Vec2( 0, yTopScale ) );
	sideUVs[5]->push_back( osg::Vec2( xTopScale, yTopScale ) );
	sideUVs[5]->push_back( osg::Vec2( xTopScale, 0 ) );

	for (int i = 0; i < 6; i++) {
		osg::Geode* geode = (osg::Geode*)box->getChild(i);
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable(0);
		geom->setTexCoordArray(0, sideUVs[i]);
	}
}

// build a pyramid
osg::Geode* Primitives::BuildPyramid() {
	return NULL; // FIXME: implement
}

// build a teleporter
osg::Geode* Primitives::BuildTeleporter() {
	return NULL; // FIXME: implement
}

// build a blue base
osg::Geode* Primitives::BuildBlueBase() {
	return NULL; // FIXME: implement
}

// build a green base
osg::Geode* Primitives::BuildGreenBase() {
	return NULL; // FIXME: implement
}

// build a red base
osg::Geode* Primitives::BuildRedBase() {
	return NULL; // FIXME: implement
}

// build a purple base
osg::Geode* Primitives::BuildPurpleBase() {
	return NULL; // FIXME: implement
}
