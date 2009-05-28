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
#include <osg/StateSet>
#include <osg/Group>
#include <osgDB/ReadFile>

#include <cmath>

using namespace std;

// build a primitive given its name
osg::Node* Primitives::BuildByName( const char* name ) {
	string str(name);

	if (str == "box")
		return BuildBox( &osg::Vec3( 1, 1, 1 ) );
	else if (str == "pyramid")
		return BuildPyramid( &osg::Vec3( 1, 1, 1 ) );
	else if (str == "red_base")
		return BuildRedBase( &osg::Vec3( 1, 1, 1 ) );
	else if (str == "green_base")
		return BuildGreenBase( &osg::Vec3( 1, 1, 1 ) );
	else if (str == "blue_base")
		return BuildBlueBase( &osg::Vec3( 1, 1, 1 ) );
	else if (str == "purple_base")
		return BuildPurpleBase( &osg::Vec3( 1, 1, 1 ) );
	else if (str == "sphere")
		return BuildSphere( &osg::Vec3( 1, 1, 1 ), &osg::Vec2( 1, 1 ), false, 5 );
	else
		return NULL;
}

// build a box
osg::Node* Primitives::BuildBox( const osg::Vec3* size ) {
	osg::Group* group = buildUntexturedBox( size );

	// make UV coordinates
	RebuildBoxUV( group, size );

	// load side texture
	osg::Texture2D* sideTexture = loadTexture( "share/box/boxwall.png" );

	// load roof texture
	osg::Texture2D* roofTexture = loadTexture( "share/box/roof.png" );

	// associate textures with nodes
	for (int i = 0; i < 4; i++)
		setNodeTexture( group->getChild( i ), sideTexture );
	for (int i = 4; i < 6; i++)
		setNodeTexture( group->getChild( i ), roofTexture );

	return group;
}

void Primitives::RebuildBoxUV(osg::Group* box, const osg::Vec3* size)
{
	// generate UVs
	osg::Vec2Array* sideUVs[6];
	for (int i = 0; i < 6; i++)
		sideUVs[i] = new osg::Vec2Array();

	float xSideUV = size->x()*2/9.42f;
	float ySideUV = size->y()*2/9.42f;
	float zSideUV = size->z()/9.42f;
	float xTopUV = xSideUV*5;
	float yTopUV = ySideUV*5;

	// +x -x
	for ( int i = 0; i < 2; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, 0 ) );
	}

	// +y -y
	for ( int i = 2; i < 4; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, 0 ) );
	}

	// +z -z
	for ( int i = 4; i < 6; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, 0 ) );
	}

	for ( int i = 0; i < 6; i++ ) {
		osg::Geode* geode = (osg::Geode*)box->getChild( i );
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable( 0 );
		geom->setTexCoordArray( 0, sideUVs[i] );
	}
}

// build a pyramid
osg::Node* Primitives::BuildPyramid( const osg::Vec3* size ) {
	osg::Geode* pyramid = new osg::Geode();
	osg::Geometry* geometry = new osg::Geometry();
	pyramid->addDrawable( geometry );

	// generate vertices for triangular sides
	osg::Vec3Array* verts = new osg::Vec3Array();
	verts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	verts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	verts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	verts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( 0, 0, size->z() ) );

	// generate verts for base
	verts->push_back( osg::Vec3( -size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), -size->y(), 0 ) );
	verts->push_back( osg::Vec3( size->x(), size->y(), 0 ) );
	verts->push_back( osg::Vec3( -size->x(), size->y(), 0 ) );

	// generate texture coordinates
	RebuildPyramidUV( pyramid, size );

	// generate vertex indices for sides
	osg::DrawElementsUInt* side =
			new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	for (int i = 0; i < 4 * 3; i++) {
		side->push_back( i );
	}

	// generate vertex indices for base
	osg::DrawElementsUInt* bottom =
		new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	bottom->push_back( 12 );
	bottom->push_back( 13 );
	bottom->push_back( 14 );
	bottom->push_back( 15 );

	geometry->setVertexArray( verts );
	geometry->setVertexAttribBinding( 0, osg::Geometry::BIND_PER_VERTEX );
	geometry->addPrimitiveSet( side );
	geometry->addPrimitiveSet( bottom );

	// load the texture from a file
	osg::Texture2D* sideTexture = loadTexture("share/pyramid/pyrwall.png");

	// assign the texture
	setNodeTexture( pyramid, sideTexture );

	return pyramid;
}

