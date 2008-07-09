#ifndef _BZVIEW_H_
#define _BZVIEW_H_

#ifdef _WIN32
	#include "win32.h"
	#include "resource.h"
	#pragma warning( disable : 4251 )
	#pragma warning( disable : 4267 )
#endif

#include <string>
#include <vector>

#include <lf/Lightfeather.h>
using namespace lf;

#include "world.h"

class Application: public input::IKeyListener
{
public:
	Application();
	virtual ~Application();

	virtual bool init ( const std::vector<std::string> &params );
	virtual bool frame ( void );

	int run ( const std::vector<std::string> &params );

	void keyPressed(input::CKeyEvent& event);

protected:
	virtual void setup2D ( void );
	virtual void setupScene ( void );
	virtual void setupWorldScene ( void );

	// setup
	void addObject ( MapItem* object );

	void processBox ( MapItem* object );
	void processInstance ( MapItem* object );

	render::IRenderWindow		*rwin;
	CResourceManager			*rmgr;
	scene::CSceneManager		*smgr;
	scene::C3DCamera			*cam;
	scene::CLight				*light; 

	World						world;

};


#endif