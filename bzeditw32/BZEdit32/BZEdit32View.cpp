// BZEdit32View.cpp : implementation of the CBZEdit32View class
//

#include "stdafx.h"
#include "BZEdit32.h"

#include "BZEdit32Doc.h"
#include "BZEdit32View.h"
#include "Objects.h"
#include "DupeAndDlog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32View

IMPLEMENT_DYNCREATE(CBZEdit32View, CView)

BEGIN_MESSAGE_MAP(CBZEdit32View, CView)
	//{{AFX_MSG_MAP(CBZEdit32View)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_ELEMENTLIST, OnViewElementlist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ELEMENTLIST, OnUpdateViewElementlist)
	ON_COMMAND(ID_CENTER_VIEW, OnCenterView)
	ON_COMMAND(ID_ISO_VIEW, OnIsoView)
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_COMMAND(ID_X_MINUS_VIEW, OnXMinusView)
	ON_COMMAND(ID_X_PLUS_VIEW, OnXPlusView)
	ON_COMMAND(ID_Y_MINUS_VIEW, OnYMinusView)
	ON_COMMAND(ID_Y_PLUS_VIEW, OnYPlusView)
	ON_COMMAND(ID_Z_PLUS_VIEW, OnZPlusView)
	ON_COMMAND(ID_VIEW_ELEMENTINSPECTOR, OnViewElementinspector)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ELEMENTINSPECTOR, OnUpdateViewElementinspector)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_MOVETOSELECTION, OnViewMovetoselection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVETOSELECTION, OnUpdateViewMovetoselection)
	ON_COMMAND(ID_MAKE_TRANS_ON_SLECT, OnMakeTransOnSlect)
	ON_UPDATE_COMMAND_UI(ID_MAKE_TRANS_ON_SLECT, OnUpdateMakeTransOnSlect)
	ON_COMMAND(ID_UTILTY_DUPLICATE, OnUtiltyDuplicate)
	ON_UPDATE_COMMAND_UI(ID_UTILTY_DUPLICATE, OnUpdateUtiltyDuplicate)
	ON_COMMAND(ID_UTILTY_DUPLICATEAND, OnUtiltyDuplicateand)
	ON_UPDATE_COMMAND_UI(ID_UTILTY_DUPLICATEAND, OnUpdateUtiltyDuplicateand)
	ON_COMMAND(ID_HD_PAN, OnHdPan)
	ON_UPDATE_COMMAND_UI(ID_HD_PAN, OnUpdateHdPan)
	ON_COMMAND(ID_HV_MOVE, OnHvMove)
	ON_UPDATE_COMMAND_UI(ID_HV_MOVE, OnUpdateHvMove)
	ON_COMMAND(ID_ROTOATE, OnRotoate)
	ON_UPDATE_COMMAND_UI(ID_ROTOATE, OnUpdateRotoate)
	ON_COMMAND(ID_SCALE, OnScale)
	ON_UPDATE_COMMAND_UI(ID_SCALE, OnUpdateScale)
	ON_COMMAND(ID_SELECT_MODE, OnSelectMode)
	ON_UPDATE_COMMAND_UI(ID_SELECT_MODE, OnUpdateSelectMode)
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_SHOW_ALL, OnShowAll)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ALL, OnUpdateShowAll)
	ON_COMMAND(ID_BOX_BUTON, OnBoxButon)
	ON_UPDATE_COMMAND_UI(ID_BOX_BUTON, OnUpdateBoxButon)
	ON_COMMAND(ID_PYR_BUTON, OnPyrButon)
	ON_UPDATE_COMMAND_UI(ID_PYR_BUTON, OnUpdatePyrButon)
	ON_COMMAND(ID_TP_BUTTON, OnTpButton)
	ON_UPDATE_COMMAND_UI(ID_TP_BUTTON, OnUpdateTpButton)
	ON_COMMAND(ID_LINK_BUTTON, OnLinkButton)
	ON_UPDATE_COMMAND_UI(ID_LINK_BUTTON, OnUpdateLinkButton)
	ON_COMMAND(ID_BASE_BUTTON, OnBaseButton)
	ON_UPDATE_COMMAND_UI(ID_BASE_BUTTON, OnUpdateBaseButton)
	ON_WM_MENUSELECT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32View construction/destruction

