// PocketEngine.cpp : Defines the entry point for the DLL application.
//

#define _WIN32_WINNT 0x0400

#include "PocketEngine.h"
#include <math.h>

#include <stdio.h>

// window proc for GL Window
int WINAPI MSGLWndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Open GL defines
#define		_GridOffset	0.0f // offset of grid in D

// default lists
#define		_GRID_LIST	1
#define		_AXIS_LIST	1

// menu button codes
#define		_LMB	1
#define		_RMB	2
#define		_MMB	3

#define		_SEL_BUFFER_SIZE	20000

BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

void CPocketEngine::SetDrawBack ( void (*call)(void*), void *cookie)
{
	m_DrawBack = call;
	m_DrawCookie = m_DrawBack;
}

void CPocketEngine::CallDrawBack ( void )
{
	if (m_DrawBack)
		(*m_DrawBack)(m_DrawCookie);
}

// Window Class

CPocketEngine::CPocketEngine( void )
{	
	// init everything to null and/or some defluat
	m_hGLWindow = NULL;
	m_hInstance = NULL;
	m_iWinX = -1;
	m_iWinY = -1;
	m_szWindowName[0] = NULL;

	m_fov = 40.0;

	m_fOuterGridSize = 1000.0f;
	m_fInnerGridSize = 100.0f;
	m_fInnerGridSpace = 5.0f;

	// white
	m_BackGroundColor.r = 1.0f;
	m_BackGroundColor.g = 1.0f;
	m_BackGroundColor.b = 1.0f;

	m_bDrawAxis = false;
	m_bDrawGrid = true;
	m_bDrawNormals = false;
	m_bWireOverlay = false;

	m_bUseViewKeys = true;

	m_xZRange = _FarClip;

	m_bRegistered = false;

	m_bReversePolySide = false;

	m_bDrawing = false;
	m_bOverlayMode = false;

	m_DrawCookie = NULL;
	m_DrawBack = NULL;

	m_aViewPos[0] = 0.0f;
	m_aViewPos[1] = 0.0f;
	m_aViewPos[2] = -15.0f;
	m_aViewRot[0] = 45.0f;
	m_aViewRot[1] = -45.0f;
	m_aViewRot[2] = 0.0f;

	m_eViewMode = ePartView;

	m_hGLRC = NULL;
	m_hdc = NULL;
	m_hPalette = NULL;

	m_bHandleMouse = false;

	m_uiSelectBuf = (GLuint*)malloc(sizeof(GLuint)*_SEL_BUFFER_SIZE);
}

CPocketEngine::~CPocketEngine( void )
{
	if (m_uiSelectBuf)
		free(m_uiSelectBuf);
	ReleaseCapture();
	Kill();
}

// creates the MSGL window and it's OpenGL contexts
bool CPocketEngine::Create ( int x, int y, HINSTANCE hInstance, HWND hParent)
{
	if (m_hGLWindow)
		return false;

	// if it's already up the kill it
	Kill();

	strcpy(m_szWindowName,"MyWin");
    WNDCLASS wc;

	m_hInstance = hInstance;

	if (!m_bRegistered) // only need to register the win class once
	{
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC) MSGLWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor (NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_szWindowName;

		RegisterClass (&wc);
		m_bRegistered = true;
	}
   
	int		iWindowStyle = 0;
	int		iOX,iOY;
	HMENU	hMENU;

	if ( hParent) // is it a child Window Style
	{
		iWindowStyle = WS_CHILD | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		iOX = 0;
		iOY = 0;
		hMENU = NULL; //no main menu if it's a child window
	}
	else
	{
		iWindowStyle = 	WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		iOX = CW_USEDEFAULT;
		iOY = CW_USEDEFAULT;
	}

	m_hGLWindow = CreateWindow (m_szWindowName, m_szWindowName,
								iWindowStyle,iOX, iOY, x, y,
								hParent, hMENU, hInstance, (int*)this); // the "this" saves the classpointer into the window class
	
	if(!m_hGLWindow)
		return false;

    ShowWindow (m_hGLWindow, SW_SHOWNORMAL);

	m_iWinX = x;
	m_iWinY = y;

	return true;
}

// kills the window, it's GL and it's Assembly
void CPocketEngine::Kill ( void )
{
	KillGL();

	if (m_hGLWindow)
		DestroyWindow(m_hGLWindow);

	m_hInstance = NULL;
	m_iWinX = -1;
	m_iWinY = -1;
	m_szWindowName[0] = NULL;

	m_hGLWindow = NULL;
}

// resize the openGL context
void CPocketEngine::SizeGlWindow(UINT nType, int cx, int cy) 
{
	m_fAspect = 1.33f;

	m_iViewportX = cx;
	m_iViewportY = cy;

	wglMakeCurrent(m_hdc, m_hGLRC);

	if (m_iViewportY != 0)
	m_fAspect = (float) m_iViewportX / (float) m_iViewportY;

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (m_fov,        // Field-of-view angle (def = 30)
					m_fAspect,      // Aspect ratio of viewing volume
					_NearClip,            // Distance to near clipping plane
					_FarClip);         // Distance to far clipping plane

	glViewport (0, 0, m_iViewportX, m_iViewportY);
}

