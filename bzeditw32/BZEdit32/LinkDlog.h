#if !defined(AFX_LINKDLOG_H__3A90B592_6582_4179_8D4E_683A9EC9D8B1__INCLUDED_)
#define AFX_LINKDLOG_H__3A90B592_6582_4179_8D4E_683A9EC9D8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkDlog.h : header file
//

#include "World.h"

/////////////////////////////////////////////////////////////////////////////
// CLinkDlog dialog

class CLinkDlog : public CDialog
{
// Construction
public:
	CLinkDlog(CWnd* pParent = NULL);   // standard constructor

	CWorld		*m_pWorld;

	void SetSel ( int to, int from );
	void GetSel ( int &to, int &from );
// Dialog Data
	//{{AFX_DATA(CLinkDlog)
	enum { IDD = IDD_LINK_INTERFACE };
	CComboBox	m_cToTPList;
	CComboBox	m_cToSideList;
	CComboBox	m_cFromSideList;
	CComboBox	m_cFromTPList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkDlog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinkDlog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKDLOG_H__3A90B592_6582_4179_8D4E_683A9EC9D8B1__INCLUDED_)
