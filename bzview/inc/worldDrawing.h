#ifndef _WORLD_DRAWING_H_
#define _WORLD_DRAWING_H_

#include "world.h"

class ClassicBoxDrawInfo : public BaseDrawInfo
{
public:
	unsigned int	glList[2];
	int				roofTexture;
	int				wallTexture;

	ClassicBoxDrawInfo()
	{
		wallTexture = -1;
		roofTexture = -1;
		glList[0] = glList[1] = 0xFFFFFFFF;
	}

	void draw ( void );
};

class ClassicPyramidDrawInfo : public BaseDrawInfo
{
public:
	unsigned int	glList;
	int				pryTexture;

	ClassicPyramidDrawInfo()
	{
		pryTexture = -1;
		glList = 0xFFFFFFFF;
	}

	void draw ( void );
};


#endif //_WORLD_DRAWING_H_