void CPocketEngine::DrawTexturedPlanerXZQuad ( float x, float y, float z, float h, float v,float r, float g, float b, float a) 
{
	glPushMatrix();
	glColor4f(r,g,b,a);
	glTranslatef(x,y,z);
	glRotatef(-m_aViewRot[2],0,0,1);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex3f(-h*0.5f,0.0f,-v*0.5f);

		glTexCoord2f(1,1);
		glVertex3f(h*0.5f,0.0f,-v*0.5f);

		glTexCoord2f(1,0);
		glVertex3f(h*0.5f,0.0f,v*0.5f);
		
		glTexCoord2f(0,0);
		glVertex3f(-h*0.5f,0.0f,v*0.5f);
	glEnd();

	glColor4f(1.0f,1.0f,1.0,1.0f);
	glPopMatrix();
}



void CPocketEngine::DrawTexturedPlanerQuad ( float x, float y, float z, float h, float v,float r, float g, float b, float a) 
{
	glPushMatrix();
	glColor4f(r,g,b,a);
	glTranslatef(x,y,z);
	glRotatef(-m_aViewRot[2],0,0,1);
	glRotatef(-m_aViewRot[0],1,0,0);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex3f(-h*0.5f,0.0f,-v*0.5f);

		glTexCoord2f(1,1);
		glVertex3f(h*0.5f,0.0f,-v*0.5f);

		glTexCoord2f(1,0);
		glVertex3f(h*0.5f,0.0f,v*0.5f);
		
		glTexCoord2f(0,0);
		glVertex3f(-h*0.5f,0.0f,v*0.5f);
	glEnd();

	glColor4f(1.0f,1.0f,1.0,1.0f);
	glPopMatrix();
}

void CPocketEngine::DrawFlatXYQuad ( float x, float y, float z, float h, float v,float r, float g, float b, float a)
{
	
	if (m_bOverlayMode)
	{
		glPushMatrix();
		glLoadIdentity();
	}

	glColor4f(r,g,b,a);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glVertex3f((float)x,(float)y,(float)z);
		glVertex3f((float)x+h,(float)y,(float)z);
		glVertex3f((float)x+h,(float)y+v,(float)z);
		glVertex3f((float)x,y+(float)v,(float)z);
	glEnd();

	glColor4f(1,1,1,1);

	if (m_bOverlayMode)
		glPopMatrix();
}

void CPocketEngine::DrawTexturedXYQuad ( float x, float y, float z, float h, float v,float r, float g, float b, float a)
{
	if (m_bOverlayMode)
	{
		glPushMatrix();
		glLoadIdentity();
	}

	glColor4f(r,g,b,a);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f((float)x,(float)y,(float)z);

		glTexCoord2f(1,0);
		glVertex3f((float)x+h,(float)y,(float)z);

		glTexCoord2f(1,1);
		glVertex3f((float)x+h,(float)y+v,(float)z);

		glTexCoord2f(0,1);
		glVertex3f((float)x,y+(float)v,(float)z);
	glEnd();

	glColor4f(1,1,1,1);

	if (m_bOverlayMode)
		glPopMatrix();

}

// initalise the OpenGL context
void CPocketEngine::InitGL ( long iX,long iY,HWND hWnd )
{
	m_iViewportX = iX;
	m_iViewportY = iY;

	// create a new context
	GLCreate (hWnd);

	// set the BG color
	glClearColor ((float)m_BackGroundColor.r, (float)m_BackGroundColor.g, (float)m_BackGroundColor.b, 1.0);

	// make everything look it's best not fastest.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT ,GL_NICEST);

	// we want a z buffer
	glEnable (GL_DEPTH_TEST);

	// we want back face culling
    glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// pick a poly winding
	if (m_bReversePolySide)
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);

	// we want smooth filled polies
	glShadeModel (GL_SMOOTH);
	glPolygonMode (GL_FRONT, GL_FILL);

    // add 2 lights
	float	f[4] = {1.0f};
	f[0] = f[1] = f[2] = 0.25f;

	SetLightInfo(eLightAmb,f);

	f[0] = f[1] = f[2] = 0.60f;
	SetLightInfo(eLightDif,f);

	f[0] = f[1] = f[2] = 0.60f;
	SetLightInfo(eLightSpec,f);

	f[0] = 0;f[1] = 0;f[2] = 10.0f;
	SetLightInfo(eLightPos,f);

	SetLight();
	// but only enable one ?!?
	glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0); 

	// we want alpha based transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_COLOR_MATERIAL);

	// set the projection matrix and viewport info
	SetDepthRange(m_xZRange);
	
	// clear out the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	// build the Grid list
	LoadUILists();
}

