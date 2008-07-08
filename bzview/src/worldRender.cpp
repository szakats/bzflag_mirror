
#include "worldRender.h"
#include "utils.h"
#include "worldDrawing.h"

float groundRepeat = 1.0f/20.0f;

void processTransformStack ( AffineTransformationStack &transforms )
{
// 	GLfloat t[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
// 					{0.0f, 1.0f, 0.0f, 0.0f},
// 					{0.0f, 0.0f, 1.0f, 0.0f},
// 					{0.0f, 0.0f, 0.0f, 1.0f}};
// 
// 	int numTrans = (int)transforms.size();
// 	for ( int i = numTrans-1; i >= 0; i-- )
// 	{
// 		switch(transforms[i].type)
// 		{
// 			case eShear:
// 					t[0][2] = transforms[i].values[0];
// 					t[1][2] = transforms[i].values[1];
// 					t[2][0] = transforms[i].values[3];
// 					glMultMatrixf((GLfloat*)t);
// 					break;	
// 
// 			case eSpin:
// 				glRotatef(transforms[i].values[0],transforms[i].values[1],transforms[i].values[2],transforms[i].values[3]);
// 				break;
// 
// 			case eScale:
// 				glScalef(transforms[i].values[0],transforms[i].values[1],transforms[i].values[2]);
// 				break;
// 
// 			case eShift:
// 			default:
// 				glTranslatef(transforms[i].values[0],transforms[i].values[1],transforms[i].values[2]);
// 				break;
// 		}
// 	}
}

WorldRender::WorldRender(World &_world) : world(_world)
{
}

void WorldRender::init ( void )
{
}

void WorldRender::draw ( void )
{
	// draw some boxes
// 	glEnable(GL_LIGHTING);

	// draw some big ass ground
	tvItList worlds = world.findItemsOfClass(std::string("world"));

	float worldSize[2] = {400,400};

	if ( worlds.size() )
	{
		WorldItem* worldItem = (WorldItem*)world.getItem(worlds[0]);

		if ( worldItem->size[0] > 0)
			worldSize[0] = worldItem->size[0];

		if ( worldItem->size[1] > 0)
			worldSize[1] = worldItem->size[1];
		else
			worldSize[1] = worldSize[0];
	}

	drawGrounds(worldSize);

	tvItList boxes = world.findItemsOfClass(std::string("box"));

	for ( unsigned int i = 0; i < boxes.size(); i++)
		drawClassicBox(world.getItem(boxes[i]));


	tvItList pyramids = world.findItemsOfClass(std::string("pyramid"));

	for ( unsigned int i = 0; i < pyramids.size(); i++)
		drawClassicPyramid(world.getItem(pyramids[i]));


	tvItList instances = world.findItemsOfClass(std::string("instance"));

	for ( unsigned int i = 0; i < instances.size(); i++)
		drawInstance(world.getItem(instances[i]));

	drawUnderGrounds(worldSize);
// 	glColor4f(1,1,1,1);
}

void WorldRender::drawGrounds ( float size[2] )
{
	// green and solid from the top side of the dirt
// 	glColor4f(0.125f,0.5f,0.125f,1);
// 
// 	TextureSystem &ts = TextureSystem::Instance();
// 
// 	float textureScale = groundRepeat;
// 
// 	glTexturesOn();
// 
// 	ts.bind(stdGroundTexture);
// 	glBegin(GL_QUADS);
// 		glNormal3f(0,0,1);
// 		glTexCoord2f(-size[0]*textureScale,size[1]*textureScale);
// 		glVertex3f(-size[0],size[1],0);
// 
// 		glTexCoord2f(-size[0]*textureScale,-size[1]*textureScale);
// 		glVertex3f(-size[0],-size[1],0);
// 
// 		glTexCoord2f(size[0]*textureScale,-size[1]*textureScale);
// 		glVertex3f(size[0],-size[1],0);
// 
// 		glTexCoord2f(size[0]*textureScale,size[1]*textureScale);
// 		glVertex3f(size[0],size[1],0);
// 	glEnd();
// 
// 	ts.bind(stdWallTexture);
// 
// 	glBegin(GL_QUADS);
// 		float wallHeight = 10;
// 		// outer walls
// 		glColor4f(0.5f,0.27f,0.0f,1);
// 
// 		// X+
// 		glNormal3f(-1,0,0);
// 		glVertex3f(size[0],size[1],0);
// 		glVertex3f(size[0],-size[1],0);
// 		glVertex3f(size[0],-size[1],wallHeight);
// 		glVertex3f(size[0],size[1],wallHeight);
// 
// 		// X-
// 		glNormal3f(1,0,0);
// 		glVertex3f(-size[0],size[1],wallHeight);
// 		glVertex3f(-size[0],-size[1],wallHeight);
// 		glVertex3f(-size[0],-size[1],0);	
// 		glVertex3f(-size[0],size[1],0);
// 
// 		// Y+ side
// 		glNormal3f(0,-1,0);
// 		glVertex3f(-size[0],size[1],0);
// 		glVertex3f(size[0],size[1],0);
// 		glVertex3f(size[0],size[1],wallHeight);
// 		glVertex3f(-size[0],size[1],wallHeight);
// 
// 		// Y- side
// 		glNormal3f(0,1,0);
// 		glVertex3f(-size[0],-size[1],wallHeight);
// 		glVertex3f(size[0],-size[1],wallHeight);
// 		glVertex3f(size[0],-size[1],0);	
// 		glVertex3f(-size[0],-size[1],0);
// 
// 	glEnd();
// 	glTexturesOff();
}

