// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BZEdit32.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <io.h>
#include <direct.h>

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_MENUSELECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	std::vector<HMODULE>::iterator itr = m_vPluginHandles.begin();

	while (itr != m_vPluginHandles.end())
	{
		FreeLibrary(*itr);
		itr++;
	}
	m_vPluginHandles.clear();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#if (_MSC_VER < 1200) // VC5-
	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, AFX_IDW_TOOLBAR) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
#else // VC6+
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
#endif
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

#if (_MSC_VER < 1200) // VC5-
	if (!m_wndItemBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, AFX_IDW_TOOLBAR) ||
		!m_wndItemBar.LoadToolBar(IDR_NEWENT_BAR))
#else // VC6+
	if (!m_wndItemBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndItemBar.LoadToolBar(IDR_NEWENT_BAR))
#endif
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	EnableDocking(CBRS_ALIGN_TOP );
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP );
	m_wndItemBar.EnableDocking(CBRS_ALIGN_TOP );

// Hm, this does not compile under VC5 
#if (_MSC_VER >= 1200)
	RECT	rRect;

	rRect.top = 20;
	rRect.right = (LOWORD(m_wndToolBar.GetToolBarCtrl().GetButtonSize()) * m_wndToolBar.GetToolBarCtrl().GetButtonCount())+5;
	rRect.bottom = HIWORD(m_wndToolBar.GetToolBarCtrl().GetButtonSize())+20;
	rRect.left = 0;
//	m_wndToolBar.GetWindowRect(&rRect);

	DockControlBar(&m_wndToolBar);//,AFX_IDW_DOCKBAR_TOP ,&rRect);

	int iOffset = rRect.right - rRect.left;

	//m_wndItemBar.GetClientRect(&rRect);
	
//	rRect.left += iOffset;
//	rRect.right += iOffset;
	rRect.left = rRect.right + 5;
	rRect.top = 20;
	rRect.right = 1000;//rRect.left+LOWORD(m_wndItemBar.GetToolBarCtrl().GetButtonSize()) * m_wndItemBar.GetToolBarCtrl().GetButtonCount()+5;
	rRect.bottom = HIWORD(m_wndItemBar.GetToolBarCtrl().GetButtonSize())+20;

	DockControlBar(&m_wndItemBar);//,AFX_IDW_DOCKBAR_TOP ,&rRect);
#else // _MSC_VER < 1200
	// still need to do this for VC5
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndItemBar);
#endif

	LoadPlugins();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::LoadPlugins ( void )
{
	// load up all the plugins

	char	szPath[512];
	char	searchstr[512];
	GetModuleFileName(AfxGetInstanceHandle(),szPath,512);

	if (strrchr(szPath,'\\') != NULL)
		*(strrchr(szPath,'\\')+1) = NULL;

	strcat(szPath,"plugins\\");

	strcpy(searchstr,szPath);
	strcat(searchstr,"*.dll");
	
	m_vPluginHandles.clear();

	struct _finddata_t fileInfo;
	long	hFile;

	char	FilePath[1024];

	bool	bDone = false;

	int		iFiles = 0;

	hFile = _findfirst(searchstr,&fileInfo);

	HMODULE		hLib;

	CMenu* menu_bar = GetMenu();
	if (!menu_bar)
		return;

	int iCount = menu_bar->GetMenuItemCount();
	CMenu* pluginsMenu = menu_bar->GetSubMenu(3); 
	if (!pluginsMenu)
		return;

	iCount = pluginsMenu->GetMenuItemCount();

	int	iErr,(*lpProc)(char*);

	if (hFile != -1)
	{
		while (!bDone)
		{
			if ( (strcmp(fileInfo.name,".") != 0) && (strcmp(fileInfo.name,"..") != 0))
			{
				if ( !(fileInfo.attrib & _A_SUBDIR ))
				{
					strcpy(FilePath,szPath);
					strcat(FilePath,"\\");
					strcat(FilePath,fileInfo.name);

					hLib = LoadLibrary(FilePath);
					if (hLib)
					{
						lpProc = (int (__cdecl *)(char*))GetProcAddress(hLib, "init_plugin");
						if (lpProc)
						{
							char name[512];
							iErr = lpProc(name);

							if (iErr == 0)
							{
								pluginsMenu->InsertMenu(iFiles,MF_BYCOMMAND|MF_STRING,iFiles,name);
								iFiles++;

								m_vPluginHandles.push_back(hLib);
							}
						}
					}
				}
				if (_findnext(hFile,&fileInfo) == -1)
					bDone = true;
			}
		}
	}
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

		if (HIWORD(wParam) ==0)
		{
			int iItem = LOWORD(wParam);

			if (iItem < m_vPluginHandles.size())
			{
				int	iErr,(*lpProc)(void*);

				lpProc = (int (__cdecl *)(void*))GetProcAddress(m_vPluginHandles[iItem], "run_plugin");
				if (lpProc)
				{
					iErr = lpProc(NULL);
				}
			}
		}
	return CFrameWnd::OnCommand(wParam, lParam);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
	if ((nFlags & MF_SYSMENU) || (nFlags & MF_POPUP)  )
	{
		CMenu	*pMenu = GetMenu()->GetSubMenu(3);
		if (pMenu->GetSafeHmenu()== hSysMenu)
		{
			int i =10;

			i++;
		}
	}
	CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
}