void CPocketEngine::SetLightInfo ( int iItem, float *pParams )
{
	switch(iItem)
	{
		case eLightPos:
			memcpy(m_fLight1Pos,pParams,sizeof(float)*3);
			break;
		case eLightSpec:
			memcpy(m_fLight1SpecularColor,pParams,sizeof(float)*3);
			break;
		case eLightDif:
			memcpy(m_fLight1DifuseColor,pParams,sizeof(float)*3);
			break;
		case eLightAmb:
			memcpy(m_fLight1AmbColor,pParams,sizeof(float)*3);
			break;
	}
}

void CPocketEngine::SetLight ( void )
{
	glLightfv (GL_LIGHT0, GL_AMBIENT,m_fLight1AmbColor);
	glLightfv (GL_LIGHT0, GL_DIFFUSE,m_fLight1DifuseColor);
	glLightfv (GL_LIGHT0, GL_SPECULAR,m_fLight1SpecularColor);
	glLightfv (GL_LIGHT0, GL_POSITION,m_fLight1Pos);
}

void CPocketEngine::SetBGColor ( float r, float g, float b)
{
	trColor clr = {r,g,b};
	SetBGColor(clr);
}

void CPocketEngine::SetBGColor ( trColor clr )
{
	m_BackGroundColor = clr; 
	glClearColor ((float)m_BackGroundColor.r, (float)m_BackGroundColor.g, (float)m_BackGroundColor.b, 1.0);
}


// chnages the grid constants and re builds the lists
void  CPocketEngine::SetInnerGrid( float size)
{
	m_fInnerGridSize = size;
	LoadUILists();
}

void  CPocketEngine::SetOuterGrid( float size) 
{ 
	m_fOuterGridSize  = size;
	LoadUILists();
}

void CPocketEngine::SetInnerGrisSpacing( float size )
{
	m_fInnerGridSpace = size;
	LoadUILists();
}

// sets the range for the view frustum ( and z buffer range )
void  CPocketEngine::SetDepthRange ( float xMaxZ )
{
	m_xZRange = xMaxZ;

	wglMakeCurrent(m_hdc, m_hGLRC);
	if (GLRCValid())
	{
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		gluPerspective (m_fov,         // Field-of-view angle (def = 30)
					m_fAspect, // Aspect ratio of viewing volume
					_NearClip,            // Distance to near clipping plane
					m_xZRange);         // Distance to far clipping plane
		glViewport (0, 0, m_iViewportX, m_iViewportY);
	}
}

// gets and sets the current view pos and rot
void CPocketEngine::GetCurrentViewRotation ( float *px, float *py, float *pz )
{
	if (!px || !py || !pz)
		return;

	*px = m_aViewRot[0];
	*py = m_aViewRot[1];
	*pz = m_aViewRot[2];
}

void CPocketEngine::GetCurrentViewPosition ( float *px, float *py, float *pz )
{
	if (!px || !py || !pz)
		return;

	*px = m_aViewPos[0];
	*py = m_aViewPos[1];
	*pz = m_aViewPos[2];
}

void CPocketEngine::RotateView ( float px, float py, float pz )
{	
	m_aViewRot[0] = px;
	m_aViewRot[1] = py;
	m_aViewRot[2] = pz;
}

void CPocketEngine::RotateViewInc ( float px, float py, float pz )
{
	m_aViewRot[0] += px;
	m_aViewRot[1] += py;
	m_aViewRot[2] += pz;
}

void CPocketEngine::PanView ( float px, float py, float pz )
{
	m_aViewPos[0] = px;
	m_aViewPos[1] = py;
	m_aViewPos[2] = pz;
}

void CPocketEngine::PanViewInc ( float px, float py, float pz )
{
	if (m_eViewMode == eQuestType)
	{
		// do the vertical part
		m_aViewPos[0]+= (float)sin(m_aViewRot[2]*0.017453f) * py;
		m_aViewPos[1]-= (float)cos(m_aViewRot[2]*0.017453f) * py;

		// do the hirozonatl part
		m_aViewPos[0]+= (float)sin((m_aViewRot[2]-90.0f)*0.017453f) * px;
		m_aViewPos[1]-= (float)cos((m_aViewRot[2]-90.0f)*0.017453f) * px;

		m_aViewPos[2] += pz;
	}
	else
	{
		m_aViewPos[0] += px;
		m_aViewPos[1] += py;
		m_aViewPos[2] += pz;
	}
}