// rebuild UVs for a pyr, should be called whenever the box is scaled
void Primitives::RebuildPyramidUV( osg::Geode* pyr, const osg::Vec3* size ) {
	osg::Vec2Array* texcoords = new osg::Vec2Array();

	static const float Scale = 7.55f;

	float dist = size->length();
	float diagonalScale = dist/Scale;
	float xScale = size->x()*2/Scale;
	float yScale = size->y()*2/Scale;

	for ( int i = 0; i < 4; i++ ) {
		if ( i%2 ) {
			texcoords->push_back( osg::Vec2( 0, 0 ) );
			texcoords->push_back( osg::Vec2( yScale, 0 ) );
			texcoords->push_back( osg::Vec2( 0, diagonalScale ) );
		}
		else {
			texcoords->push_back( osg::Vec2( 0, 0 ) );
			texcoords->push_back( osg::Vec2( xScale, 0 ) );
			texcoords->push_back( osg::Vec2( 0, diagonalScale ) );
		}
	}

	texcoords->push_back( osg::Vec2( xScale, yScale ) );
	texcoords->push_back( osg::Vec2( 0, yScale ) );
	texcoords->push_back( osg::Vec2( 0, 0 ) );
	texcoords->push_back( osg::Vec2( xScale, 0 ) );

	osg::Geometry* geometry = (osg::Geometry*)pyr->getDrawable( 0 );
	geometry->setTexCoordArray( 0, texcoords );
}



// build a blue base
osg::Node* Primitives::BuildBlueBase( const osg::Vec3* size ) {
	return buildBase( "share/base/blue_basewall.png", "share/base/blue_basetop.png", size );
}

// build a green base
osg::Node* Primitives::BuildGreenBase( const osg::Vec3* size ) {
	return buildBase( "share/base/green_basewall.png", "share/base/green_basetop.png", size );
}

// build a red base
osg::Node* Primitives::BuildRedBase( const osg::Vec3* size ) {
	return buildBase( "share/base/red_basewall.png", "share/base/red_basetop.png", size );
}

// build a purple base
osg::Node* Primitives::BuildPurpleBase( const osg::Vec3* size ) {
	return buildBase( "share/base/purple_basewall.png", "share/base/purple_basetop.png", size );
}

// regenerate base UVs
void Primitives::RebuildBaseUV( osg::Group* base, const osg::Vec3* size ) {
	// generate UVs
	osg::Vec2Array* sideUVs[6];
	for (int i = 0; i < 6; i++)
		sideUVs[i] = new osg::Vec2Array();

	// FIXME: properly generate side UVs for base
	float xSideUV = 1;
	float ySideUV = 1;
	float zSideUV = 1;
	float xTopUV = 1;
	float yTopUV = 1;

	// +x -x
	for ( int i = 0; i < 2; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, 0 ) );
	}

	// +y -y
	for ( int i = 2; i < 4; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, 0 ) );
	}

	// +z -z
	for ( int i = 4; i < 6; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, 0 ) );
	}

	for ( int i = 0; i < 6; i++ ) {
		osg::Geode* geode = (osg::Geode*)base->getChild( i );
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable( 0 );
		geom->setTexCoordArray( 0, sideUVs[i] );
	}
}

osg::Node* Primitives::BuildSphere( const osg::Vec3* size, const osg::Vec2* texSize, bool hemisphere, int divisions ) {
	osg::Group* group = new osg::Group();
	osg::Geode* outside = new osg::Geode();
	osg::Geode* bottom = new osg::Geode();
	group->addChild( outside );
	group->addChild( bottom );

	RebuildSphere( group, size, texSize, hemisphere, divisions );

	osg::Texture2D* outsideTexture = loadTexture( "share/box/boxwall.png" );
	osg::Texture2D* bottomTexture = loadTexture( "share/box/roof.png" );

	setNodeTexture( outside, outsideTexture );
	setNodeTexture( bottom, bottomTexture );

	return group;
}

