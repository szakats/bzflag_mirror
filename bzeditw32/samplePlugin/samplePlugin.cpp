// samplePlugin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "samplePlugin.h"
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

#include "..\BZEdit32\api.h"

/////////////////////////////////////////////////////////////////////////////
// CSamplePluginApp

BEGIN_MESSAGE_MAP(CSamplePluginApp, CWinApp)
	//{{AFX_MSG_MAP(CSamplePluginApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSamplePluginApp construction

CSamplePluginApp::CSamplePluginApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSamplePluginApp object

CSamplePluginApp theApp;


int init_plugin ( char *szLabel )
{
	strcpy(szLabel,"Add a Box");
	return 0;
}

int run_plugin ( void * param )
{
	AddObject("BOX");
	UpdateWorld();
	SetDocDirty();
	return 0;
}