// draw the grid and tell the asembly to draw
bool CPocketEngine::BeginDraw ( void )
{
	if (m_bDrawing || !m_hGLRC | !m_hdc)
		return false;

	m_bDrawing = true;
	m_bOverlayMode = false;

	// make sure that our openGL Context is the current one
	wglMakeCurrent(m_hdc, m_hGLRC);

	// rest GL
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
	
	switch (m_eViewMode)
	{
		case eMatrix:
			glMultMatrixf(m_aViewMatrix);
			break;

		case ePartView:
			glTranslatef(0,0,m_aViewPos[2]);
			glRotatef(m_aViewRot[0], 1.0f, 0.0f, 0.0f);
			glTranslatef(m_aViewPos[0],m_aViewPos[1],0);
			glRotatef(m_aViewRot[1], 0.0f, 1.0f, 0.0f);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			break;

		case eQuakeType:
			glRotatef(m_aViewRot[1], 0.0f, 0.0f, 1.0f);
			glRotatef(m_aViewRot[0], 1.0f, 0.0f, 0.0f);
			glRotatef(m_aViewRot[2], 0.0f, 1.0f, 0.0f);
			glTranslatef(m_aViewPos[0],m_aViewPos[1],m_aViewPos[2]);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			break;
		
		case eQuestType:
			glTranslatef(0,0,-m_fPullBack);				// pull back on allong the zoom vector
			glRotatef(m_aViewRot[0], 1.0f, 0.0f, 0.0f);			// pops us to the tilt
			glRotatef(m_aViewRot[2], 0.0f, 1.0f, 0.0f);			// gets us on our rot
			glTranslatef(-m_aViewPos[0],-m_aViewPos[2],-m_aViewPos[1]);	// take us to the pos
			glRotatef(-90, 1.0f, 0.0f, 0.0f);			// gets us into XY
			break;

	}
	// save the matrix
	glPushMatrix();
	
	// do our grid
	if (m_bDrawGrid)
		glCallList(_GRID_LIST);

	glColor3f(0.5f,0.5f,0.5f);

	return m_bDrawing;
}

void CPocketEngine::EndDraw ( void )
{
	// make sure that our openGL Context is the current one
	wglMakeCurrent(m_hdc, m_hGLRC);

	if (!m_bDrawing)
		return;

	if (m_bOverlayMode)
		EndOverlayMode();

	m_bDrawing = false;

	// put the matrix back
	glPopMatrix();
	//	flip it
	SwapBuffers(m_hdc);
}

bool CPocketEngine::BeginSelect ( int x, int y )
{
	if (m_bDrawing || !m_hGLRC | !m_hdc)
		return false;

	m_bDrawing = true;
	m_bOverlayMode = false;

	// make sure that our openGL Context is the current one
	wglMakeCurrent(m_hdc, m_hGLRC);

	GLint	viewport[4] = {0};
	
	memset(m_uiSelectBuf,0,sizeof(GLuint)*_SEL_BUFFER_SIZE);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(_SEL_BUFFER_SIZE, m_uiSelectBuf);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(-1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y),2.0, 2.0, viewport);
	gluPerspective (m_fov,m_fAspect,_NearClip,m_xZRange);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	switch (m_eViewMode)
	{
		case eMatrix:
			glMultMatrixf(m_aViewMatrix);
			break;

		case ePartView:
			glTranslatef(0,0,m_aViewPos[2]);
			glRotatef(m_aViewRot[0], 1.0f, 0.0f, 0.0f);
			glTranslatef(m_aViewPos[0],m_aViewPos[1],0);
			glRotatef(m_aViewRot[1], 0.0f, 1.0f, 0.0f);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			break;

		case eQuakeType:
			glRotatef(m_aViewRot[1], 0.0f, 0.0f, 1.0f);
			glRotatef(m_aViewRot[0], 1.0f, 0.0f, 0.0f);
			glRotatef(m_aViewRot[2], 0.0f, 1.0f, 0.0f);
			glTranslatef(m_aViewPos[0],m_aViewPos[1],m_aViewPos[2]);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			break;

		case eQuestType:
			glTranslatef(0,0,-m_fPullBack);				// pull back on allong the zoom vector
			glRotatef(m_aViewRot[0], 1.0f, 0.0f, 0.0f);			// pops us to the tilt
			glRotatef(m_aViewRot[2], 0.0f, 1.0f, 0.0f);			// gets us on our rot
			glTranslatef(-m_aViewPos[0],-m_aViewPos[2],-m_aViewPos[1]);	// take us to the pos
			glRotatef(-90, 1.0f, 0.0f, 0.0f);			// gets us into XY
			break;
	}

	return true;
}

unsigned int CPocketEngine::EndSelect ( void )
{
	if (!m_bDrawing)
		return -1;

	GLint	hits = 0;

	long	iTopName = -1;
	float	LastZ = 0.0f;
	bool	bFirstShot = true;
	long	i;

	unsigned	int j;
	GLuint		names = 0, *ptr = NULL;
	float		z1 = 0,z2 = 0;
	long		iName = 0;

	// make sure that our openGL Context is the current one
//	wglMakeCurrent(m_hdc, m_hGLRC);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();
	
	// end render
	hits = glRenderMode(GL_RENDER);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (m_fov,m_fAspect,_NearClip,m_xZRange);
	glViewport (0, 0, m_iViewportX, m_iViewportY);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
	
	char	msg[255];

	if (hits >0)
	{
		ptr = (GLuint *) m_uiSelectBuf;

		for (i = 0; i < hits; i++) 
		{  /* for each hit  */
			names = *ptr;
			ptr++;
			z1 = (float) *ptr/0x7fffffff;
			ptr++;
			z2 = (float) *ptr/0x7fffffff;
			ptr++;
			
			// stupid compiler bug
			sprintf(msg,"names %d f1 %f f2 %f",names,z1,z2);

			if (bFirstShot)
			{
				LastZ = z1;
				bFirstShot = false;
			}
			for (j = 0; j < names; j++)
			{  /* for each name */
				iName = *ptr;
				ptr++;
			}
			if (z1 <= LastZ)
			{
				LastZ = z1;
				iTopName = iName;
			}
		}

	}
	m_bDrawing = false;

	if (iTopName == -1)
		return NULL;
	else
		return iTopName;
}