void Primitives::RebuildSphere( osg::Group* sphere, const osg::Vec3* size, const osg::Vec2* texSize, bool hemisphere, int divisions ) {
	osg::Geode* outside = (osg::Geode*)sphere->getChild( 0 );
	osg::Geode* bottom = (osg::Geode*)sphere->getChild( 0 );

	if ( outside->getNumDrawables() > 0 )
		outside->removeDrawables( 0 );
	if ( bottom->getNumDrawables() > 0 )
		bottom->removeDrawables( 0 );


	int i, j, q;
	const float minSize = 1.0e-6f; // cheezy / lazy
	int factor = 2;

	// setup the multiplying factor
	if (hemisphere) {
		factor = 1;
	}

	// absolute the sizes
	osg::Vec3 sz( fabsf(size->x()), fabsf(size->y()), fabsf(size->z()) );
	
	// adjust the texture sizes
	osg::Vec2 texsz(texSize->x(), texSize->y());
	if (texsz.x() < 0.0f) {
		// unless you want to do elliptic integrals, here's
		// the Ramanujan approximation for the circumference
		// of an ellipse  (it will be rounded anyways)
		const float circ =
			(float)M_PI * ((3.0f * (sz.x() + sz.y())) -
			sqrtf ((sz.x() + (3.0f * sz.y())) * (sz.y() + (3.0f * sz.x()))));
		// make sure it's an integral number so that the edges line up
		texsz._v[0] = -floorf(circ / texsz.x());
	}
	if (texsz.y() < 0.0f) {
		texsz._v[1] = -((2.0f * sz.z()) / texsz.y());
	}

	// validity checking
	if ((divisions < 1) ||
		(texsz.x() < minSize) || (texsz.y() < minSize) ||
		(sz.x() < minSize) || (sz.y() < minSize) || (sz.z() < minSize)) {
		return;
	}

	// setup the coordinates
	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::Vec2Array* texcoords = new osg::Vec2Array();

	osg::Vec3 v;
	osg::Vec2 t;

	// the center vertices
	v.set( 0, 0, sz.z() );
	vertices->push_back(v);
	if (!hemisphere) {
		v._v[2] = -sz.z();
		vertices->push_back(v);
	}
	t.set( 0.5f, 1.0f );
	texcoords->push_back( t );
	if (!hemisphere) {
		t._v[1] = 0.0f;
		texcoords->push_back(t);
	}
	
	// the rest of the vertices
	for ( i = 0; i < divisions; i++ ) {
		for ( j = 0; j < (4 * (i + 1)); j++ ) {

			float h_angle = (float)((M_PI * 2.0) * j / (4 * (i + 1)));
			h_angle = h_angle;
			float v_angle = (float)((M_PI / 2.0) *
				(divisions - i - 1) / (divisions));
			osg::Vec3 unit;
			unit.set( cosf(h_angle) * cosf(v_angle),
					  sinf(h_angle) * cosf(v_angle),
					  sinf(v_angle) );
			// vertex
			v = osg::Vec3(sz.x() * unit.x(), sz.y() * unit.y(), sz.z() * unit.z());
			vertices->push_back( v );
			// texcoord
			t.set( (float)j / (float)(4 * (i + 1)) * texsz.x(),
				   (float)(divisions - i - 1) / (float)divisions );
			if ( !hemisphere ) {
				t._v[1] = 0.5f + (0.5f * t.y());
			}
			t._v[1] = t.y() * texsz.y();
			texcoords->push_back( t );

			// the bottom hemisphere
			if ( !hemisphere ) {
				if ( i != (divisions - 1) ) {
					// vertex
					v._v[2] = -v.z();
					vertices->push_back( v );
					// texcoord
					t._v[1] = texsz.y() - t.y();
					texcoords->push_back( t );
				}
			}
		}
	}

	// the closing strip of texture coordinates
	const int texStripOffset = texcoords->size();
	t.set( texsz.x() * 0.5f, // weirdness
		   texsz.y() * 1.0f );
	texcoords->push_back( t );
	if (!hemisphere) {
		t._v[1] = 0.0f;
		texcoords->push_back( t );
	}
	for (i = 0; i < divisions; i++) {
		t.set( texsz.x() * 1.0f,
			   (float)(divisions - i - 1) / (float)divisions );
		if (!hemisphere) {
			t._v[1] = 0.5f + (0.5f * t.y());
		}
		t._v[1] = texsz.y() * t.y();
		texcoords->push_back(t);
		// the bottom hemisphere
		if (!hemisphere) {
			if (i != (divisions - 1)) {
				t._v[1] = texsz.y() - t.y();
				texcoords->push_back(t);
			}
		}
	}

	// the bottom texcoords for hemispheres
	const int bottomTexOffset = texcoords->size();
	if (hemisphere) {
		const float astep = (float)((M_PI * 2.0) / (float) (divisions * 4));
		for (i = 0; i < (divisions * 4); i++) {
			float ang = astep * (float)i;
			t.set( texsz.x() * (0.5f + (0.5f * cosf(ang))),
				   texsz.y() * (0.5f + (0.5f * sinf(ang))) );
			texcoords->push_back(t);
		}
	}

	// make the mesh
	int faceCount = (divisions * divisions) * 8;
	osg::Geometry* geometry = new osg::Geometry();
	outside->addDrawable(geometry);

	// add the faces to the mesh
	int k = (divisions - 1);
	int ringOffset;
	if (!hemisphere) {
		ringOffset = 1 + (((k*k)+k)*2);
	} else {
		ringOffset = 0;
	}

	// mapping from vertices to texcoords must be 1 to 1 so separate arrays are used
	osg::Vec3Array* realVertices = new osg::Vec3Array();
	osg::Vec2Array* realTexcoords = new osg::Vec2Array();
	geometry->setVertexArray( realVertices );
	geometry->setTexCoordArray( 0, realTexcoords );

	for (q = 0; q < 4; q++) {
		for (i = 0; i < divisions; i++) {
			for (j = 0; j < (i + 1); j++) {
				int a, b, c, d, ta, tc;
				// a, b, c form the upwards pointing triangle
				// b, c, d form the downwards pointing triangle
				// ta and tc are the texcoords for a and c
				const bool lastStrip = ((q == 3) && (j == i));
				const bool lastCircle = (i == (divisions - 1));

				// setup 'a'
				if (i > 0) {
					if (lastStrip) {
						k = (i - 1);
						a = 1 + (((k*k)+k)*2);
					} else {
						k = (i - 1);
						a = 1 + (((k*k)+k)*2) + (q*(k+1)) + j;
					}
				} else {
					a = 0;
				}

				// setup 'b'
				b = 1 + (((i*i)+i)*2) + (q*(i+1)) + j;

				// setup 'c'
				if (lastStrip) {
					c = 1 + (((i*i)+i)*2);
				} else {
					c = b + 1;
				}

				// setup 'd' for the down-pointing triangle
				k = (i + 1);
				d = 1 + (((k*k)+k)*2) + (q*(k+1)) + (j + 1);


				// top hemisphere
				a = a * factor;
				if (!lastCircle) {
					b = b * factor;
					c = c * factor;
				} else {
					b = b + ringOffset;
					c = c + ringOffset;
				}
				if (i != (divisions - 2)) {
					d = d * factor;
				} else {
					d = d + ringOffset;
				}

				// deal with the last strip of texture coordinates
				if (!lastStrip) {
					ta = a;
					tc = c;
				} else {
					ta = texStripOffset + (i * factor);
					tc = texStripOffset + ((i + 1) * factor);
				}

				realVertices->push_back( (*vertices)[a] );
				realVertices->push_back( (*vertices)[b] );
				realVertices->push_back( (*vertices)[c] );
				realTexcoords->push_back( (*texcoords)[ta] );
				realTexcoords->push_back( (*texcoords)[b] );
				realTexcoords->push_back( (*texcoords)[tc] );
				if (!lastCircle) {
					realVertices->push_back( (*vertices)[b] );
					realVertices->push_back( (*vertices)[d] );
					realVertices->push_back( (*vertices)[c] );
					realTexcoords->push_back( (*texcoords)[b] );
					realTexcoords->push_back( (*texcoords)[d] );
					realTexcoords->push_back( (*texcoords)[tc] );
				}

				// bottom hemisphere
				if (!hemisphere) {
					a = a + 1;
					ta = ta + 1;
					if (!lastCircle) {
						b = b + 1;
						c = c + 1;
						tc = tc + 1;
					}
					if (i != (divisions - 2)) {
						d = d + 1;
					}
					realVertices->push_back( (*vertices)[a] );
					realVertices->push_back( (*vertices)[c] );
					realVertices->push_back( (*vertices)[b] );
					realTexcoords->push_back( (*texcoords)[ta] );
					realTexcoords->push_back( (*texcoords)[tc] );
					realTexcoords->push_back( (*texcoords)[b] );
					if (!lastCircle) {
						realVertices->push_back( (*vertices)[b] );
						realVertices->push_back( (*vertices)[c] );
						realVertices->push_back( (*vertices)[d] );
						realTexcoords->push_back( (*texcoords)[b] );
						realTexcoords->push_back( (*texcoords)[tc] );
						realTexcoords->push_back( (*texcoords)[d] );
					}
				}
			}
		}
	}

	osg::DrawElementsUInt* faces = new osg::DrawElementsUInt( osg::DrawElements::TRIANGLES, 0 );
	for (int i = 0; i < realVertices->size(); i++)
		faces->push_back( i );
	geometry->addPrimitiveSet( faces );

	// add the bottom disc
	if (hemisphere) {
		osg::Geometry* bottomGeom = new osg::Geometry();
		bottom->addDrawable( bottomGeom );

		osg::Vec3Array* realVertices = new osg::Vec3Array();
		osg::Vec2Array* realTexcoords = new osg::Vec2Array();
		bottomGeom->setVertexArray( realVertices );
		bottomGeom->setTexCoordArray( 0, realTexcoords );

		k = (divisions - 1);
		const int offset = 1 + (((k*k)+k)*2);
		for (i = 0; i < (divisions * 4); i++) {
			const int vv = (divisions * 4) - i - 1;
			realVertices->push_back( (*vertices)[vv + offset] );
			realTexcoords->push_back( (*texcoords)[i + bottomTexOffset] );
		}
		
		osg::DrawElementsUInt* faces = new osg::DrawElementsUInt( osg::DrawElements::QUADS, 0 );
		for (int i = 0; i < realVertices->size(); i++)
			faces->push_back( i );

		bottomGeom->addPrimitiveSet( faces );
	}


}

