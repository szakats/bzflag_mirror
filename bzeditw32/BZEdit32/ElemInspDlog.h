#if !defined(AFX_ELEMINSPDLOG_H__CFDFD102_C3AA_4AFF_BE1B_FDE7EAD06EC7__INCLUDED_)
#define AFX_ELEMINSPDLOG_H__CFDFD102_C3AA_4AFF_BE1B_FDE7EAD06EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemInspDlog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemInspDlog dialog

#include "BZEdit32Doc.h"

class CElemInspDlog : public CDialog
{
// Construction
public:
	CElemInspDlog(CWnd* pParent = NULL);   // standard constructor

	bool			m_bWinUp;
	CBZEdit32Doc	*m_pDoc;

	CView			*m_pView;

	int				m_iThisItem;
	bool			m_bItemHasData;

	void			*m_pItemInterface;

	void Update ( void );
// Dialog Data
	//{{AFX_DATA(CElemInspDlog)
	enum { IDD = IDD_ELEMENT_INSP };
	CEdit	m_oNameField;
	CStatic		m_cInsertWindow;
	CString		m_szType;
	CString		m_szName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemInspDlog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemInspDlog)
	afx_msg void OnDuplicate();
	afx_msg void OnRemoveItem();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMINSPDLOG_H__CFDFD102_C3AA_4AFF_BE1B_FDE7EAD06EC7__INCLUDED_)