CBZEdit32View::CBZEdit32View()
{
	// TODO: add construction code here
	m_bSkipNextSelect = false;

	m_eMouseMode = eSelectMode;
}

CBZEdit32View::~CBZEdit32View()
{
}

BOOL CBZEdit32View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32View drawing

void CBZEdit32View::DrawNiceGrid ( void )
{

	CWorld	*pWorld = &(GetDocument()->m_oWorld);

	float	fGridSpacing = 25;

	glDepthFunc(GL_LEQUAL);
//	glDepthMask(0);

	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor4f(0.65f,0.65f,0.65f,1);
	// make the grid
	for (float f = 0; f <= M_WORLDSIZE ;f+=fGridSpacing)
	{
		glVertex3f(-M_WORLDSIZE,f,0.1f);
		glVertex3f(M_WORLDSIZE,f,0.1f);

		glVertex3f(-M_WORLDSIZE,-f,0.1f);
		glVertex3f(M_WORLDSIZE,-f,0.1f);

		glVertex3f(f,M_WORLDSIZE,0.1f);
		glVertex3f(f,-M_WORLDSIZE,0.1f);

		glVertex3f(-f,M_WORLDSIZE,0.1f);
		glVertex3f(-f,-M_WORLDSIZE,0.1f);
	}
	glEnd();

	glLineWidth(4.0f);
	glBegin(GL_LINES);

	glColor4f(0.85f,0.25f,0.25f,1.0f);
	glVertex3f(fGridSpacing,0,0.1f);
	glVertex3f(-fGridSpacing,0,0.1f);
	
	glColor4f(0.25f,0.25f,0.85f,1.0f);
	glVertex3f(0,fGridSpacing,0.1f);
	glVertex3f(0,-fGridSpacing,0.1f);

	glColor4f(0.25f,0.85f,0.25f,1.0f);
	glVertex3f(0,0,fGridSpacing);
	glVertex3f(0,0,0);
	glEnd();


	glLineWidth(2.0f);
	// X +
	glBegin(GL_LINES);
	glColor4f(1,1,1,1.0f);
		glVertex3f(M_WORLDSIZE,-10,30);
		glVertex3f(M_WORLDSIZE,10,60);

		glVertex3f(M_WORLDSIZE,10,30);
		glVertex3f(M_WORLDSIZE,-10,60);

		glVertex3f(M_WORLDSIZE,-12,45);
		glVertex3f(M_WORLDSIZE,-22,45);

		glVertex3f(M_WORLDSIZE,-17,40);
		glVertex3f(M_WORLDSIZE,-17,50);
	glEnd();

	// x-
	glBegin(GL_LINES);
	glColor4f(1,1,1,1.0f);
		glVertex3f(-M_WORLDSIZE,-10,30);
		glVertex3f(-M_WORLDSIZE,10,60);

		glVertex3f(-M_WORLDSIZE,10,30);
		glVertex3f(-M_WORLDSIZE,-10,60);

		glVertex3f(-M_WORLDSIZE,12,45);
		glVertex3f(-M_WORLDSIZE,22,45);
	glEnd();


	// y+
	glBegin(GL_LINES);
	glColor4f(1,1,1,1.0f);

		glVertex3f(0,M_WORLDSIZE,30);
		glVertex3f(0,M_WORLDSIZE,45);

		glVertex3f(0,M_WORLDSIZE,45);
		glVertex3f(-10,M_WORLDSIZE,60);

		glVertex3f(0,M_WORLDSIZE,45);
		glVertex3f(10,M_WORLDSIZE,60);

		glVertex3f(12,M_WORLDSIZE,45);
		glVertex3f(22,M_WORLDSIZE,45);

		glVertex3f(17,M_WORLDSIZE,40);
		glVertex3f(17,M_WORLDSIZE,50);
	glEnd();

		// y+
	glBegin(GL_LINES);
	glColor4f(1,1,1,1.0f);

		glVertex3f(0,-M_WORLDSIZE,30);
		glVertex3f(0,-M_WORLDSIZE,45);

		glVertex3f(0,-M_WORLDSIZE,45);
		glVertex3f(-10,-M_WORLDSIZE,60);

		glVertex3f(0,-M_WORLDSIZE,45);
		glVertex3f(10,-M_WORLDSIZE,60);

		glVertex3f(-12,-M_WORLDSIZE,45);
		glVertex3f(-22,-M_WORLDSIZE,45);

	glEnd();

//	glDepthMask(1);

	glDepthFunc(GL_LEQUAL);
	glLineWidth(1.0f);
}