bool CPocketEngine::SetOverlayMode ( void )
{
	if (m_bOverlayMode)
		return true;

	if (!m_bDrawing)
		return false;

	m_bOverlayMode = true;

	// make sure that our openGL Context is the current one
//	wglMakeCurrent(m_hdc, m_hGLRC);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0.0,(double)m_iViewportX,
			0.0,(double)m_iViewportY,
			0.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);

	return true;
}

void CPocketEngine::EndOverlayMode ( void )
{
	// make sure that our openGL Context is the current one
	wglMakeCurrent(m_hdc, m_hGLRC);

	if (m_bDrawing && m_bOverlayMode)
	{
		glPopMatrix();
		
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		gluPerspective (m_fov,         // Field-of-view angle (def = 30)
					m_fAspect, // Aspect ratio of viewing volume
					_NearClip,            // Distance to near clipping plane
					m_xZRange);         // Distance to far clipping plane
		glViewport (0, 0, m_iViewportX, m_iViewportY);

		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHTING);
		m_bOverlayMode = false;
	}
}

// OpenGL core code ( don't change this unless it's needed
void CPocketEngine::KillGL ( void )
{

	GLDestroy (m_hGLWindow);
}

BOOL CPocketEngine::QueryNewPal ( void )
{
	if (GLRCValid() && PaletteValid()) 
	{
		RedoPalette ();
		return true;
	}
	else 
		return false;
}

void CPocketEngine::PalChanged ( void )
{
	RedoPalette ();
}

void CPocketEngine::GLCreate (HWND hWnd)
{
	m_hdc = GetDC(hWnd);

	SetupPixelFormat ();
	SetupPalette ();
	m_hGLRC = wglCreateContext (m_hdc);
	wglMakeCurrent(m_hdc, m_hGLRC);
}

void CPocketEngine::SetCurrnet ( HWND hWnd )
{
	SetupPixelFormat ();
	SetupPalette ();
	wglMakeCurrent(m_hdc, m_hGLRC);
}

void CPocketEngine::SetSize (int iX, int iY)
{
	m_iViewportX = iX;
	m_iViewportY = iY;
}

void CPocketEngine::GetSize (int *iX, int *iY)
{
	if (iX != NULL && iY != NULL)
	{
		m_iViewportX = *iX;
		m_iViewportY = *iY;
	}
}

void CPocketEngine::GLDestroy (HWND hWnd)
{
	if (m_hGLRC) 
	{
		wglMakeCurrent (NULL, NULL);
		wglDeleteContext(m_hGLRC);
	}

	if (m_hPalette) 
	{
		DeleteObject (m_hPalette);
		
	}
	m_hGLRC = NULL;
	m_hdc = NULL;
	m_hPalette = NULL;
}

bool CPocketEngine::GLRCValid ( void )
{
	if (m_hGLRC)
		return true;
	else
		return false;
}

bool CPocketEngine::PaletteValid ( void )
{
	if (m_hPalette)
		return true;
	else
		return false;
}

HDC CPocketEngine::GLGetDC ( void )
{
	return m_hdc;
}

void CPocketEngine::RedoPalette ( void )
{
	UnrealizeObject (m_hPalette);
	SelectPalette (m_hdc, m_hPalette, false);
	RealizePalette (m_hdc);
}