void WorldRender::drawUnderGrounds ( float size[2] )
{
	// green and solid from the top
// 	glColor4f(0.0f,0.25f,0.0f,0.75f);
// 
// 	TextureSystem &ts = TextureSystem::Instance();
// 
// 	float textureScale = groundRepeat;
// 
// 	glTexturesOn();
// 
// 	ts.bind(stdGroundTexture);
// 
// 	glBegin(GL_QUADS);
// 		glNormal3f(0,0,-1);
// 
// 		glTexCoord2f(size[0]*textureScale,size[1]*textureScale);
// 		glVertex3f(size[0],size[1],0);
// 
// 		glTexCoord2f(size[0]*textureScale,-size[1]*textureScale);
// 		glVertex3f(size[0],-size[1],0);
// 
// 		glTexCoord2f(-size[0]*textureScale,-size[1]*textureScale);
// 		glVertex3f(-size[0],-size[1],0);	
// 		
// 		glTexCoord2f(-size[0]*textureScale,size[1]*textureScale);
// 		glVertex3f(-size[0],size[1],0);
// 	glEnd();
// 
// 	glTexturesOff();
// 
// 	glBegin(GL_QUADS);
// 		float wallHeight = 10;
// 		// outer walls
// 		glColor4f(0.25f,0.13f,0.0f,0.75f);
// 
// 		// X+
// 		glNormal3f(1,0,0);
// 		glVertex3f(size[0],size[1],wallHeight);
// 		glVertex3f(size[0],-size[1],wallHeight);
// 		glVertex3f(size[0],-size[1],0);	
// 		glVertex3f(size[0],size[1],0);
// 
// 		// X-
// 		glNormal3f(-1,0,0);
// 		glVertex3f(-size[0],size[1],0);
// 		glVertex3f(-size[0],-size[1],0);
// 		glVertex3f(-size[0],-size[1],wallHeight);
// 		glVertex3f(-size[0],size[1],wallHeight);
// 
// 		// Y+ side
// 		glNormal3f(0,1,0);
// 		glVertex3f(-size[0],size[1],wallHeight);
// 		glVertex3f(size[0],size[1],wallHeight);
// 		glVertex3f(size[0],size[1],0);	
// 		glVertex3f(-size[0],size[1],0);
// 
// 		// Y- side
// 		glNormal3f(0,-1,0);
// 		glVertex3f(-size[0],-size[1],0);
// 		glVertex3f(size[0],-size[1],0);
// 		glVertex3f(size[0],-size[1],wallHeight);
// 		glVertex3f(-size[0],-size[1],wallHeight);
// 
// 	glEnd();
 }
// 
 void WorldRender::drawObject ( MapItem* object )
 {
// 	if (!object)
// 		return;
// 
// 	std::string className = TextUtils::tolower(object->getClass());
// 
// 	if (className == "box")
// 		drawClassicBox(object);
// 	else if (className == "pyramid")
// 		drawClassicPyramid(object);
// 	else if (className == "instance")
// 		drawInstance(object);
}

void  WorldRender::drawClassicBox ( MapItem* object )
{
// 	ClassicBoxItem	*box = (ClassicBoxItem*)object;
// 	if (!box)
// 		return;
// 
// 	ClassicBoxDrawInfo *drawInfo = NULL;
// 	if (!box->drawInfo)
// 	{
// 		drawInfo = new ClassicBoxDrawInfo;
// 		drawInfo->load(box,editor);
// 		box->drawInfo = (BaseDrawInfo*)drawInfo;
// 	}
// 	drawInfo = (ClassicBoxDrawInfo*)box->drawInfo;
// 
// 	glPushMatrix();
// 	glRotatef(box->rot,0,0,1);
// 	glTranslatef(box->pos[0],box->pos[1],box->pos[2]);
// 	processTransformStack(box->transformStack);
// 
// 	drawInfo->draw();
// 
// 	glPopMatrix();
}

void WorldRender::drawClassicPyramid (  MapItem* object )
{
// 	ClassicPyramidItem	*pyr = (ClassicPyramidItem*)object;
// 	if (!pyr)
// 		return;
// 
// 	ClassicPyramidDrawInfo *drawInfo = NULL;
// 	if (!pyr->drawInfo)
// 	{
// 		drawInfo = new ClassicPyramidDrawInfo;
// 		drawInfo->load(pyr,editor);
// 		pyr->drawInfo = (BaseDrawInfo*)drawInfo;
// 	}
// 	drawInfo = (ClassicPyramidDrawInfo*)pyr->drawInfo;
// 
// 	glPushMatrix();
// 	glRotatef(pyr->rot,0,0,1);
// 	glTranslatef(pyr->pos[0],pyr->pos[1],pyr->pos[2]);
// 	processTransformStack(pyr->transformStack);
// 
// 	drawInfo->draw();
// 
// 	glPopMatrix();
}


void WorldRender::drawInstance (  MapItem* object )
{
	InstanceItem *instance = (InstanceItem*)object;
	if (!instance)
		return;

	GroupItem *group = (GroupItem*)world.getItem(world.findItemOfClassByName(std::string("group"),instance->define));
	if ( !group )
		return;

// 	glPushMatrix();
	processTransformStack(instance->transformStack);

	for ( int i = 0; i < group->getSubItemCount(); i++ )
		drawObject(group->getSubItem(i));

// 	glPopMatrix();
}



	