void CBZEdit32View::OnDraw(CDC* pDC)
{
	CBZEdit32Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_oEngine.BeginDraw())
	{
		m_oEngine.SetLight();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		if (m_bDrawGrid)
		{
			DrawNiceGrid();
		}

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

		if (GetDocument()->m_oWorld.Inited())
			GetDocument()->m_oWorld.Render();
		
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		if (m_bDrawGrid)
		{
			DrawNiceGrid();
		}

		m_oEngine.EndDraw();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32View printing

BOOL CBZEdit32View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBZEdit32View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBZEdit32View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32View diagnostics

#ifdef _DEBUG
void CBZEdit32View::AssertValid() const
{
	CView::AssertValid();
}

void CBZEdit32View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBZEdit32Doc* CBZEdit32View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBZEdit32Doc)));
	return (CBZEdit32Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBZEdit32View message handlers

BOOL CBZEdit32View::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

int CBZEdit32View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_oEngine.Create(10,10,lpCreateStruct->hInstance,GetSafeHwnd());
	
	m_oEngine.SetViewType(eQuestType);
	m_oEngine.SetPullBack(512.0f);
	m_oEngine.ShowGrid(false);
	m_oEngine.RotateView(45.0f,0,0);
	m_oEngine.PanView(0,0,0);
	m_oEngine.SetBGColor(0.0f,0.0f,0.0f);

	float lightPos[3];
	lightPos[0] = 250.0f;
	lightPos[1] = 100.0f;
	lightPos[2] = 500.0f;

	RegisterClasses(&(GetDocument()->m_oWorld));

	m_oEngine.SetLightInfo(eLightPos,lightPos);

	m_oElementInspectorDlog.m_pDoc = GetDocument();
	m_oElementInspectorDlog.m_pView = this;
	m_oElementInspectorDlog.Create(IDD_ELEMENT_INSP,this);

	m_oElemListDlog.m_pDoc = GetDocument();
	m_oElemListDlog.m_pView = this;
	m_oElemListDlog.m_pInspector = &m_oElementInspectorDlog;
	m_oElemListDlog.Create(IDD_ELEMENT_LIST_DLOG,this);

	m_bDrawGrid= true;

//	LoadPlugins();
	
	return 0;	
}

void CBZEdit32View::OnDestroy() 
{	
	if (m_oElemListDlog.GetSafeHwnd()!=NULL)
		m_oElemListDlog.DestroyWindow();

	if (m_oElementInspectorDlog.GetSafeHwnd()!=NULL)
		m_oElemListDlog.DestroyWindow();

	m_oEngine.Kill();	


	CView::OnDestroy();	
}

void CBZEdit32View::OnSize(UINT nType, int cx, int cy) 
{	
	CView::OnSize(nType, cx, cy);

	int ListW,InspH;

	ListW = 200;
	InspH = 168;

	m_oElemListDlog.MoveWindow(0,0,ListW,cy,false);
	m_oElementInspectorDlog.MoveWindow(ListW,cy-InspH,cx-ListW,InspH,false);
	::MoveWindow(m_oEngine.GetWindow(),ListW,0,cx-ListW,cy-InspH,false);
	OnDraw(NULL);	
}

void CBZEdit32View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CView::OnLButtonDblClk(nFlags, point);
}

void CBZEdit32View::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CView::OnRButtonDblClk(nFlags, point);
}

void CBZEdit32View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd	*pFocusWin = GetFocus();

	if (pFocusWin != this)
	{
		m_bSkipNextSelect = false;
		SetFocus();
	}

	if(	m_eMouseMode != eSelectMode	)
	{
		SetFocus();
		SetCapture();
		GetCursorPos(&m_rLastPoint);
		OnDraw(NULL);	
	}

	CView::OnLButtonDown(nFlags, point);
}

void CBZEdit32View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(	m_eMouseMode == eSelectMode	)
	{
		if (m_bSkipNextSelect)
		{
			m_bSkipNextSelect = false;
			return;
		}

		int	iHit;

		if (m_oEngine.BeginSelect(point.x,point.y))
		{
			GetDocument()->m_oWorld.RenderTargets();

			iHit = m_oEngine.EndSelect();

			iHit--;
			m_oElemListDlog.SelectItem(iHit);	
		}
	}

	if(	m_eMouseMode != eSelectMode	)
	{
		ReleaseCapture();
		OnDraw(NULL);
	}


	CView::OnLButtonUp(nFlags, point);
}

