// LinkDlog.cpp : implementation file
//

#include "stdafx.h"
#include "bzedit32.h"
#include "LinkDlog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkDlog dialog


CLinkDlog::CLinkDlog(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkDlog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinkDlog)
	//}}AFX_DATA_INIT

	m_pWorld = NULL;
}


void CLinkDlog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkDlog)
	DDX_Control(pDX, IDC_TO_TP, m_cToTPList);
	DDX_Control(pDX, IDC_TO_SIDE, m_cToSideList);
	DDX_Control(pDX, IDC_FROM_SIDE, m_cFromSideList);
	DDX_Control(pDX, IDC_FROM_TP, m_cFromTPList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkDlog, CDialog)
	//{{AFX_MSG_MAP(CLinkDlog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkDlog message handlers

void CLinkDlog::SetSel ( int to, int from )
{
	int iTP = 0;
	bool bFront;

	m_cToSideList.SetCurSel(0);
	if (to == -1)
		m_cToTPList.SetCurSel(0);
	else
	{
		iTP = to/2;
		bFront = to%2 == 0;

		m_cToTPList.SetCurSel(iTP+1);
		if (!bFront)
			m_cToSideList.SetCurSel(1);
	}
		
	m_cFromSideList.SetCurSel(0);
	if (from == -1)
		m_cFromTPList.SetCurSel(0);
	else
	{
		iTP = from/2;
		bFront = from%2 == 0;

		m_cFromTPList.SetCurSel(iTP+1);
		if (!bFront)
			m_cFromSideList.SetCurSel(1);
	}
}

void CLinkDlog::GetSel ( int &to, int &from )
{
	if (m_cToTPList.GetCurSel() == 0)
		to = -1;
	else
		to = ((m_cToTPList.GetCurSel()-1)*2)+m_cToSideList.GetCurSel();
		
	if (m_cFromTPList.GetCurSel() == 0)
		from = -1;
	else
		from = ((m_cFromTPList.GetCurSel()-1)*2)+m_cFromSideList.GetCurSel();
}

BOOL CLinkDlog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cFromTPList.SetItemData(m_cFromTPList.AddString("None"),-1);
	m_cToTPList.SetItemData(m_cToTPList.AddString("None"),-1);

	m_cToSideList.SetItemData(m_cToSideList.AddString("Front (Y+)"),1);
	m_cToSideList.SetItemData(m_cToSideList.AddString("Back (Y-)"),2);

	m_cFromSideList.SetItemData(m_cFromSideList.AddString("Front (Y+)"),1);
	m_cFromSideList.SetItemData(m_cFromSideList.AddString("Back (Y-)"),2);
	
	trObjectList	cList;
	trObjectList::iterator itr;
	if (m_pWorld)
	{
		m_pWorld->GetObjectListOfType ( "Teleporter", cList );

		itr = cList.begin();

		while ( itr != cList.end() )
		{
			m_cFromTPList.SetItemData(m_cFromTPList.AddString((*itr)->GetListName()),(*itr)->GetID());
			m_cToTPList.SetItemData(m_cToTPList.AddString((*itr)->GetListName()),(*itr)->GetID());
			itr++;
		}
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
}
