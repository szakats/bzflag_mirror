// ElementListDlog.cpp : implementation file
//

#include "stdafx.h"
#include "BZEdit32.h"
#include "ElementListDlog.h"

#include <VECTOR>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElementListDlog dialog


CElementListDlog::CElementListDlog(CWnd* pParent /*=NULL*/)
	: CDialog(CElementListDlog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElementListDlog)
	//}}AFX_DATA_INIT

	m_bWinUp = false;
	m_pDoc = NULL;
	m_pInspector = NULL;
	m_pView = NULL;
	m_bInited = false;
}


void CElementListDlog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElementListDlog)
	DDX_Control(pDX, IDC_ADD, m_oNewITemButton);
	DDX_Control(pDX, IDC_HIDE_GROUP, m_cHideGroupItem);
	DDX_Control(pDX, IDC_NEW_GROUP, m_cNewGroupButton);
	DDX_Control(pDX, IDC_GROUP_LIST, m_cGroupList);
	DDX_Control(pDX, IDC_SELECT_GROUP, m_cSelectButton);
	DDX_Control(pDX, IDC_GROUPS_LABEL, m_cGroupLabel);
	DDX_Control(pDX, IDC_ADD_SEL_TO_GROUP, m_cAddButton);
	DDX_Control(pDX, IDC_ELEMENT_LIST_CTL, m_oListCtl);
	DDX_Control(pDX, IDC_ELEMENT_FILTER, m_oListFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElementListDlog, CDialog)
	//{{AFX_MSG_MAP(CElementListDlog)
	ON_CBN_SELCHANGE(IDC_ELEMENT_FILTER, OnSelchangeElementFilter)
	ON_NOTIFY(NM_CLICK, IDC_ELEMENT_LIST_CTL, OnClickElementListCtl)
	ON_NOTIFY(NM_DBLCLK, IDC_ELEMENT_LIST_CTL, OnDblclkElementListCtl)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ELEMENT_LIST_CTL, OnItemchangedElementListCtl)
	ON_NOTIFY(LVN_KEYDOWN, IDC_ELEMENT_LIST_CTL, OnKeydownElementListCtl)
	ON_BN_CLICKED(IDC_ADD_SEL_TO_GROUP, OnAddSelToGroup)
	ON_BN_CLICKED(IDC_SELECT_GROUP, OnSelectGroup)
	ON_BN_CLICKED(IDC_HIDE_GROUP, OnHideGroup)
	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroup)
	ON_LBN_SELCHANGE(IDC_GROUP_LIST, OnSelchangeGroupList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_ELEMENT_LIST_CTL, OnOdstatechangedElementListCtl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementListDlog message handlers

void CElementListDlog::Update ( void )
{
	if (m_bWinUp)
		OnSelchangeElementFilter();
}

void CElementListDlog::SelectItem(int iItem )
{
	if (!m_pDoc)
		return;

	if (iItem ==-1)
	{
		m_pDoc->m_oWorld.SetSelItem(iItem);

		m_oListCtl.SetItemState(m_oListCtl.GetSelectionMark(),0,LVIS_SELECTED);
		m_oListCtl.SetSelectionMark(-1);

		ListSelChange();

		return;
	}
	m_pDoc->m_oWorld.SetSelItem(iItem);

	CBaseObject	*pObject = m_pDoc->m_oWorld.GetObject(iItem);

	if (!pObject)
		return;

	int	iCurSel = m_oListFilter.GetCurSel();

	if ( iCurSel != 0 )
	{
		CString		sThisType = m_pDoc->m_oWorld.GetFactoryName(m_oListFilter.GetItemData(iCurSel));
		CString		sObjType = pObject->GetTypeName();

		int iFactoryID = m_oListFilter.GetItemData(iCurSel);
		int iTypeID =  pObject->GetTypeID();

		if (iFactoryID != iTypeID)
		{
			int iThisTypeItem = 0;
			for ( int i = 0; i < m_oListFilter.GetCount();i++)
			{
				int iData = m_oListFilter.GetItemData(i);
				if (iData == iTypeID)
					iThisTypeItem = i;
			}
			m_oListFilter.SetCurSel(iThisTypeItem);
			OnSelchangeElementFilter();
		}
	}

	int iCount = m_oListCtl.GetItemCount();

	int iSelItem = -1;
	for (int i = 0 ;i < iCount; i++)
	{
		if (m_oListCtl.GetItemData(i) == iItem)
			iSelItem = i;

		m_oListCtl.SetItemState(i,0,LVIS_SELECTED);
	}
	
	if (iSelItem != -1)
	{
		m_oListCtl.SetSelectionMark(iSelItem);
		m_oListCtl.SetItemState(iSelItem,0xffff,LVIS_SELECTED);
	}

	ListSelChange();
	m_oListCtl.Invalidate(true);
}

void CElementListDlog::OnSelchangeElementFilter() 
{
	if (!m_pDoc)
		return;

	int	iCurSel = m_oListFilter.GetCurSel();

	int	iListSel = m_oListCtl.GetSelectionMark();
	m_oListCtl.DeleteAllItems();

	CString	ListType;
	CString	ObjectType;

	int iListType;
	int	iObjectType;

	if (iCurSel != 0)
		iListType= m_oListFilter.GetItemData(iCurSel);

	m_oNewITemButton.EnableWindow(iCurSel != 0);
	
	if (m_pDoc && (m_pDoc->m_oWorld.Size()>0) )
	{
		int		iCount = 0;
		bool	bAd;

		for (int i = 0; i < m_pDoc->m_oWorld.Size();i++)
		{
			CBaseObject	*pObject = m_pDoc->m_oWorld.GetObject(i);
			if (pObject)
			{
				iObjectType = pObject->GetTypeID();
				bAd = false;

				if (iCurSel == 0)
					bAd = true;
				else if (iObjectType == iListType)
					bAd = true;

				if (bAd)
				{
					m_oListCtl.InsertItem(iCount,pObject->GetListName(),iObjectType);
					m_oListCtl.SetItemData(iCount,i);
					iCount++;
				}
			}
		}
	}

	if (iListSel >=m_oListCtl.GetItemCount())
		iListSel = m_oListCtl.GetItemCount()-1;

	if (iListSel !=-1)
	{
		m_oListCtl.SetItemState(iListSel,0xffff,LVIS_SELECTED);
		m_oListCtl.SetSelectionMark(iListSel);

		m_pDoc->m_oWorld.SetSelItem(m_oListCtl.GetItemData(iListSel));
	}
	else
		m_pDoc->m_oWorld.SetSelItem(-1);

	if (m_pInspector)
		m_pInspector->Update();
}

void CElementListDlog::ListSelChange ( void )
{
	int	iSel = m_oListCtl.GetSelectionMark();
	
	if (iSel != -1)
		m_pDoc->m_oWorld.SetSelItem(m_oListCtl.GetItemData(iSel));
	else
		m_pDoc->m_oWorld.SetSelItem(-1);

	if (m_pInspector)
		m_pInspector->Update();

	if (m_pView)
		m_pView->Invalidate();
}

void CElementListDlog::OnClickElementListCtl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ListSelChange();
	*pResult = 0;
}