void CPocketEngine::SetupPixelFormat ( void )
{
	 HANDLE hHeap;
    int nColors, i;
    LPLOGPALETTE lpPalette;
    BYTE byRedMask, byGreenMask, byBlueMask;

    static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof (PIXELFORMATDESCRIPTOR),             // Size of this structure
        1,                                          // Version number
        PFD_DRAW_TO_WINDOW |                        // Flags
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER ,
        PFD_TYPE_RGBA,                              // RGBA pixel values
        32,                                         // 24-bit color
        0, 0, 0, 0, 0, 0,                           // Don't care about these
        0, 0,                                       // No alpha buffer
        0, 0, 0, 0, 0,                              // No accumulation buffer
        32,                                         // 32-bit depth buffer
        0,                                          // No stencil buffer
        0,                                          // No auxiliary buffers
        PFD_MAIN_PLANE,                             // Layer type
        0,                                          // Reserved (must be 0)
        0, 0, 0                                     // No layer masks
    };

    int nPixelFormat;
    
    nPixelFormat = ChoosePixelFormat (m_hdc, &pfd);
    SetPixelFormat (m_hdc, nPixelFormat, &pfd);

    DescribePixelFormat (m_hdc, nPixelFormat, sizeof (PIXELFORMATDESCRIPTOR),
        &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        nColors = 1 << pfd.cColorBits;
        hHeap = GetProcessHeap ();

        (LPLOGPALETTE) lpPalette = (LPLOGPALETTE)HeapAlloc (hHeap, 0,
            sizeof (LOGPALETTE) + (nColors * sizeof (PALETTEENTRY)));
            
        lpPalette->palVersion = 0x300;
        lpPalette->palNumEntries = nColors;

        byRedMask = (1 << pfd.cRedBits) - 1;
        byGreenMask = (1 << pfd.cGreenBits) - 1;
        byBlueMask = (1 << pfd.cBlueBits) - 1;

        for (i=0; i<nColors; i++) {
            lpPalette->palPalEntry[i].peRed =
                (((i >> pfd.cRedShift) & byRedMask) * 255) / byRedMask;
            lpPalette->palPalEntry[i].peGreen =
                (((i >> pfd.cGreenShift) & byGreenMask) * 255) / byGreenMask;
            lpPalette->palPalEntry[i].peBlue =
                (((i >> pfd.cBlueShift) & byBlueMask) * 255) / byBlueMask;
            lpPalette->palPalEntry[i].peFlags = 0;
        }

        m_hPalette = CreatePalette (lpPalette);
        HeapFree (hHeap, 0, lpPalette);

        if (m_hPalette != NULL) {
            SelectPalette (m_hdc, m_hPalette, FALSE);
            RealizePalette (m_hdc);
        }
    }
}

