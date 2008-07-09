
#include "worldRender.h"
#include "utils.h"
#include "worldDrawing.h"

float groundRepeat = 1.0f/20.0f;

void processTransformStack ( AffineTransformationStack &transforms,scene::CModelSceneNode *node )
{
// 	GLfloat t[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
// 					{0.0f, 1.0f, 0.0f, 0.0f},
// 					{0.0f, 0.0f, 1.0f, 0.0f},
// 					{0.0f, 0.0f, 0.0f, 1.0f}};
// node
	for ( size_t i = transforms.size()-1; i >= 0; i-- )
	{
		switch(transforms[i].type)
		{
			case eShear:
// 				//node->??
// 				t[0][2] = transforms[i].values[0];
// 				t[1][2] = transforms[i].values[1];
// 				t[2][0] = transforms[i].values[3];
// 				glMultMatrixf((GLfloat*)t);
				break;	

			case eSpin:
				node->setRotation(core::quaternion(vector3df(transforms[i].values[1],transforms[i].values[2],transforms[i].values[3]),transforms[i].values[0]));
				break;

			case eScale:
				glScalef(transforms[i].values[0],transforms[i].values[1],transforms[i].values[2]);
				break;

			case eShift:
			default:
				node->tr
				glTranslatef(transforms[i].values[0],transforms[i].values[1],transforms[i].values[2]);
				break;
		}
	}
}

WorldRender::WorldRender(World &_world) : world(_world)
{
}

void WorldRender::build ( CResourceManager *_rmgr, scene::CSceneManager *_smgr )
{
	rmgr = _rmgr;
	smgr = _smgr;

	transformStack.clear();

	res::CTexture *roof = rmgr->loadTexture("mesh.png","roof");


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

	buildGrounds(worldSize);

	tvItList objects = world.getItemList();

	for ( size_t i = 0; i < objects.size(); i++)
		buildObject(world.getItem(objects[i]));
}

void WorldRender::buildGrounds ( float size[2] )
{
	// add the ground
	res::CTexture *groundTex = rmgr->loadTexture("std_ground.png","std_ground");
	res::CTexture *wall = rmgr->loadTexture("wall.png","wall");

	res::CPlaneModel *plane = new res::CPlaneModel("ground",size[0]*2,size[1]*2,size[0]/4,size[1]/4,size[0]/2,size[1]/2);
	plane->getMaterial(0)->setTexture(0,groundTex);
	plane->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);

	scene::CModelSceneNode *planeNode = new scene::CModelSceneNode(plane);
	smgr->addSceneNode(planeNode);
	planeNode->setPosition(core::vector3df(0.0f,0.0f,0.0f));
	//cubeNode->rotateYDegrees(45.0f);
	//cubeNode->rotateXDegrees(-90.0f);
	planeNode->drop();

	// X+ wall
	res::CCubeModel *wallmesh = new res::CCubeModel("X+wall",0.5f,10.0f,size[1]*2,0,0,0,true);
	wallmesh->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);
	wallmesh->getMaterial(0)->setTexture(0,wall);

	scene::CModelSceneNode *wallNode = new scene::CModelSceneNode(wallmesh);
	smgr->addSceneNode(wallNode);
	wallNode->setPosition(core::vector3df(size[0],5.0f,0.0f));
	wallNode->drop();

	// X- wall
	wallmesh = new res::CCubeModel("X-wall",0.5f,10.0f,size[1]*2,0,0,0,true);
	wallmesh->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);
	wallmesh->getMaterial(0)->setTexture(0,wall);

	wallNode = new scene::CModelSceneNode(wallmesh);
	smgr->addSceneNode(wallNode);
	wallNode->setPosition(core::vector3df(-size[0],5.0f,0.0f));
	wallNode->drop();

	// Z+ wall
	wallmesh = new res::CCubeModel("Z+wall",size[0]*2,10.0f,0.5f,0,0,0,true);
	wallmesh->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);
	wallmesh->getMaterial(0)->setTexture(0,wall);

	wallNode = new scene::CModelSceneNode(wallmesh);
	smgr->addSceneNode(wallNode);
	wallNode->setPosition(core::vector3df(0.0f,5.0f,size[1]));
	wallNode->drop();

	// Z+ wall
	wallmesh = new res::CCubeModel("Z+wall",size[0]*2,10.0f,0.5f,0,0,0,true);
	wallmesh->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);
	wallmesh->getMaterial(0)->setTexture(0,wall);

	wallNode = new scene::CModelSceneNode(wallmesh);
	smgr->addSceneNode(wallNode);
	wallNode->setPosition(core::vector3df(0.0f,5.0f,-size[1]));
	wallNode->drop();

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

//void WorldRender::drawUnderGrounds ( float size[2] )
//{
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
 void WorldRender::buildObject ( MapItem* object )
 {
 	if (!object)
 		return;
 
	std::string className = tolower(object->getClass());
 
	transformStack.push_back(object->transformStack);
 	if (className == "box")
 		buildClassicBox(object);
 	else if (className == "pyramid")
 		buildClassicPyramid(object);
 	else if (className == "instance")
 		buildInstance(object);

	transformStack.pop_back();
 }


void WorldRender:processStack ( scene::CModelSceneNode *node )
{
	std::list<AffineTransformationStack>::iterator itr = transformStack.begin();
	while (itr != transformStack.end())
	{
		processTransformStack(*itr,node);
		itr++;
	}
}

void  WorldRender::buildClassicBox ( MapItem* object )
{
	if (!object)
		return;

	ClassicBoxItem* box = (ClassicBoxItem*)object;

	std::string name;
	if (box->name.size())
		name = box->name;
	else
		name = format("box:%d",object->getID());

	res::CTexture *boxwall = rmgr->loadTexture("boxwall.png","boxwall");

	res::CCubeModel *boxMesh = new res::CCubeModel(name.c_str(),box->scale[0],box->scale[2],box->scale[1],0,0,0,true);
	boxMesh->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);
	boxMesh->getMaterial(0)->setTexture(0,boxwall);

	scene::CModelSceneNode *boxNode = new scene::CModelSceneNode(boxMesh);
	smgr->addSceneNode(boxNode);

	// process the transform stack
	wallNode->setPosition(core::vector3df(size[0],5.0f,0.0f));
	wallNode->drop();

}

void WorldRender::buildClassicPyramid (  MapItem* object )
{

}

void WorldRender::buildInstance (  MapItem* object )
{
	InstanceItem *instance = (InstanceItem*)object;
	if (!instance)
		return;

	GroupItem *group = (GroupItem*)world.getItem(world.findItemOfClassByName(std::string("group"),instance->define));
	if ( !group )
		return;

	for ( int i = 0; i < group->getSubItemCount(); i++ )
		buildObject(group->getSubItem(i));
}



	