void CElementListDlog::OnDblclkElementListCtl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ListSelChange();
	*pResult = 0;
}

BOOL CElementListDlog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_pDoc)
	{
		m_oListFilter.ResetContent();
		m_oListFilter.SetItemData(m_oListFilter.AddString("All"),-1);

		m_hDefaultIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_BOXWALL_ICON));
		HICON	hIcon;
		CBaseObject	*pTempObject;
		
		oImgeList.DeleteImageList();
		oImgeList.Create(16,16,ILC_COLOR4,5,m_pDoc->m_oWorld.GetFactoryCount()+2);

		for (int i = 0; i < m_pDoc->m_oWorld.GetFactoryCount(); i++)
		{
			pTempObject = m_pDoc->m_oWorld.GetFactory(i)->New();

			hIcon = NULL;
			if (pTempObject && pTempObject->ShowInInterface())
			{
				hIcon = (HICON)pTempObject->GetIcon();

				int iItem = m_oListFilter.AddString(m_pDoc->m_oWorld.GetFactoryName(i));
				m_oListFilter.SetItemData(iItem,i);

			}
			if (!hIcon)
				hIcon = m_hDefaultIcon;

			oImgeList.Add(hIcon);
			
			m_pDoc->m_oWorld.GetFactory(i)->Delete(pTempObject);
		}

		m_oListFilter.SetCurSel(0);
		m_oListCtl.SetImageList(&oImgeList,LVSIL_SMALL);

		OnSelchangeElementFilter();

		SetDefID(IDC_ELEMENT_FILTER);
	}

	m_bWinUp = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElementListDlog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	oImgeList.DeleteImageList();

	m_bWinUp = false;	
}

