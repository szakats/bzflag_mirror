// DupeAndDlog.cpp : implementation file
//

#include "stdafx.h"
#include "bzedit32.h"
#include "DupeAndDlog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDupeAndDlog dialog


CDupeAndDlog::CDupeAndDlog(CWnd* pParent /*=NULL*/)
	: CDialog(CDupeAndDlog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDupeAndDlog)
	m_iNumDupes = 0;
	m_bRotate = FALSE;
	m_bScale = FALSE;
	m_bTransWithRot = FALSE;
	m_bTranslate = FALSE;
	m_fXScale = 0.0f;
	m_fXTrans = 0.0f;
	m_fYScale = 0.0f;
	m_fYTrans = 0.0f;
	m_fZRot = 0.0f;
	m_fZScale = 0.0f;
	m_fZTrans = 0.0f;
	//}}AFX_DATA_INIT
}


void CDupeAndDlog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDupeAndDlog)
	DDX_Control(pDX, IDC_TRANS_FRAME, m_oTransFrame);
	DDX_Control(pDX, IDC_SCALE_FRAME, m_oScaleFrame);
	DDX_Control(pDX, IDC_ROT_FRAME, m_oRotFrame);
	DDX_Text(pDX, IDC_NUM_DUPES, m_iNumDupes);
	DDX_Check(pDX, IDC_ROTATE, m_bRotate);
	DDX_Check(pDX, IDC_SCALE, m_bScale);
	DDX_Check(pDX, IDC_TRANS_W_ROT, m_bTransWithRot);
	DDX_Check(pDX, IDC_TRANSLATE, m_bTranslate);
	DDX_Text(pDX, IDC_X_SCALE_TE, m_fXScale);
	DDX_Text(pDX, IDC_X_TRANS_TE, m_fXTrans);
	DDX_Text(pDX, IDC_Y_SCALE_TE, m_fYScale);
	DDX_Text(pDX, IDC_Y_TRANS_TE, m_fYTrans);
	DDX_Text(pDX, IDC_Z_ROT_TE, m_fZRot);
	DDX_Text(pDX, IDC_Z_SCALE_TE, m_fZScale);
	DDX_Text(pDX, IDC_Z_TRANS_TE, m_fZTrans);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDupeAndDlog, CDialog)
	//{{AFX_MSG_MAP(CDupeAndDlog)
	ON_BN_CLICKED(IDC_TRANSLATE, OnTranslate)
	ON_BN_CLICKED(IDC_SCALE, OnScale)
	ON_BN_CLICKED(IDC_ROTATE, OnRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDupeAndDlog message handlers

BOOL CDupeAndDlog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	GetDlgItem(IDC_ROTATE)->UpdateWindow();
	GetDlgItem(IDC_SCALE)->UpdateWindow();
	GetDlgItem(IDC_TRANSLATE)->UpdateWindow();

	GetDlgItem(IDC_X_TRANS_TE)->EnableWindow(m_bTranslate);
	GetDlgItem(IDC_Y_TRANS_TE)->EnableWindow(m_bTranslate);
	GetDlgItem(IDC_Z_TRANS_TE)->EnableWindow(m_bTranslate);

	GetDlgItem(IDC_X_SCALE_TE)->EnableWindow(m_bScale);
	GetDlgItem(IDC_Y_SCALE_TE)->EnableWindow(m_bScale);
	GetDlgItem(IDC_Z_SCALE_TE)->EnableWindow(m_bScale);

	GetDlgItem(IDC_Z_ROT_TE)->EnableWindow(m_bRotate);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDupeAndDlog::OnTranslate() 
{
	m_bTranslate = !m_bTranslate;
	
	GetDlgItem(IDC_X_TRANS_TE)->EnableWindow(m_bTranslate);
	GetDlgItem(IDC_Y_TRANS_TE)->EnableWindow(m_bTranslate);
	GetDlgItem(IDC_Z_TRANS_TE)->EnableWindow(m_bTranslate);
}

void CDupeAndDlog::OnScale() 
{
	m_bScale = !m_bScale;
	
	GetDlgItem(IDC_X_SCALE_TE)->EnableWindow(m_bScale);
	GetDlgItem(IDC_Y_SCALE_TE)->EnableWindow(m_bScale);
	GetDlgItem(IDC_Z_SCALE_TE)->EnableWindow(m_bScale);
}

void CDupeAndDlog::OnRotate() 
{
	// TODO: Add your control notification handler code here
	m_bRotate = !m_bRotate;
	GetDlgItem(IDC_Z_ROT_TE)->EnableWindow(m_bRotate);

}
