/* bzflag
 * Copyright (c) 1993 - 2002 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "ViewItemScene.h"
#include "SceneVisitorRender.h"
#include "SceneNodeMatrixTransform.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

//
// ViewItemScene
//

ViewItemScene::ViewItemScene() : xOffset(0.0f), yOffset(0.0f), zDepth(0.0f),
								zRotateFixed(0.0f), zRotateScaled(0.0f),
								yRotateFixed(0.0f), yRotateScaled(0.0f),
								fovxScale(1.0f), fovyScale(1.0f)
{
	// create nodes
	sceneProj  = new SceneNodeMatrixTransform;	// player projection
	sceneXForm = new SceneNodeMatrixTransform;	// player view

	// set transform types
	sceneProj->type.set(SceneNodeTransform::Projection);

	// assemble
	sceneProj->pushChild(sceneXForm);
	sceneXForm->unref();
}

ViewItemScene::~ViewItemScene()
{
	sceneProj->unref();
}

void					ViewItemScene::setOffset(float x, float y, float z)
{
	xOffset = x;
	yOffset = y;
	zDepth  = z;
}

void					ViewItemScene::setTurn(float fixed, float scaled)
{
	zRotateFixed  = fixed;
	zRotateScaled = scaled;
}

void					ViewItemScene::setTilt(float fixed, float scaled)
{
	yRotateFixed  = fixed;
	yRotateScaled = scaled;
}

void					ViewItemScene::setFOVScale(
							float _fovxScale, float _fovyScale)
{
	fovxScale = _fovxScale;
	fovyScale = _fovyScale;
}

SceneVisitorRender&		ViewItemScene::getRenderer()
{
	return render;
}

bool					ViewItemScene::onPreRender(
								float, float, float w, float h)
{
	// get scene
	SceneNode* scene = getScene();
	if (scene == NULL)
		return false;

	// get the current view
	ViewFrustum view;
	getView(view);

	// get fov
	float fovx = view.getFOVx();
	float fovy = view.getFOVy();

	// compute angle sines and cosines
	const float zAngle = -(zRotateFixed + fovx * zRotateScaled) * M_PI / 180.0f;
	const float yAngle = -(yRotateFixed + fovy * yRotateScaled) * M_PI / 180.0f;
	const float zc     = cosf(zAngle);
	const float zs     = sinf(zAngle);
	const float yc     = cosf(yAngle);
	const float ys     = sinf(yAngle);

	// adjust field of view
	fovx *= fovxScale;

	// adjust focus point based on our parameters
	const float* eye = view.getEye();
	const float* dir = view.getDirection();
	float focus[3];
/*
	focus[0] = eye[0] + yc * zc * dir[0] - yc * zs * dir[1] - ys * dir[2];
	focus[1] = eye[1] +      zs * dir[0] +      zc * dir[1]              ;
	focus[2] = eye[2] - ys * zc * dir[0] + ys * zs * dir[1] + yc * dir[2];
*/
	focus[0] = eye[0] + yc * zc * dir[0] - zs * dir[1] + ys * zc * dir[2];
	focus[1] = eye[1] + yc * zs * dir[0] + zc * dir[1] + ys * zs * dir[2];
	focus[2] = eye[2] - ys *      dir[0]               + yc *      dir[2];

	// adjust frustum
	view.setProjection(fovx, fovyScale * w / h, view.getNear(), view.getFar());
	if (zDepth > 0.0f)
		view.setOffset(xOffset, zDepth);
	view.setView(eye, focus);

	// set the projection and viewing transform
	sceneProj->matrix.set(view.getProjectionMatrix(), 16);
	sceneXForm->matrix.set(view.getViewMatrix(), 16);

	// push scene under our transformations
	sceneXForm->pushChild(scene);
	scene->unref();

	return true;
}

void					ViewItemScene::onPostRender(
								float, float, float w, float h)
{
	// draw
	render.setArea(w * h);
	render.traverse(sceneProj);
	sceneXForm->popChild();
}


//
// parsing function objects
//

class ViewSetRotation_t : public unary_function<std::string, ViewSize&> {
public:
	ViewSetRotation_t(ViewSize& size_) : size(size_) { }
	ViewSize&			operator()(const std::string& arg) const
	{
		float num;
		char type;
		switch (sscanf(arg.c_str(), "%f%c", &num, &type)) {
			case 1:
				size.pixel    = num;
				size.fraction = 0.0f;
				return size;

			case 2:
				if (type == '%') {
					size.pixel    = 0.0f;
					size.fraction = 0.01f * num;
					return size;
				}
				break;
		}
		throw XMLNode::AttributeException("invalid rotation");
	}

private:
	ViewSize&			size;
};

inline ViewSetRotation_t
viewSetRotation(ViewSize& size)
{
	return ViewSetRotation_t(size);
}


//
// ViewItemSceneReader
//

ViewItemSceneReader::ViewItemSceneReader() : item(NULL)
{
	// do nothing
}

ViewItemSceneReader::~ViewItemSceneReader()
{
	if (item != NULL)
		item->unref();
}

View*					ViewItemSceneReader::open(XMLTree::iterator xml)
{
	// parse
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float fov = 1.0f, fovy = 1.0f;
	ViewSize yRotate, zRotate;
	xml->getAttribute("x", xmlStrToFloat(xmlSetVar(x)));
	xml->getAttribute("y", xmlStrToFloat(xmlSetVar(y)));
	xml->getAttribute("z", xmlStrToFloat(xmlSetVar(z)));
	xml->getAttribute("fov", xmlStrToFloat(xmlSetVar(fov)));
	xml->getAttribute("fovy", xmlStrToFloat(xmlSetVar(fovy)));
	xml->getAttribute("theta", viewSetRotation(zRotate));
	xml->getAttribute("phi", viewSetRotation(yRotate));
	if (z < 0.0f)
		throw XMLIOException(xml->getAttributePosition("z"),
							"invalid focal plane distance");

	// create item
	assert(item == NULL);
	item = createItem();
	item->setOffset(x, y, z);
	item->setTurn(zRotate.pixel, zRotate.fraction);
	item->setTilt(yRotate.pixel, yRotate.fraction);
	item->setFOVScale(fov, fovy);

	return item;
}
