// ElemInspDlog.cpp : implementation file
//

#include "stdafx.h"
#include "bzedit32.h"
#include "ElemInspDlog.h"
#include "BZEdit32View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemInspDlog dialog


CElemInspDlog::CElemInspDlog(CWnd* pParent /*=NULL*/)
	: CDialog(CElemInspDlog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemInspDlog)
	m_szType = _T("");
	//}}AFX_DATA_INIT

	m_pDoc = NULL;
	m_bWinUp = false;
	m_pView = NULL;

	m_iThisItem = -2;
	m_bItemHasData = false;

	m_pItemInterface = NULL;
}


void CElemInspDlog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemInspDlog)
	DDX_Control(pDX, IDC_ITEM_NAME_EDIT, m_oNameField);
	DDX_Control(pDX, IDC_CHILD_DLOG_FRAME, m_cInsertWindow);
	DDX_Text(pDX, IDC_ITEM_TYPE_EDIT, m_szType);
	DDX_Text(pDX, IDC_ITEM_NAME_EDIT, m_szName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElemInspDlog, CDialog)
	//{{AFX_MSG_MAP(CElemInspDlog)
	ON_BN_CLICKED(IDC_DUPLICATE, OnDuplicate)
	ON_BN_CLICKED(IDC_REMOVE_ITEM, OnRemoveItem)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemInspDlog message handlers

void CElemInspDlog::OnApply() 
{
	UpdateData(true);
	CBaseObject *pObject = m_pDoc->m_oWorld.GetObject(m_iThisItem);

	if (!pObject)
		return;

	if (!m_pItemInterface)
		return;

	pObject->ApplyEdit();

	pObject->SetItemName(m_szName);

	m_pDoc->UpdateAllViews(NULL);
	m_pDoc->SetModifiedFlag(true);
}

void CElemInspDlog::OnDuplicate() 
{
	if (m_bWinUp && m_pDoc && (m_pDoc->m_oWorld.GetSelItem() != -1))
	{
		CBaseObject *pObject = m_pDoc->m_oWorld.GetObject(m_pDoc->m_oWorld.GetSelItem());
		if (!pObject)
			return;

		int iCopy = m_pDoc->m_oWorld.AddObject(pObject->GetTypeName());
		CBaseObject *pCopy = m_pDoc->m_oWorld.GetObject(iCopy);
		pObject = m_pDoc->m_oWorld.GetObject(m_pDoc->m_oWorld.GetSelItem());

		if (!pCopy)
			return;

		pCopy->CopyFrom(pObject);

		CString	temp = pObject->GetItemName();

		if (temp.IsEmpty())
			temp = pObject->GetTypeName();
		
		char *pBuffer = temp.GetBuffer(temp.GetLength());

		char	*pCopyChar = strstr(pBuffer,"copy");

		if (pCopyChar)
		{
			pCopyChar += 4;
			int iNum = 0;

			if ((pCopyChar-pBuffer) ==0)
				iNum = 1;
			else
			{
				iNum = atoi(pCopyChar+1);
				iNum++;
			}

			*pCopyChar = NULL;

			temp.ReleaseBuffer();

			CString	temp2;

			temp2.Format(" %d",iNum);

			temp += temp2;
		}
		else
		{
			temp.ReleaseBuffer();
			temp += " copy";
		}

		pCopy->SetItemName((const char*)temp);

		m_pDoc->m_oWorld.SetSelItem(pCopy->GetID());

		((CBZEdit32View*)(m_pView))->m_oElemListDlog.Update();
		((CBZEdit32View*)(m_pView))->m_oElemListDlog.SelectItem(pCopy->GetID());

		m_pDoc->SetModifiedFlag(true);
	}
}

void CElemInspDlog::OnRemoveItem() 
{
	if (m_bWinUp && m_pDoc && (m_pDoc->m_oWorld.GetSelItem() != -1))
	{
		if (m_iThisItem != -1)
		{
			if (m_pItemInterface)
			{
				CBaseObject *pObject = m_pDoc->m_oWorld.GetObject(m_iThisItem);

				if (pObject)
					pObject->CloseEditInterface();
			}
			m_pItemInterface = NULL;
			m_iThisItem = -1;
		}

		m_pDoc->m_oWorld.RemoveObject(m_pDoc->m_oWorld.GetSelItem());
		m_pDoc->m_oWorld.SetSelItem(-1);

		m_pDoc->UpdateAllViews(NULL);
		m_pDoc->SetModifiedFlag(true);
	}
}

void CElemInspDlog::Update ( void )
{
	int				iThisItem = m_pDoc->m_oWorld.GetSelItem();
	CBaseObject		*pObject;

	if (iThisItem != m_iThisItem)
	{
		if (m_pItemInterface)
		{
			pObject = m_pDoc->m_oWorld.GetObject(m_iThisItem);

			if (pObject)
				pObject->CloseEditInterface();
		}
		m_pItemInterface = NULL;
		m_iThisItem = iThisItem;

		m_szName.Empty();
		m_szType.Empty();

		UpdateData(false);
	}

	pObject = m_pDoc->m_oWorld.GetObject(m_iThisItem);

	if (!pObject)
		return;

	// see if we need to get an interface
	if (!m_pItemInterface)
		m_pItemInterface = pObject->GetEditInterface((void*)(&m_cInsertWindow));
	
	m_szName = pObject->GetItemName();
	if (m_szName.IsEmpty())
		m_szName = pObject->GetTypeName();

	m_szType = pObject->GetTypeName();

	if (m_pItemInterface)
		pObject->UpdateEditInfo();

	UpdateData(false);
}

void CElemInspDlog::OnClose() 
{
	m_bWinUp = false;

	if (m_pItemInterface)
	{
		CBaseObject		*pObject = m_pDoc->m_oWorld.GetObject(m_iThisItem);

		if (pObject)
			pObject->CloseEditInterface();

		m_pItemInterface = NULL;
	}

	CDialog::OnClose();
}

void CElemInspDlog::OnDestroy() 
{
	if (m_pItemInterface)
	{
		CBaseObject		*pObject = m_pDoc->m_oWorld.GetObject(m_iThisItem);

		if (pObject)
			pObject->CloseEditInterface();

		m_pItemInterface = NULL;
	}
	
	CDialog::OnDestroy();
	m_bWinUp = false;
}

BOOL CElemInspDlog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bWinUp = true;

	SetDefID(IDC_APPLY);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemInspDlog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
	{
		OnApply();
	}
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CElemInspDlog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