void CBZEdit32View::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	SetCapture();
	GetCursorPos(&m_rLastPoint);
	OnDraw(NULL);	

	CView::OnRButtonDown(nFlags, point);
}

void CBZEdit32View::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	
	CView::OnRButtonUp(nFlags, point);
}

BOOL CBZEdit32View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{	
	m_oEngine.MovePullBack(-zDelta/15.0f);

	if (m_oEngine.GetCurrentPullBack() < 1.0)
		m_oEngine.SetPullBack(1.0);
		
	OnDraw(NULL);	

	return true;//CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CBZEdit32View::OnMouseMove(UINT nFlags, CPoint point) 
{
	GetCursorPos(&point);
	
	CPoint	delta = point - m_rLastPoint;
	m_rLastPoint = point;

	if ((nFlags & MK_RBUTTON))
	{
		if ((nFlags & MK_SHIFT) && (nFlags & MK_CONTROL))
			m_oEngine.MovePullBack(delta.y/3.0f);
		else if ((nFlags & MK_CONTROL))
			m_oEngine.PanViewInc(delta.x/2.0f,0,delta.y/2.0f);
		else if ((nFlags & MK_SHIFT))
			m_oEngine.PanViewInc(delta.x/2.0f,delta.y/2.0f,0);
		else
			m_oEngine.RotateViewInc(delta.y/5.0f,0,delta.x/5.0f);
	}

	if ((nFlags & MK_LBUTTON))
	{
		int iSel = 	GetDocument()->m_oWorld.GetSelItem();

		if (iSel >=0)
		{
			CBaseObject *pObject = GetDocument()->m_oWorld.GetObject(iSel);
			if(pObject)
			{
				switch (m_eMouseMode)
				{
					case eSelectMode:
						break;

					case eRotateMode:
						pObject->SetRotAngle(pObject->GetRotAngle()+(delta.x/2.0f));
					break;

					case eScaleMode:
						pObject->SetXScale(pObject->GetXScale()+(delta.x/2.0f));

						if (nFlags & MK_SHIFT)
							pObject->SetZScale(pObject->GetZScale()+(delta.y/2.0f));
						else
							pObject->SetYScale(pObject->GetYScale()+(delta.y/2.0f));
					break;

					case eHVMoveMode:
						pObject->SetXPos(pObject->GetXPos()+(delta.x/2.0f));
						pObject->SetYPos(pObject->GetYPos()-(delta.y/2.0f));
					break;

					case eHDMoveMode:
						pObject->SetXPos(pObject->GetXPos()+(delta.x/2.0f));
						pObject->SetZPos(pObject->GetZPos()+(delta.y/2.0f));
					break;
				}

				pObject->Init();
				GetDocument()->SetModifiedFlag(true);
				m_oElementInspectorDlog.Update();
			}
		}
	}

	OnDraw(NULL);	
}

void CBZEdit32View::OnViewElementlist() 
{
	if (m_oElemListDlog.m_bWinUp)
		m_oElemListDlog.DestroyWindow();
	else
	{
		m_oElemListDlog.m_pDoc = GetDocument();
		m_oElemListDlog.DestroyWindow();
		m_oElemListDlog.Create(IDD_ELEMENT_LIST_DLOG,this);
	}		
}

void CBZEdit32View::OnUpdateViewElementlist(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_oElemListDlog.m_bWinUp);	
}

void CBZEdit32View::OnViewElementinspector() 
{
	if (m_oElementInspectorDlog.m_bWinUp)
		m_oElementInspectorDlog.DestroyWindow();
	else
	{
		m_oElementInspectorDlog.DestroyWindow();
		m_oElementInspectorDlog.Create(IDD_ELEMENT_INSP,this);
		m_oElementInspectorDlog.m_pDoc = GetDocument();
	}		
}

void CBZEdit32View::OnUpdateViewElementinspector(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_oElementInspectorDlog.m_bWinUp);
}

void CBZEdit32View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	m_oElemListDlog.Update();
	m_oElementInspectorDlog.Update();
	OnDraw(NULL);
}

void CBZEdit32View::OnCenterView() 
{
	m_oEngine.PanView(0,0,0);
	OnDraw(NULL);
}

