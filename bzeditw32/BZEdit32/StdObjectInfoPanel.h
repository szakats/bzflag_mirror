#if !defined(AFX_STDOBJECTINFOPANEL_H__1994C7A7_0C03_42DE_A7A1_9800767F0AE9__INCLUDED_)
#define AFX_STDOBJECTINFOPANEL_H__1994C7A7_0C03_42DE_A7A1_9800767F0AE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StdObjectInfoPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStdObjectInfoPanel dialog

class CStdObjectInfoPanel : public CDialog
{
// Construction
public:
	CStdObjectInfoPanel(CWnd* pParent = NULL);   // standard constructor

	bool m_bShowBorder;
	bool m_bShowTeam;
	
// Dialog Data
	//{{AFX_DATA(CStdObjectInfoPanel)
	enum { IDD = IDD_STD_INFO_DLOG };
	CButton	m_oDriveThruCB;
	CButton	m_oShootThruCB;
	CButton	m_oFlipZCB;
	CComboBox	m_cTeamList;
	CStatic	m_cTeamLabel;
	CStatic	m_oBorderLabel;
	CEdit	m_oBorderTE;
	float	m_fXPos;
	float	m_fYPos;
	float	m_fXScale;
	float	m_fYScale;
	float	m_fZPos;
	float	m_fZRot;
	float	m_fZScale;
	float	m_fBorderValue;
	BOOL	m_bFlipZ;
	BOOL	m_bShootThru;
	BOOL	m_bDriveThru;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStdObjectInfoPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStdObjectInfoPanel)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDOBJECTINFOPANEL_H__1994C7A7_0C03_42DE_A7A1_9800767F0AE9__INCLUDED_)
