/* bzflag
 * Copyright (c) 1993 - 2009 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* LaserSceneNode:
 *	Encapsulates information for rendering a laser beam.
 */

#ifndef	BZF_LASER_SCENE_NODE_H
#define	BZF_LASER_SCENE_NODE_H

#include "common.h"
#include "SceneNode.h"

class LaserSceneNode : public SceneNode {
  public:
    LaserSceneNode(const fvec3& pos, const fvec3& forward);
    ~LaserSceneNode();

    void		setTexture(const int);

    bool		cull(const ViewFrustum&) const;

    void		notifyStyleChange();
    void		addRenderNodes(SceneRenderer&);

    void		setColor ( float r, float g, float b );
    void		setCenterColor ( float r, float g, float b );
    void		setFirst ( void ) {first = true;}
  protected:
    class LaserRenderNode : public RenderNode {
      public:
        LaserRenderNode(const LaserSceneNode*);
        ~LaserRenderNode();
	void		render();
	const fvec3&	getPosition() const { return sceneNode->getCenter(); }
      private:
	void renderFlatLaser();
	void renderGeoLaser();
	const LaserSceneNode* sceneNode;
	static float	geom[6][2];
    };
    fvec4 color;
    fvec4 centerColor;
    bool first;
    friend class LaserRenderNode;

  private:
    float		azimuth, elevation;
    float		length;
    bool		texturing;
    OpenGLGState	gstate;
    LaserRenderNode	renderNode;

};

#endif // BZF_LASER_SCENE_NODE_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