void CBZEdit32View::OnIsoView() 
{
	m_oEngine.RotateView(45.0f,0,0);
	OnDraw(NULL);
}

void CBZEdit32View::OnViewGrid() 
{
	m_bDrawGrid = !m_bDrawGrid;
	OnDraw(NULL);
}

void CBZEdit32View::OnUpdateViewGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bDrawGrid);
}

void CBZEdit32View::OnXMinusView() 
{
	m_oEngine.RotateView(0,0,90.0f);
	OnDraw(NULL);
}	

void CBZEdit32View::OnXPlusView() 
{
	m_oEngine.RotateView(0,0,-90.0f);
	OnDraw(NULL);
}

void CBZEdit32View::OnYMinusView() 
{
	m_oEngine.RotateView(0,0,0.0f);
	OnDraw(NULL);
}

void CBZEdit32View::OnYPlusView() 
{
	m_oEngine.RotateView(0,0,180.0f);
	OnDraw(NULL);
}

void CBZEdit32View::OnZPlusView() 
{
	m_oEngine.RotateView(90.0f,0,0.0f);
	OnDraw(NULL);
}

void CBZEdit32View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CBZEdit32View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	switch(nChar)
	{
		case VK_DELETE:
		case VK_BACK:
			if (GetDocument()->m_oWorld.Inited() && (GetDocument()->m_oWorld.GetSelItem() != -1))
			{
				GetDocument()->m_oWorld.RemoveObject(GetDocument()->m_oWorld.GetSelItem());
				GetDocument()->m_oWorld.SetSelItem(1);

				GetDocument()->UpdateAllViews(NULL);
				GetDocument()->SetModifiedFlag(true);
			}
			break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBZEdit32View::OnViewMovetoselection() 
{
	CBaseObject	*pObject = GetDocument()->m_oWorld.GetObject(GetDocument()->m_oWorld.GetSelItem());

	m_oEngine.PanView(pObject->GetXPos(),-pObject->GetYPos(),pObject->GetZPos());
}

void CBZEdit32View::OnUpdateViewMovetoselection(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetSelItem()!= -1);
}

void CBZEdit32View::OnMakeTransOnSlect() 
{
	GetDocument()->m_oWorld.SetFadeNonSel(!GetDocument()->m_oWorld.GetFadeNonSel());
	OnDraw(NULL);
}

void CBZEdit32View::OnUpdateMakeTransOnSlect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_oWorld.GetFadeNonSel());
}

void CBZEdit32View::OnUtiltyDuplicate() 
{
	if (GetDocument()->m_oWorld.GetSelItem() != -1)
	{
		CBaseObject *pObject = GetDocument()->m_oWorld.GetObject(GetDocument()->m_oWorld.GetSelItem());
		if (!pObject)
			return;

		int iCopy = GetDocument()->m_oWorld.AddObject(pObject->GetTypeName());
		CBaseObject *pCopy = GetDocument()->m_oWorld.GetObject(iCopy);
		
		pObject = GetDocument()->m_oWorld.GetObject(GetDocument()->m_oWorld.GetSelItem());

		if (!pCopy)
			return;

		pCopy->CopyFrom(pObject);

		CString	temp = pObject->GetItemName();

		char	*pCopyChar = strstr(temp.GetBuffer(temp.GetLength()),"copy");

		if (pCopyChar)
		{
			pCopyChar += 5;
			int iNum = 0;

			iNum = atoi(pCopyChar);
			iNum++;

			*pCopyChar = NULL;

			temp.ReleaseBuffer();

			CString	temp2;

			temp2.Format("%d",iNum);

			temp += temp2;
		}
		else
		{
			temp.ReleaseBuffer();
			temp += " copy";
		}

		pCopy->SetItemName((const char*)temp);

		GetDocument()->m_oWorld.SetSelItem(pCopy->GetID());

		m_oElemListDlog.Update();
		m_oElemListDlog.SelectItem(pCopy->GetID());
		m_oElementInspectorDlog.Update();

		GetDocument()->UpdateAllViews(NULL);
		GetDocument()->SetModifiedFlag(true);
	}
}

void CBZEdit32View::OnUpdateUtiltyDuplicate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetSelItem() != -1);	
}

