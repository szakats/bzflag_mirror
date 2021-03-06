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

/* EighthDBoxSceneNode:
 *	Encapsulates information for rendering the eighth dimension
 *	of a pyramid building.
 */

#ifndef	BZF_EIGHTHD_PYR_SCENE_NODE_H
#define	BZF_EIGHTHD_PYR_SCENE_NODE_H

#include "common.h"
#include "EighthDimSceneNode.h"

class EighthDPyrSceneNode : public EighthDimSceneNode {
  public:
    EighthDPyrSceneNode(const fvec3& pos,
                        const fvec3& size, float rotation);
    ~EighthDPyrSceneNode();

    void		notifyStyleChange();
    void		addRenderNodes(SceneRenderer&);

  protected:
    class EighthDPyrRenderNode : public RenderNode {
      public:
			EighthDPyrRenderNode(const EighthDPyrSceneNode*,
				const fvec3& pos,
				const fvec3& size, float rotation);
			~EighthDPyrRenderNode();
	void		render();
	const fvec3&	getPosition() const { return sceneNode->getCenter(); }
      private:
	const EighthDPyrSceneNode* sceneNode;
	fvec3		corner[5];
    };

  private:
    OpenGLGState	 gstate;
    EighthDPyrRenderNode renderNode;
};

#endif // BZF_EIGHTHD_PYR_SCENE_NODE_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