osg::Group* Primitives::buildUntexturedBox( const osg::Vec3* size ) {
	osg::Group* group = new osg::Group();
	// separate geometry nodes are needed so that each side
	// can have a separate material
	// array is in the order +x -x +y -y +z -z in bzflag coordinates
	osg::Geode* sideNodes[6];
	for ( int i = 0; i < 6; i++ )
		sideNodes[i] = new osg::Geode();

	// assign geometry nodes to group
	for ( int i = 0; i < 6; i++ )
		group->addChild( sideNodes[i] );

	// create geometry and assign it to the nodes
	osg::Geometry* sideGeometry[6];
	for ( int i = 0; i < 6; i++ ) {
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

	// specify the vertex indices, this is the same for all sides
	osg::DrawElementsUInt* side =
		new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	side->push_back( 0 );
	side->push_back( 1 );
	side->push_back( 2 );
	side->push_back( 3 );

	// add vertex indices for each side
	for ( int i = 0; i < 6; i++ ) {
		sideGeometry[i]->setVertexAttribBinding( 0, osg::Geometry::BIND_PER_VERTEX );
		sideGeometry[i]->addPrimitiveSet( side );
	}
	
	return group;
}

osg::Group* Primitives::buildBase( const std::string& sideTextureFile, const std::string& roofTextureFile, const osg::Vec3* size ) {
	osg::Group* group = buildUntexturedBox( size );

	// make UV coordinates
	RebuildBaseUV( group, size );

	// load side texture
	osg::Texture2D* sideTexture = loadTexture( sideTextureFile );

	// load roof texture
	osg::Texture2D* roofTexture = loadTexture( roofTextureFile );

	// associate textures with nodes
	for (int i = 0; i < 4; i++)
		setNodeTexture( group->getChild( i ), sideTexture );
	for (int i = 4; i < 6; i++)
		setNodeTexture( group->getChild( i ), roofTexture );

	return group;
}

osg::Texture2D* Primitives::loadTexture( const std::string& file ) {
	osg::Texture2D* texture = new osg::Texture2D();
	texture->setDataVariance( osg::Object::DYNAMIC );
	osg::Image* img = osgDB::readImageFile( file );
	// only set image if it was loaded properly
	if (img)
		texture->setImage(img);

	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	return texture;
}

void Primitives::setNodeTexture( osg::Node* node, osg::Texture2D* texture ) {
	osg::StateSet* state = new osg::StateSet();

	state->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
	node->setStateSet(state);
}