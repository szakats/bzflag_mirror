#if !defined(AFX_NEWENTTOOLBAR_H__FB1D64C6_5B7A_43BB_9E79_8E37DEBFE9A7__INCLUDED_)
#define AFX_NEWENTTOOLBAR_H__FB1D64C6_5B7A_43BB_9E79_8E37DEBFE9A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewEntToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewEntToolBar window

class CNewEntToolBar : public CToolBarCtrl
{
// Construction
public:
	CNewEntToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewEntToolBar)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewEntToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNewEntToolBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWENTTOOLBAR_H__FB1D64C6_5B7A_43BB_9E79_8E37DEBFE9A7__INCLUDED_)