void CBZEdit32View::OnUtiltyDuplicateand() 
{
	CDupeAndDlog	oDlog(this);

	oDlog.m_bRotate = false;
	oDlog.m_bScale = false;
	oDlog.m_bTranslate = false;
	oDlog.m_bTransWithRot = false;
	oDlog.m_fXScale = 0.0f;
	oDlog.m_fYScale = 0.0f;
	oDlog.m_fZScale = 0.0f;
	oDlog.m_fXTrans = 0.0f;
	oDlog.m_fYTrans = 0.0f;
	oDlog.m_fZTrans = 0.0f;
	oDlog.m_fZRot = 0.0f;
	oDlog.m_iNumDupes = 0;

	if (oDlog.DoModal()==IDOK)
	{
		if (oDlog.m_iNumDupes == 0)
			return;

		if (!oDlog.m_bRotate && !oDlog.m_bTranslate &&!oDlog.m_bScale )
			return;

		trVertex3D rTransDelta, rTrans;
		rTransDelta.x = rTransDelta.y = rTransDelta.z = 0;

		trVertex3D rScaleDelta;
		rScaleDelta.x = rScaleDelta.y = rScaleDelta.z = 0;

		float		fRot = 0, fRotDelta = 0;

		if (oDlog.m_bRotate)
			fRot = fRotDelta = oDlog.m_fZRot;

		if (oDlog.m_bTranslate)
		{
			rTransDelta.x = oDlog.m_fXTrans;
			rTransDelta.y = oDlog.m_fYTrans;
			rTransDelta.z = oDlog.m_fZTrans;
		}

		if (oDlog.m_bScale)
		{
			rScaleDelta.x = oDlog.m_fXScale;
			rScaleDelta.y = oDlog.m_fYScale;
			rScaleDelta.z = oDlog.m_fZScale;
		}

		rTrans = rTransDelta;
		trVertex3D	rTempVec = rTransDelta;
		trMatrix	rMatrix;

		if (oDlog.m_bTransWithRot)
		{
			RotationMatrix(&rMatrix,sin(fRotDelta*M_DEG2RAD),cos(fRotDelta*M_DEG2RAD));
			
			TransformPos2d(&rTempVec,&rMatrix);
			rTrans = rTempVec;
		}

		CWorld	*pWorld = &GetDocument()->m_oWorld;

		CBaseObject	*pThisObject =	pWorld->GetObject(pWorld->GetSelItem());
		CBaseObject *pDupeObject;

		if (!pThisObject)
		{
			MessageBox("Bad object selection");
			return;
		}

		CString	szName = pThisObject->GetItemName();

		if (szName.IsEmpty())
			szName = pThisObject->GetTypeName();

		CString	szSubName;
		CString	szItemName;

		pDupeObject = pThisObject;
		for (int iDupe = 0; iDupe < oDlog.m_iNumDupes; iDupe++)
		{
			pDupeObject	 = pWorld->GetObject(pWorld->DupeObject(pDupeObject));
			if (!pDupeObject)
			{
				MessageBox("Bad object creation");
				return;
			}
			
			szSubName.Format(" Dupe %d",iDupe);

			szItemName = szName + szSubName;

			pDupeObject->SetItemName(szItemName);

			pDupeObject->SetPos(pThisObject->GetPos()+rTrans);
			pDupeObject->SetScale(pDupeObject->GetScale()+rScaleDelta);
			pDupeObject->SetRotAngle(pDupeObject->GetRotAngle()+fRotDelta);
			pDupeObject->Init();

			fRot += fRotDelta;

			if (oDlog.m_bTransWithRot)
			{
				RotationMatrix(&rMatrix,sin(fRot*M_DEG2RAD),cos(fRot*M_DEG2RAD));			
				rTempVec = rTransDelta;

				TransformPos2d(&rTempVec,&rMatrix);
				rTrans +=rTempVec;
			}
			else
				rTrans += rTransDelta;
		}

		m_oElemListDlog.Update();
		m_oElemListDlog.SelectItem(pDupeObject->GetID());
		m_oElementInspectorDlog.Update();

		GetDocument()->SetModifiedFlag(true);
		OnDraw(NULL);
	}
}

void CBZEdit32View::OnUpdateUtiltyDuplicateand(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetSelItem() != -1);	
}

void CBZEdit32View::OnHdPan() 
{
	m_eMouseMode = eHDMoveMode;
}

void CBZEdit32View::OnUpdateHdPan(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_eMouseMode==eHDMoveMode);
}

