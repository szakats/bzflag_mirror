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

	res::CTexture *ltexture = rmgr->loadTexture("title.png","title");

	pane->drawImage( ltexture, core::vector2d<s32>( rwin->getInnerSize().X - ltexture->getSize().X , rwin->getInnerSize().Y -ltexture->getSize().Y ), ltexture->getSize(),render::ETT_TEST);	
}


void Application::setupScene()
{
	cam = new scene::C3DCamera(rwin, core::PI / 3.0f, 1.0f,1000.0f, true); 
	cam->setBackgroundColor(core::CColorI(0,0,255,0));

	cam->setPosition(core::vector3df(0.0f,0.0f,-20.0f));
	rwin->getRenderLayer3D()->add(cam);
	cam->drop();

	res::CCubeModel *cube = new res::CCubeModel("cube1",5.0f,5.0f,5.0f);
	cube->getMaterial(0)->setRenderFeature(render::ERPF_COLOR_ONLY);

	scene::CModelSceneNode *cubeNode = new scene::CModelSceneNode(cube);
	smgr->addSceneNode(cubeNode);
	cubeNode->setPosition(core::vector3df(0.0f,0.0f,0.0f));
	cubeNode->rotateYDegrees(45.0f);
	cubeNode->rotateXDegrees(-45.0f);

	cubeNode->drop();
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


