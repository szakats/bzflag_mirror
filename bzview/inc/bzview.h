#ifndef _BZVIEW_H_
#define _BZVIEW_H_

#ifdef _WIN32
	#include "win32.h"
	#include "resource.h"
	#pragma warning( disable : 4251 )
	#pragma warning( disable : 4267 )
#endif

#include <lf/Lightfeather.h>
using namespace lf;

class Application: public input::IKeyListener
{
public:
	Application();
	virtual ~Application();

	virtual bool init ( void );
	virtual bool frame ( void );

	int run ( void );

	void keyPressed(input::CKeyEvent& event);

protected:

	virtual void setup2D ( void );
	virtual void setupScene ( void );

	render::IRenderWindow		*rwin;
	CResourceManager			*rmgr;
	scene::CSceneManager		*smgr;
	scene::C3DCamera			*cam;
};


#endif