#if !defined(AFX_DUPEANDDLOG_H__468CCAB3_88B7_4C30_A610_323834E0A345__INCLUDED_)
#define AFX_DUPEANDDLOG_H__468CCAB3_88B7_4C30_A610_323834E0A345__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DupeAndDlog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDupeAndDlog dialog

class CDupeAndDlog : public CDialog
{
// Construction
public:
	CDupeAndDlog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDupeAndDlog)
	enum { IDD = IDD_DUPE_AND };
	CButton	m_oTransFrame;
	CButton	m_oScaleFrame;
	CButton	m_oRotFrame;
	int		m_iNumDupes;
	BOOL	m_bRotate;
	BOOL	m_bScale;
	BOOL	m_bTransWithRot;
	BOOL	m_bTranslate;
	float	m_fXScale;
	float	m_fXTrans;
	float	m_fYScale;
	float	m_fYTrans;
	float	m_fZRot;
	float	m_fZScale;
	float	m_fZTrans;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDupeAndDlog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDupeAndDlog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTranslate();
	afx_msg void OnScale();
	afx_msg void OnRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUPEANDDLOG_H__468CCAB3_88B7_4C30_A610_323834E0A345__INCLUDED_)
