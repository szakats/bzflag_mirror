// samplePlugin.h : main header file for the SAMPLEPLUGIN DLL
//

#if !defined(AFX_SAMPLEPLUGIN_H__1DBB32B3_6642_42AC_93E8_78E8C4C506A5__INCLUDED_)
#define AFX_SAMPLEPLUGIN_H__1DBB32B3_6642_42AC_93E8_78E8C4C506A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSamplePluginApp
// See samplePlugin.cpp for the implementation of this class
//

class CSamplePluginApp : public CWinApp
{
public:
	CSamplePluginApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSamplePluginApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSamplePluginApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEPLUGIN_H__1DBB32B3_6642_42AC_93E8_78E8C4C506A5__INCLUDED_)
