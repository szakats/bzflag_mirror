// StdObjectInfoPanel.cpp : implementation file
//

#include "stdafx.h"
#include "bzedit32.h"
#include "StdObjectInfoPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStdObjectInfoPanel dialog


CStdObjectInfoPanel::CStdObjectInfoPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CStdObjectInfoPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStdObjectInfoPanel)
	m_fXPos = 0.0f;
	m_fYPos = 0.0f;
	m_fXScale = 0.0f;
	m_fYScale = 0.0f;
	m_fZPos = 0.0f;
	m_fZRot = 0.0f;
	m_fZScale = 0.0f;
	m_fBorderValue = 0.0f;
	m_bFlipZ = FALSE;
	m_bShootThru = FALSE;
	m_bDriveThru = FALSE;
	//}}AFX_DATA_INIT

	m_bShowBorder = false;
	m_bShowTeam = false;
}


void CStdObjectInfoPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStdObjectInfoPanel)
	DDX_Control(pDX, IDC_DRIVE_THRU, m_oDriveThruCB);
	DDX_Control(pDX, IDC_SHOOT_THRU, m_oShootThruCB);
	DDX_Control(pDX, IDC_FLIPZ, m_oFlipZCB);
	DDX_Control(pDX, IDC_TEAM_LIST, m_cTeamList);
	DDX_Control(pDX, IDC_TEAM_LABEL, m_cTeamLabel);
	DDX_Control(pDX, IDC_BORDER_LABEL, m_oBorderLabel);
	DDX_Control(pDX, IDC_BORDER, m_oBorderTE);
	DDX_Text(pDX, IDC_XPOS, m_fXPos);
	DDX_Text(pDX, IDC_YPOS, m_fYPos);
	DDX_Text(pDX, IDC_XSCALE, m_fXScale);
	DDX_Text(pDX, IDC_YSCALE, m_fYScale);
	DDX_Text(pDX, IDC_ZPOS, m_fZPos);
	DDX_Text(pDX, IDC_ZROT, m_fZRot);
	DDX_Text(pDX, IDC_ZSCALE, m_fZScale);
	DDX_Text(pDX, IDC_BORDER, m_fBorderValue);
	DDX_Check(pDX, IDC_FLIPZ, m_bFlipZ);
	DDX_Check(pDX, IDC_SHOOT_THRU, m_bShootThru);
	DDX_Check(pDX, IDC_DRIVE_THRU, m_bDriveThru);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStdObjectInfoPanel, CDialog)
	//{{AFX_MSG_MAP(CStdObjectInfoPanel)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStdObjectInfoPanel message handlers

BOOL CStdObjectInfoPanel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (m_bShowBorder)
	{
		m_oBorderLabel.ShowWindow(SW_NORMAL);
		m_oBorderTE.ShowWindow(SW_NORMAL);
	}

	if (m_bShowTeam)
	{
		m_cTeamList.ShowWindow(SW_NORMAL);
		m_cTeamLabel.ShowWindow(SW_NORMAL);

		m_cTeamList.SetItemData(m_cTeamList.AddString("Red"),1);
		m_cTeamList.SetItemData(m_cTeamList.AddString("Green"),2);
		m_cTeamList.SetItemData(m_cTeamList.AddString("Blue"),3);
		m_cTeamList.SetItemData(m_cTeamList.AddString("Purple"),4);

		m_cTeamList.SetCurSel(0);
	}
	SetDefID(IDC_XPOS);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStdObjectInfoPanel::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd	*pParent = GetParent();
	if (!pParent)
		return;

	pParent->SendMessage(WM_KEYUP,nChar,nFlags);

//	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CStdObjectInfoPanel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd	*pParent = GetParent();
	if (!pParent)
		return;

	pParent->SendMessage(WM_KEYDOWN,nChar,nFlags);
	
//	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
