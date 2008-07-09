#ifndef _WORLD_RENDER_H_
#define _WORLD_RENDER_H_

#include "bzview.h"
#include <list>

class WorldRender
{
public:
	WorldRender (World &_world);

	void build ( CResourceManager *_rmgr, scene::CSceneManager *_smgr );
 );
protected:
	World	&world;

	void buildGrounds ( float size[2] );
//	void drawUnderGrounds ( float size[2] );

	void buildObject ( MapItem* object );

	void buildInstance (  MapItem* object );
	void buildClassicBox (  MapItem* object );
	void buildClassicPyramid (  MapItem* object );

	std::list<AffineTransformationStack> transformStack;

	CResourceManager *rmgr;
	scene::CSceneManager *smgr;

private:
	void processStack ( scene::CModelSceneNode *node );
};

#endif //_WORLD_RENDER_H_
