// BZEdit32View.h : interface of the CBZEdit32View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BZEDIT32VIEW_H__27A77FC2_910A_4937_AB88_7E9B030B14F0__INCLUDED_)
#define AFX_BZEDIT32VIEW_H__27A77FC2_910A_4937_AB88_7E9B030B14F0__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pocketEngine.h"
#include "ElementListDlog.h"
#include "ElemInspDlog.h"

typedef enum
{
	eSelectMode,
	eHVMoveMode,
	eHDMoveMode,
	eRotateMode,
	eScaleMode
}trMouseMode;


class CBZEdit32View : public CView
{
protected: // create from serialization only
	CBZEdit32View();
	DECLARE_DYNCREATE(CBZEdit32View)

	CPocketEngine	m_oEngine;
// Attributes
public:
	CBZEdit32Doc* GetDocument();

	CPoint				m_rLastPoint;
	bool				m_bSkipNextSelect;

	CElementListDlog	m_oElemListDlog;
	CElemInspDlog		m_oElementInspectorDlog;

	trMouseMode			m_eMouseMode;

	bool				m_bDrawGrid;
// Operations
public:
	void DrawNiceGrid ( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBZEdit32View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBZEdit32View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CBZEdit32View)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewElementlist();
	afx_msg void OnUpdateViewElementlist(CCmdUI* pCmdUI);
	afx_msg void OnCenterView();
	afx_msg void OnIsoView();
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnXMinusView();
	afx_msg void OnXPlusView();
	afx_msg void OnYMinusView();
	afx_msg void OnYPlusView();
	afx_msg void OnZPlusView();
	afx_msg void OnViewElementinspector();
	afx_msg void OnUpdateViewElementinspector(CCmdUI* pCmdUI);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewMovetoselection();
	afx_msg void OnUpdateViewMovetoselection(CCmdUI* pCmdUI);
	afx_msg void OnMakeTransOnSlect();
	afx_msg void OnUpdateMakeTransOnSlect(CCmdUI* pCmdUI);
	afx_msg void OnUtiltyDuplicate();
	afx_msg void OnUpdateUtiltyDuplicate(CCmdUI* pCmdUI);
	afx_msg void OnUtiltyDuplicateand();
	afx_msg void OnUpdateUtiltyDuplicateand(CCmdUI* pCmdUI);
	afx_msg void OnHdPan();
	afx_msg void OnUpdateHdPan(CCmdUI* pCmdUI);
	afx_msg void OnHvMove();
	afx_msg void OnUpdateHvMove(CCmdUI* pCmdUI);
	afx_msg void OnRotoate();
	afx_msg void OnUpdateRotoate(CCmdUI* pCmdUI);
	afx_msg void OnScale();
	afx_msg void OnUpdateScale(CCmdUI* pCmdUI);
	afx_msg void OnSelectMode();
	afx_msg void OnUpdateSelectMode(CCmdUI* pCmdUI);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowAll();
	afx_msg void OnUpdateShowAll(CCmdUI* pCmdUI);
	afx_msg void OnBoxButon();
	afx_msg void OnUpdateBoxButon(CCmdUI* pCmdUI);
	afx_msg void OnPyrButon();
	afx_msg void OnUpdatePyrButon(CCmdUI* pCmdUI);
	afx_msg void OnTpButton();
	afx_msg void OnUpdateTpButton(CCmdUI* pCmdUI);
	afx_msg void OnLinkButton();
	afx_msg void OnUpdateLinkButton(CCmdUI* pCmdUI);
	afx_msg void OnBaseButton();
	afx_msg void OnUpdateBaseButton(CCmdUI* pCmdUI);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BZEdit32View.cpp
inline CBZEdit32Doc* CBZEdit32View::GetDocument()
   { return (CBZEdit32Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BZEDIT32VIEW_H__27A77FC2_910A_4937_AB88_7E9B030B14F0__INCLUDED_)
