// BZEdit32.h : main header file for the BZEDIT32 application
//

#if !defined(AFX_BZEDIT32_H__3013151F_D29C_4136_8C32_2A1FC3C9B1F3__INCLUDED_)
#define AFX_BZEDIT32_H__3013151F_D29C_4136_8C32_2A1FC3C9B1F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32App:
// See BZEdit32.cpp for the implementation of this class
//

class CBZEdit32App : public CWinApp
{
public:
	CBZEdit32App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBZEdit32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBZEdit32App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BZEDIT32_H__3013151F_D29C_4136_8C32_2A1FC3C9B1F3__INCLUDED_)
