// NewEntToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "bzedit32.h"
#include "NewEntToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewEntToolBar

CNewEntToolBar::CNewEntToolBar()
{
}

CNewEntToolBar::~CNewEntToolBar()
{
}


BEGIN_MESSAGE_MAP(CNewEntToolBar, CToolBarCtrl)
	//{{AFX_MSG_MAP(CNewEntToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewEntToolBar message handlers

BOOL CNewEntToolBar::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CToolBarCtrl::OnCommand(wParam, lParam);
}