void CElementListDlog::OnClose() 
{
	m_bWinUp = false;
	oImgeList.DeleteImageList();
	CDialog::OnClose();
}

void CElementListDlog::OnSize(UINT nType, int cx, int cy) 
{
	int	newCX = cx,newCY = cy;

	if (cx < 120)
		newCX = 120;

	if (cy < 400)
		newCY = 400;

	int	iGroupH = 200;

	if (m_bWinUp)
	{
		m_oListCtl.MoveWindow(10,50,newCX-20,cy-60-iGroupH);

		m_cGroupList.SetWindowPos(NULL,10,cy-iGroupH+16,0,0,SWP_NOZORDER|SWP_NOSIZE);
		m_cGroupLabel.SetWindowPos(NULL,10,cy-iGroupH,0,0,SWP_NOZORDER|SWP_NOSIZE);

		m_cNewGroupButton.SetWindowPos(NULL,10,cy-iGroupH+135,0,0,SWP_NOZORDER|SWP_NOSIZE);
		m_cHideGroupItem.SetWindowPos(NULL,10+80,cy-iGroupH+135,0,0,SWP_NOZORDER|SWP_NOSIZE);

		m_cAddButton.SetWindowPos(NULL,10,cy-iGroupH+165,0,0,SWP_NOZORDER|SWP_NOSIZE);
		m_cSelectButton.SetWindowPos(NULL,10+80,cy-iGroupH+165,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}
		
	CDialog::OnSize(nType, newCX, newCY);
}

void CElementListDlog::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CRect	NewR = pRect;

	if ((NewR.right-NewR.left) < 100)
		NewR.right = NewR.left+100;
	
	CDialog::OnSizing(fwSide, NewR);
	}

void CElementListDlog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CElementListDlog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CElementListDlog::OnItemchangedElementListCtl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	ListSelChange();
	
	*pResult = 0;
}

void CElementListDlog::OnKeydownElementListCtl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;	
	*pResult = 0;
}

void CElementListDlog::OnAddSelToGroup() 
{	
}

void CElementListDlog::OnSelectGroup() 
{	
}

void CElementListDlog::OnHideGroup() 
{
}

void CElementListDlog::OnNewGroup() 
{
}

void CElementListDlog::OnSelchangeGroupList() 
{
}

void CElementListDlog::OnAdd() 
{
	int	iCurSel = m_oListFilter.GetCurSel();
	
	if (iCurSel <1)
		return;

	int iData = m_oListFilter.GetItemData(iCurSel);

	int iItem = m_pDoc->m_oWorld.AddObject(m_pDoc->m_oWorld.GetFactoryName(iData));
	m_pDoc->m_oWorld.SetSelItem(iItem);
	Update();
	SelectItem(iItem);

	m_pDoc->UpdateAllViews(NULL);
}

void CElementListDlog::OnOdstatechangedElementListCtl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVODSTATECHANGE* pStateChanged = (NMLVODSTATECHANGE*)pNMHDR;
	// TODO: Add your control notification handler code here
	ListSelChange();
	
	*pResult = 0;
}