void CPocketEngine::SetupPalette ( void )
{
	int pixelFormat = GetPixelFormat(m_hdc);
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE* pPal;
	int paletteSize;

	DescribePixelFormat(m_hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		paletteSize = 1 << pfd.cColorBits;
	} 
	else 
	{
		return;
	}

	pPal = (LOGPALETTE*)
		malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
	pPal->palVersion = 0x300;
	pPal->palNumEntries = paletteSize;

	// build a simple RGB color palette 
	{
		int redMask = (1 << pfd.cRedBits) - 1;
		int greenMask = (1 << pfd.cGreenBits) - 1;
		int blueMask = (1 << pfd.cBlueBits) - 1;
		int i;

		for (i=0; i<paletteSize; ++i)
		{
			pPal->palPalEntry[i].peRed =
				(((i >> pfd.cRedShift) & redMask) * 255) / redMask;
			pPal->palPalEntry[i].peGreen =
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
			pPal->palPalEntry[i].peBlue =
				(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
			pPal->palPalEntry[i].peFlags = 0;
		}
	}

	m_hPalette = (HPALETTE)CreatePalette(pPal);
	free(pPal);

	if (m_hPalette)
	{
		SelectPalette(m_hdc, m_hPalette, false);
		RealizePalette(m_hdc);
	}
}

// generates the grids display list or draws it manyaly if needed
void CPocketEngine::LoadUILists ( bool bDraw )
{
	if (!bDraw)
	{
		if (glIsList(_GRID_LIST))
			glDeleteLists(_GRID_LIST,1);
			
		glNewList (_GRID_LIST,GL_COMPILE);
	}
	glDisable(GL_TEXTURE_2D);

	glNormal3f(0.0f,1.0f,0.0f);
	// draw the grid
	glLineWidth (1.0f);

	float c[4] = {0.5f,0.5f,0.5f,1.0f};

	c[0] = 0.1F; c[1] = 0.1F; c[2] = 0.5F;
//	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);

	glColor3f(c[0],c[1],c[2]);
	float	i;
	glBegin(GL_LINES);
	for (i = -m_fInnerGridSize;i<= m_fInnerGridSize;i+= m_fInnerGridSpace)
	{
		glVertex3f ((float)-m_fInnerGridSize,(float)i,_GridOffset);
		glVertex3f ((float)m_fInnerGridSize,(float)i,_GridOffset);
	}
	glEnd();

	glBegin(GL_LINES);
	for (i = -m_fInnerGridSize;i<= m_fInnerGridSize;i+= m_fInnerGridSpace)
	{
		glVertex3f ((float)i,(float)-m_fInnerGridSize,_GridOffset);
		glVertex3f ((float)i,(float)m_fInnerGridSize,_GridOffset);
	}
	glEnd();


	c[0] = 0.2F; c[1] = 0.2F; c[2] = 0.2F;
//	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
	glColor3f(c[0],c[1],c[2]);


	glBegin(GL_LINES);
	for (i = -m_fOuterGridSize;i<= m_fOuterGridSize;i+=m_fInnerGridSize)
	{
		glVertex3f ((float)-m_fOuterGridSize,(float)i,_GridOffset);
		glVertex3f ((float)m_fOuterGridSize,(float)i,_GridOffset);
		
	}
	glEnd();

	glBegin(GL_LINES);
	for (i = -m_fOuterGridSize;i<= m_fOuterGridSize;i+=m_fInnerGridSize)
	{
		glVertex3f ((float)i,(float)-m_fOuterGridSize,_GridOffset);
		glVertex3f ((float)i,(float)m_fOuterGridSize,_GridOffset);
	}
	glEnd();

	// y axis
	c[0] = 1.0F; c[1] = 0.0F; c[2] = 0.0F;
//	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
	glColor3f(c[0],c[1],c[2]);

	glLineWidth (3.0F);
	glBegin(GL_LINE_STRIP);
		glVertex3f (0,0,0);
		glVertex3f (0,(float)m_fInnerGridSize,0);
		glVertex3f (0,m_fInnerGridSize * 0.8f,m_fInnerGridSize * 0.1f);
		glVertex3f (0,m_fInnerGridSize * 0.8f,0.0f);
	glEnd();

	// "Y"
	glBegin(GL_LINES);
		glVertex3f (-m_fInnerGridSize * 0.05f,m_fInnerGridSize,0);
		glVertex3f (-m_fInnerGridSize * 0.1f,m_fInnerGridSize * 0.9f,0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f (-m_fInnerGridSize * 0.11f,m_fInnerGridSize,0);
		glVertex3f (-m_fInnerGridSize * 0.076f,m_fInnerGridSize * 0.95f,0);
	glEnd(); 

	// X Axis
//	glColor3f (0.0F, 1.0F, 0.0F);
	c[0] = 0.0F; c[1] = 1.0F; c[2] = 0.0F;
	//glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
	glColor3f(c[0],c[1],c[2]);

	glLineWidth (3.0F);
	glBegin(GL_LINE_STRIP);
		glVertex3f (0,0,0);
		glVertex3f ((float)m_fInnerGridSize,0,0);
		glVertex3f (m_fInnerGridSize * 0.8f,m_fInnerGridSize * 0.1f,0);
		glVertex3f (m_fInnerGridSize * 0.8f,0,0);
	glEnd();

	// "X"
	glBegin(GL_LINES);
		glVertex3f (m_fInnerGridSize * 0.95f,0,m_fInnerGridSize * 0.15f);
		glVertex3f (m_fInnerGridSize * 0.87f,0,m_fInnerGridSize * 0.05f);
		glVertex3f (m_fInnerGridSize * 0.87f,0,m_fInnerGridSize * 0.15f);
		glVertex3f (m_fInnerGridSize * 0.95f,0,m_fInnerGridSize * 0.05f);
	glEnd();

//	Z Axis
//	glColor3f (0.0F, 0.0F, 1.0F);
	c[0] = 0.0F; c[1] = 0.0F; c[2] = 1.0F;
//	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
	glColor3f(c[0],c[1],c[2]);

	glLineWidth (3.0F);
	glBegin(GL_LINE_STRIP);
		glVertex3f (0,0,0);
		glVertex3f (0,0,(float)m_fInnerGridSize);
		glVertex3f (m_fInnerGridSize * 0.1f,0,m_fInnerGridSize * 0.8f);
		glVertex3f (0,0,m_fInnerGridSize * 0.8f);
	glEnd();

	// end grid
	glLineWidth (1.0F);

	if (!bDraw)
		glEndList();
}


void CPocketEngine::MouseButton ( int iButton, bool bDown, short iX, short iY, int iFlags )
{
	if (!m_bUseViewKeys)
		return;

	bool	bSelect = true;

	int		iRealButton = iButton;

	if (GetSystemMetrics(SM_SWAPBUTTON))
	{
		if (iRealButton == _LMB)
			iRealButton = _RMB;
		else if (iRealButton == _RMB)
			iRealButton = _LMB;
	}
	
	m_aLastPoint[0] = iX;
	m_aLastPoint[1] = iY;

	switch(iRealButton)
	{
		case _LMB:

			if (bDown)
				SetCapture(m_hGLWindow);
			else
				ReleaseCapture();
		break;
		
		case _MMB:

		break;
	}
}

void CPocketEngine::MouseWheel ( short iParam )
{
	if (!m_bUseViewKeys)
		return;

	m_aViewPos[2] += (iParam/120.0f) * (m_aViewPos[2]/16.0f) ;
	CallDrawBack();
}

void CPocketEngine::MouseMove ( short iX, short iY, int iFlags )
{
	bool bNeedRedraw = false;
	float	xXDelta,xYDelta,xTemp = 0;
	int		iMoveMode = 0;
	
	if (!m_bUseViewKeys)
		return;

	if ( (iFlags & MK_LBUTTON) || ((GetSystemMetrics(SM_SWAPBUTTON)) && (iFlags & MK_RBUTTON)) )
	{
		// get the view mode
		if (m_bUseViewKeys)
		{
			if (iFlags & MK_SHIFT)
				iMoveMode = MSGLMODE_ROT;
			else if (iFlags & MK_CONTROL)
				iMoveMode = MSGLMODE_PAN;
			else if ( (GetAsyncKeyState(18) ) )// || (GetAsyncKeyState(VK_RMENU) == 0x0800))
				iMoveMode = MSGLMODE_ZOOM;
			else
				iMoveMode = MSGLMODE_SELECT;
		}

		//get and save the point
		xXDelta = ((float)m_aLastPoint[0] - (float)iX);
		xYDelta = ((float)m_aLastPoint[1] - (float)iY);
		m_aLastPoint[0] = iX;
		m_aLastPoint[1] = iY;

		float fGoodZoom = m_aViewPos[2];

		// we have a drag
		switch (iMoveMode)
		{	
			case MSGLMODE_ROT:

				if (fabs(fGoodZoom) > 100.0f)
					fGoodZoom = 100.0f * (fGoodZoom/(float)fabs(fGoodZoom));

				xXDelta *= (fGoodZoom/120.0f);
				xYDelta *= (fGoodZoom/120.0f);

				RotateViewInc(xTemp,xXDelta,xTemp);
			break;

			case MSGLMODE_PAN:
				xXDelta *= (m_aViewPos[2]/800.0f);
				xYDelta *= (m_aViewPos[2]/-800.0f);
				
				PanViewInc(xXDelta,xYDelta,xTemp);
			break;

			case MSGLMODE_ZOOM:
				xYDelta = (xYDelta/30.0f) * (m_aViewPos[2]/16.0f) ;
				PanViewInc(xTemp,xTemp,xYDelta);
			break;

		}
		CallDrawBack();
	}
}

void CPocketEngine::SizeWindow( int x, int y )
{
	::MoveWindow(m_hGLWindow,0,0,x,y,false);
}

// win proc
int WINAPI MSGLWndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CPocketEngine	*pPKWin;

	CREATESTRUCT	*pCS;

	long iWinLong = GetWindowLong(hWnd,GWL_USERDATA);;
	
	if (iWinLong)
		pPKWin = (CPocketEngine*)GetWindowLong(hWnd,GWL_USERDATA);
	else
		pPKWin = NULL;

	HWND parent = GetParent(hWnd);
	switch (msg)
	{
		case WM_CREATE:
			pCS = (CREATESTRUCT*) lParam;

			pPKWin =(CPocketEngine*)pCS->lpCreateParams ;
			if (!pPKWin)
			{
				DestroyWindow(hWnd);
				return 0;
			}

			SetWindowLong(hWnd,GWL_USERDATA,(long)pPKWin);

			RECT	Rect;

			GetClientRect(hWnd,&Rect);
			pPKWin->InitGL (Rect.right,Rect.top,hWnd);
	
			return 0;

		case WM_DESTROY:
			if (pPKWin)
			{
				pPKWin->SetWindow(NULL);
				pPKWin->Kill();
			}
			//PostQuitMessage (0);
			return 0;

		case WM_QUERYNEWPALETTE:
			if (pPKWin->QueryNewPal())
				return 0;
			else
				break;

		case WM_PALETTECHANGED:
			if (pPKWin)
				pPKWin->PalChanged();
			return 0;

		case WM_SIZE:
			if (pPKWin)
				pPKWin->SizeGlWindow(wParam,LOWORD(lParam),HIWORD(lParam) );
			return 0;

		case WM_PAINT:
			ValidateRect(hWnd,NULL);
			if (pPKWin)
				pPKWin->CallDrawBack();
			return 0;

		case WM_LBUTTONDOWN:
				if (pPKWin && pPKWin->m_bHandleMouse)
				{
					pPKWin->MouseButton(_LMB,true,(short)LOWORD(lParam),(short)HIWORD(lParam),wParam);
					return 0;
				}

				if (parent)
					SendMessage(parent,msg, wParam, lParam);

				break;

		case WM_LBUTTONUP:
				if (pPKWin && pPKWin->m_bHandleMouse)
				{
					pPKWin->MouseButton(_LMB,false,(short)LOWORD(lParam),(short)HIWORD(lParam),wParam);
					return 0;
				}

				if (parent)
					SendMessage(parent,msg, wParam, lParam);

				break;

		case WM_RBUTTONDOWN:
				if (pPKWin && pPKWin->m_bHandleMouse)
				{
					pPKWin->MouseButton(_RMB,true,(short)LOWORD(lParam),(short)HIWORD(lParam),wParam);
					return 0;
				}

				if (parent)
					SendMessage(parent,msg, wParam, lParam);

				break;

		case WM_RBUTTONUP:
				if (pPKWin && pPKWin->m_bHandleMouse)
				{
					pPKWin->MouseButton(_RMB,false,(short)LOWORD(lParam),(short)HIWORD(lParam),wParam);
					return 0;
				}

				if (parent)
					SendMessage(parent,msg, wParam, lParam);

				break;

		case WM_MOUSEMOVE:
				if (pPKWin && pPKWin->m_bHandleMouse)
				{
					pPKWin->MouseMove((short)LOWORD(lParam),(short)HIWORD(lParam),wParam);
					return 0;
				}

				if (parent)
					SendMessage(parent,msg, wParam, lParam);

				break;

		case WM_MOUSEWHEEL:
				if (pPKWin && pPKWin->m_bHandleMouse)
				{
					pPKWin->MouseWheel((short)HIWORD(wParam));
					return 0;
				}

				if (parent)
					SendMessage(parent,msg, wParam, lParam);

				break;
	}
	return DefWindowProc (hWnd, msg, wParam, lParam);
}