// BZEdit32Doc.h : interface of the CBZEdit32Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BZEDIT32DOC_H__9CB484A1_04F2_45FF_B2F5_3ADFC9EFBA85__INCLUDED_)
#define AFX_BZEDIT32DOC_H__9CB484A1_04F2_45FF_B2F5_3ADFC9EFBA85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "world.h"
#include "TextureMngr.h"

class CBZEdit32Doc : public CDocument
{
protected: // create from serialization only
	CBZEdit32Doc();
	DECLARE_DYNCREATE(CBZEdit32Doc)

// Attributes
public:
	CWorld				m_oWorld;
	bool				m_bOpen;
	CString				m_oFile;
	CTextureManager		m_oTexMan;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBZEdit32Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBZEdit32Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBZEdit32Doc)
	afx_msg void OnFileNew();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnEditMakeanglesdeg();
	afx_msg void OnUpdateEditMakeanglesdeg(CCmdUI* pCmdUI);
	afx_msg void OnEditMakeanglesrot();
	afx_msg void OnUpdateEditMakeanglesrot(CCmdUI* pCmdUI);
	afx_msg void OnUtiltyConvertmapfiletodeg();
	afx_msg void OnUpdateUtiltyConvertmapfiletodeg(CCmdUI* pCmdUI);
	afx_msg void OnUtiltyConvertmapfiletorad();
	afx_msg void OnUpdateUtiltyConvertmapfiletorad(CCmdUI* pCmdUI);
	afx_msg void OnUtiltyDumpmap();
	afx_msg void OnUpdateUtiltyDumpmap(CCmdUI* pCmdUI);
	afx_msg void OnUtiltyDupeandflipx();
	afx_msg void OnUpdateUtiltyDupeandflipx(CCmdUI* pCmdUI);
	afx_msg void OnUtiltyDupeandflipy();
	afx_msg void OnUpdateUtiltyDupeandflipy(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BZEDIT32DOC_H__9CB484A1_04F2_45FF_B2F5_3ADFC9EFBA85__INCLUDED_)
