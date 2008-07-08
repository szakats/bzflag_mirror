// bzview.cpp : Defines the entry point for the application.
//

#include "bzview.h"

#ifdef _WIN32
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int       nCmdShow)
{
#else
int main( int argc, char*argv[] )
{
#endif
	Application app;
	return app.run();
}	

Application::Application() : rwin(NULL)
{
	lf::initLF();
}

Application::~Application()
{
	lf::deinitLF();
}

bool Application::init ( void )
{
	rwin = CLFRender::getInstance().createRenderWindow(core::vector2di(0,0),core::vector2d<s32>(1024,768),32, 32,render::EWCF_AUTOCLOSE, render::EAAF_NONE);

	if(!rwin)
		return false;

	rwin->setWindowCaption(L"BZView");
	rwin->setVisible(true);
	rwin->addKeyListener(this);

	rmgr = CResourceManager::getInstancePtr();

	core::stringc mediaDir = CLFOS::getInstance().getFileSystem()->getApplicationDirectory();
	mediaDir += "/data";
	rmgr->getFileSystem()->addSearchPath(mediaDir.c_str());

	smgr = rwin->getRenderLayer3D()->getSceneManager();
	CLFRender::getInstance().setAutoSleep(0);

	setupScene();
	setup2D();

	return true;
}

bool Application::frame ( void )
{
	return CLFRender::getInstance().update();
}

int Application::run ( void )
{
	if (!init())
		return 1;

	while(frame());

	return 0;
}

void Application::setup2D( void )
{
	render::CRenderLayer2D *pane = new render::CRenderLayer2D(core::rect<s32>(0,0,rwin->getInnerSize().X,rwin->getInnerSize().Y),core::CColorI(0,0,0,255),false,false,false);

	rwin->add(pane);
	pane->drop();

	//res::CTexture *title = rmgr->loadTexture("title.png","title");
	//pane->drawImage( title, core::vector2d<s32>( rwin->getInnerSize().X - title->getSize().X , rwin->getInnerSize().Y -title->getSize().Y ), title->getSize(),render::ETT_TEST);	


	res::CTexture *icon = rmgr->loadTexture("bzflag-256x256.png","icon");
	pane->drawImage( icon, core::vector2d<s32>( rwin->getInnerSize().X - icon->getSize().X , rwin->getInnerSize().Y -icon->getSize().Y ), icon->getSize(),render::ETT_TEST);	

}

void Application::setupScene()
{
	cam = new scene::C3DCamera(rwin, core::PI / 3.0f, 1.0f,1000.0f, true); 
	cam->setBackgroundColor(core::CColorI(128,128,255,0));

//	cam->setTarget(core::vector3df(0,0,0),core::vector3df(0,0,1));

	cam->setPosition(core::vector3df(0.0f,7.0f,-20.0f));
	rwin->getRenderLayer3D()->add(cam);
	cam->drop();

	res::CTexture *boxwall = rmgr->loadTexture("boxwall.png","boxwall");

	res::CCubeModel *cube = new res::CCubeModel("cube1",5.0f,5.0f,5.0f,0,0,0,true);
	cube->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);
	cube->getMaterial(0)->setTexture(0,boxwall);

	scene::CModelSceneNode *cubeNode = new scene::CModelSceneNode(cube);
	smgr->addSceneNode(cubeNode);
	cubeNode->setPosition(core::vector3df(0.0f,2.5f,0.0f));
	cubeNode->rotateYDegrees(45.0f);
	//cubeNode->rotateXDegrees(-45.0f);
	cubeNode->drop();


	cube = new res::CCubeModel("cube1",1.0f,1.0f,1.0f,0,0,0,true);
	cube->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);
	cube->getMaterial(0)->setTexture(0,boxwall);

	scene::CModelSceneNode *cubeNode2 = new scene::CModelSceneNode(cube);
	smgr->addSceneNode(cubeNode2);
	cubeNode2->setPosition(core::vector3df(0.0f,5.5f,0.0f));
	//cubeNode->rotateYDegrees(45.0f);
	//cubeNode->rotateXDegrees(-45.0f);
	cubeNode2->drop();


	res::CTexture *groundTex = rmgr->loadTexture("std_ground.png","std_ground");

	res::CPlaneModel *plane = new res::CPlaneModel("ground",800,800,100,100,200,200);
	plane->getMaterial(0)->setTexture(0,groundTex);
	plane->getMaterial(0)->setRenderFeature(render::ERPF_DIFFUSEMAP);

	scene::CModelSceneNode *planeNode = new scene::CModelSceneNode(plane);
	smgr->addSceneNode(planeNode);
	planeNode->setPosition(core::vector3df(0.0f,0.0f,0.0f));
	//cubeNode->rotateYDegrees(45.0f);
	//cubeNode->rotateXDegrees(-90.0f);
	planeNode->drop();

	// lighting

	render::CRenderStateLighting *rstLgt = new render::CRenderStateLighting(true);
	cam->replace(rstLgt);
	rstLgt->drop();

	render::CRenderStateSpecularLighting *rstSpl = new render::CRenderStateSpecularLighting(true,true);
	cam->replace(rstSpl);
	rstSpl->drop();

	render::CRenderStateAmbientLight *rstAmb = new render::CRenderStateAmbientLight(core::CColorI(50,50,50,255));
	cam->replace(rstAmb);
	rstAmb->drop();


	light = new scene::CLight( scene::ELT_POINT, 
		core::CColorF(0.7f,0.7f,0.7f,1.0f), 
		core::CColorF(1.0f,1.0f,1.0f,1.0f),
		core::CColorF(0.2f,0.2f,0.2f,1.0f), 
		100.0f,
		0.5f,
		core::PI/3,
		core::PI/2,
		0.0f,
		0.0f,
		1.0f / 500.0f,
		0.0f 
		);

	light->setPosition(core::vector3df(15.0f,15.0f,0.0f));
	smgr->add(light);
	light->drop();

	scene::CSceneStateLight *cstLgt = new scene::CSceneStateLight(light,true);
	smgr->getRootSceneNode()->addSceneState(cstLgt);
	cstLgt->drop();

	light->makeShadowProjector( 1024,	
								1024,	
								1.0f,	
								1000.0f,
								0.0f,	
								0.0f,	
								0.0f,	
								0.0f	
								);

	// now that the light projects shadows, it must also be set which nodes in the scene should
	// cast a shadow from this light. In our basic test-scene we will use the diamond-model/node
	// as the sole shadow-caster
	scene::CSceneStateShadowCaster *sssc = new scene::CSceneStateShadowCaster(light);
	cubeNode->addSceneState(sssc);
	cubeNode2->addSceneState(sssc);
	sssc->drop();

	scene::CSceneStateShadowReceiver *sssr = new scene::CSceneStateShadowReceiver(light,true);

	// now that scenestate is added to the plane node so that it should receive shadow from the light
	planeNode->addSceneState(sssr);
	cubeNode->addSceneState(sssr);
	cubeNode2->addSceneState(sssr);
	sssr->drop();
}

void Application::keyPressed(input::CKeyEvent& event)
{
	switch(event.getKey())
	{
	case input::KEY_ESCAPE:
		rwin->closeWindow();
		break;
	}
}


