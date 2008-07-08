#ifndef _WORLD_RENDER_H_
#define _WORLD_RENDER_H_

#include "world.h"

class WorldRender
{
public:
	WorldRender (World &_world);

	void init ( void );

	void draw ( void );
protected:
	World	&world;

	void drawGrounds ( float size[2] );
	void drawUnderGrounds ( float size[2] );

	void drawObject ( MapItem* object );

	void drawInstance (  MapItem* object );
	void drawClassicBox (  MapItem* object );
	void drawClassicPyramid (  MapItem* object );

	// texture stash
	int stdWallTexture;
	int	stdGroundTexture;
};

#endif //_WORLD_RENDER_H_
