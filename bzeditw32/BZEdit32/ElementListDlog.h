#if !defined(AFX_ELEMENTLISTDLOG_H__4F78501A_F044_40AD_8428_6AB76B94A59F__INCLUDED_)
#define AFX_ELEMENTLISTDLOG_H__4F78501A_F044_40AD_8428_6AB76B94A59F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElementListDlog.h : header file
//

#include "BZEdit32Doc.h"
#include "ElemInspDlog.h"

/////////////////////////////////////////////////////////////////////////////
// CElementListDlog dialog

class CElementListDlog : public CDialog
{
// Construction
public:
	CElementListDlog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CElementListDlog)
	enum { IDD = IDD_ELEMENT_LIST_DLOG };
	CButton		m_oNewITemButton;
	CButton		m_cHideGroupItem;
	CButton		m_cNewGroupButton;
	CListBox	m_cGroupList;
	CButton		m_cSelectButton;
	CStatic		m_cGroupLabel;
	CButton		m_cAddButton;
	CListCtrl	m_oListCtl;
	CComboBox	m_oListFilter;
	//}}AFX_DATA

	bool		m_bInited;
	CImageList	oImgeList;
	HICON		m_hDefaultIcon;
	bool			m_bWinUp;
	CBZEdit32Doc	*m_pDoc;

	CElemInspDlog	*m_pInspector;

	CView			*m_pView;

	void Update ( void );

	void ListSelChange ( void );

	void SelectItem ( int iItem );

// Helper functions for setting/getting selection mark of the list control

	int  GetSelectionIndex ( void );
	
	void SetSelectionIndex ( int iItem );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementListDlog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElementListDlog)
	afx_msg void OnSelchangeElementFilter();
	afx_msg void OnClickElementListCtl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkElementListCtl(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnItemchangedElementListCtl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownElementListCtl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddSelToGroup();
	afx_msg void OnSelectGroup();
	afx_msg void OnHideGroup();
	afx_msg void OnNewGroup();
	afx_msg void OnSelchangeGroupList();
	afx_msg void OnAdd();
	afx_msg void OnOdstatechangedElementListCtl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTLISTDLOG_H__4F78501A_F044_40AD_8428_6AB76B94A59F__INCLUDED_)
