
#ifdef POCKETENGINE_EXPORTS
#define POCKETENGINE_API __declspec(dllexport)
#else
#define POCKETENGINE_API __declspec(dllimport)
#endif

#ifndef _POCKET_ENGINE_H_
#define _POCKET_ENGINE_H_

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "3dtypes.h"

#define		_NearClip	1.0
#define		_FarClip	10000.0

typedef enum
{
	eMatrix,
	ePartView,
	eQuakeType,
	eQuestType,
	eManual
}teViewType;

enum
{
	eLightPos,
	eLightSpec,
	eLightDif,
	eLightAmb,
	eLgihtOther
};

typedef void (*tfDrawBackCB)(void*);

#define MSGLMODE_PAN		1
#define MSGLMODE_ROT		2
#define MSGLMODE_ZOOM		3
#define MSGLMODE_SELECT		4

class POCKETENGINE_API CPocketEngine
{
public:
	CPocketEngine ( void );
	~CPocketEngine ( void );

	// window creation
	bool Create ( int x, int y, HINSTANCE hInstance, HWND hParent = NULL );
	void Kill ( void );

	void UseViewCtls ( bool bUse) {m_bUseViewKeys = bUse;}
	void SizeWindow( int x, int y );

	void ShowGrid ( bool bShow ) {m_bDrawGrid = bShow;}
	bool GridShown () {return m_bDrawGrid;}

	// data retrival
	HWND GetWindow ( void ) {return m_hGLWindow;}
	void SetWindow ( HWND wnd) {m_hGLWindow = wnd;}
	
	void DrawFlatXYQuad ( float x, float y, float z = 0, float h = 10.0f, float v = 10.0f,float r = 1, float g = 1, float b = 1, float a = 1 );
	void DrawTexturedXYQuad ( float x, float y, float z = 0, float h = 10.0f, float v = 10.0f,float r = 1, float g = 1, float b = 1, float a = 1 );
	void DrawTexturedPlanerQuad ( float x, float y, float z = 0, float h = 10.0f, float v = 10.0f,float r = 1, float g = 1, float b = 1, float a = 1 );
	void DrawTexturedPlanerXZQuad ( float x, float y, float z = 0, float h = 10.0f, float v = 10.0f,float r = 1, float g = 1, float b = 1, float a = 1 );
	
	void SetInnerGrid( float size);
	void SetOuterGrid( float size);
	void SetInnerGrisSpacing( float size );
	void SetDepthRange ( float xMaxZ );

	void SetBGColor ( trColor clr );
	void SetBGColor ( float r, float g, float b);

	int GetWinH ( void ) {return m_iViewportX;}
	int GetWinV ( void ) {return m_iViewportY;}

	HINSTANCE GetInstance ( void ) {return m_hInstance;}

	// view controls
	void GetCurrentViewRotation ( float *px, float *py, float *pz );
	void GetCurrentViewPosition ( float *px, float *py, float *pz );

	float	GetCurrentPullBack ( void ){ return m_fPullBack;}

	void RotateView ( float px, float py, float pz );
	void RotateViewInc ( float px, float py, float pz );

	void PanView ( float px, float py, float pz );
	void PanViewInc ( float px, float py, float pz );

	void SetPullBack ( float fPull ) { m_fPullBack = fPull;}
	void MovePullBack  ( float fPull ) { m_fPullBack += fPull;}

	void SetDrawBack ( void (*call)(void*), void *cookie);
	void CallDrawBack ( void );

	void SetViewType ( teViewType type ) { m_eViewMode = type;}

	bool SetOverlayMode ( void );
	void EndOverlayMode ( void );

	void MakeCurrent ( void ) {wglMakeCurrent(m_hdc, m_hGLRC);}
	
	float		m_aViewMatrix[16];

	bool		m_bHandleMouse;

	// lightinh stuff
	void SetLightInfo ( int iItem, float *pParams );

	void SetLight ( void );

private:
	HWND		m_hGLWindow;
	HINSTANCE	m_hInstance;
	int			m_iWinX,m_iWinY;
	char		m_szWindowName[255];

	bool		m_bRegistered;

	// Drawing flags
	bool		m_bDrawAxis;
	bool		m_bDrawGrid;
	bool		m_bDrawNormals;
	bool		m_bWireOverlay;

	bool		m_bDrawing;
	bool		m_bOverlayMode;

	void				*m_DrawCookie;
	tfDrawBackCB		m_DrawBack;

	// Body View Flags
	bool		m_bWire;
	bool		m_bShaded;
	bool		m_bTextured;
	bool		m_bSmoothed;
	bool		m_bLit;

	// open GL context variables
	HDC			m_hdc;
	HGLRC		m_hGLRC;
	HPALETTE	m_hPalette;

public:
	long		m_iViewportX;
	long		m_iViewportY;
	float		m_fov;
	float		m_fAspect;

private:
	float		m_aViewPos[3];
	float		m_aViewRot[3];
	float		m_fPullBack;

	float		m_fOuterGridSize,m_fInnerGridSize;
	float		m_fInnerGridSpace;

	float		m_xZRange;

	trColor		m_BackGroundColor;

	short		m_aLastPoint[2];
	short		m_aLastRMMPoint[2];

	// view state variables
	bool		m_bUseViewKeys;
	teViewType	m_eViewMode;

	bool		m_bReversePolySide;

public:
	// gl view functions
	void		InitGL ( long iX,long iY,HWND hWnd );
	void		KillGL ( void );
	bool		BeginDraw ( void );
	void		EndDraw ( void );

	bool		BeginSelect ( int x, int y );
	unsigned int EndSelect ( void );

	BOOL		QueryNewPal ( void );
	void		PalChanged ( void );

	void		SizeGlWindow(UINT nType, int cx, int cy);

	void		MouseButton ( int iButton, bool bDown, short iX, short iY, int iFlags );
	void		MouseMove ( short iX, short iY, int iFlags );
	void		MouseWheel ( short iParam );

	void		ReversePolySide ( bool bRev ) { m_bReversePolySide = bRev; }

private:
	void		SetupPixelFormat (void);
	void		SetupPalette (void);
	void		RedoPalette (void);
	bool		PaletteValid (void);

	void		GLCreate (HWND);
	void		GLDestroy (HWND);

	void		SetCurrnet (HWND);
	bool		GLRCValid (void);

	HDC			GLGetDC (void);
	
	void		LoadUILists ( bool bDraw = false );

	void		SetSize (int, int);
	void		GetSize (int *, int *);

	GLuint		*m_uiSelectBuf;

	float		m_fLight1Pos[3];
	float		m_fLight1AmbColor[3];
	float		m_fLight1DifuseColor[3];
	float		m_fLight1SpecularColor[3];

};

#endif // pkengine