void CBZEdit32View::OnHvMove() 
{
	m_eMouseMode = eHVMoveMode;
}

void CBZEdit32View::OnUpdateHvMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_eMouseMode==eHVMoveMode);
}

void CBZEdit32View::OnRotoate() 
{
	m_eMouseMode = eRotateMode;
}

void CBZEdit32View::OnUpdateRotoate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_eMouseMode==eRotateMode);
}

void CBZEdit32View::OnScale() 
{
	m_eMouseMode = eScaleMode;
}

void CBZEdit32View::OnUpdateScale(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_eMouseMode==eScaleMode);
}

void CBZEdit32View::OnSelectMode() 
{
	m_eMouseMode = eSelectMode;
}

void CBZEdit32View::OnUpdateSelectMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_eMouseMode==eSelectMode);
}

void CBZEdit32View::OnMButtonUp(UINT nFlags, CPoint point) 
{
	if (GetDocument()->m_oWorld.GetSelItem() <0)
		return;
	
	CBaseObject	*pObject = GetDocument()->m_oWorld.GetObject(GetDocument()->m_oWorld.GetSelItem());

	m_oEngine.PanView(pObject->GetXPos(),-pObject->GetYPos(),pObject->GetXPos());
	
	CView::OnMButtonUp(nFlags, point);
}

void CBZEdit32View::OnShowAll() 
{
	GetDocument()->m_oWorld.SetDrawAll(!GetDocument()->m_oWorld.GetDrawAll());
	OnDraw(NULL);
}

void CBZEdit32View::OnUpdateShowAll(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_oWorld.GetDrawAll());
	
}

void CBZEdit32View::OnBoxButon() 
{
	int ID = GetDocument()->m_oWorld.AddObject("Box");
	if (ID != -1)
	{
		m_oElemListDlog.Update();
		m_oElemListDlog.SelectItem(ID);
		m_oElementInspectorDlog.Update();
		OnDraw(NULL);
		GetDocument()->SetModifiedFlag(true);
	}
}

void CBZEdit32View::OnUpdateBoxButon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetFactory("Box") != NULL);	
}

void CBZEdit32View::OnPyrButon() 
{
	int ID = GetDocument()->m_oWorld.AddObject("Pyramid");
	if (ID != -1)
	{
		m_oElemListDlog.Update();
		m_oElemListDlog.SelectItem(ID);
		m_oElementInspectorDlog.Update();
		OnDraw(NULL);
		GetDocument()->SetModifiedFlag(true);
	}
}

void CBZEdit32View::OnUpdatePyrButon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetFactory("Pyramid") != NULL);	
	
}

void CBZEdit32View::OnTpButton() 
{
	int ID = GetDocument()->m_oWorld.AddObject("Teleporter");
	if (ID != -1)
	{
		m_oElemListDlog.Update();
		m_oElemListDlog.SelectItem(ID);
		m_oElementInspectorDlog.Update();
		OnDraw(NULL);
		GetDocument()->SetModifiedFlag(true);
	}
}

void CBZEdit32View::OnUpdateTpButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetFactory("Teleporter") != NULL);	
	
}

void CBZEdit32View::OnLinkButton() 
{
	int ID = GetDocument()->m_oWorld.AddObject("Link");
	if (ID != -1)
	{
		m_oElemListDlog.Update();
		m_oElemListDlog.SelectItem(ID);
		m_oElementInspectorDlog.Update();
		OnDraw(NULL);
		GetDocument()->SetModifiedFlag(true);
	}
}

void CBZEdit32View::OnUpdateLinkButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetFactory("Link") != NULL);		
}

void CBZEdit32View::OnBaseButton() 
{
	int ID = GetDocument()->m_oWorld.AddObject("Base");
	if (ID != -1)
	{
		m_oElemListDlog.Update();
		m_oElemListDlog.SelectItem(ID);
		m_oElementInspectorDlog.Update();
		OnDraw(NULL);
		GetDocument()->SetModifiedFlag(true);
	}
}

void CBZEdit32View::OnUpdateBaseButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_oWorld.GetFactory("Base") != NULL);	
}

BOOL CBZEdit32View::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CBZEdit32View::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::OnCommand(wParam, lParam);
}

void CBZEdit32View::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
	CView::OnMenuSelect(nItemID, nFlags, hSysMenu);
	
	// TODO: Add your message handler code here
	
}
